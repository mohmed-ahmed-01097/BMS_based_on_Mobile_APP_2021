
#include "Time.h"                                                       // Timer library
#include "EEPROM.h"                                                     // EEPROM library
#include <NTPClient.h>                                                  // time Library 
#include <WiFiUdp.h>
#include <time.h>
#include <TimeLib.h>

WiFiUDP Udp;                                                        // Define NTP Client to get time
//NTPClient timeClient(Udp, "pool.ntp.org");
NTPClient timeClient(Udp, "eg.pool.ntp.org");
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Month names
String months[12]  = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// initialization web time
// >1msec
void Time_Init(S_Time *_now, S_Date *_today) {
  
  timeClient.begin();                                                   // Initialize a NTPClient to get time
  timeClient.setTimeOffset(TIME_ZONE_OFFSET);                           // Set the Time Offset
  Time_GetSec();
  Time_GetTime(_now);                                                   // Get the Time
  Time_GetDate(_today);                                                 // Get the Date
  EEPROM_Write_Time(_now);
  EEPROM_Write_Date(_today); 
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// updata the Time and the Date
// >1msec if normal run
// 63msec if store to eeprom
void Time_UpDate(S_Time *_now, S_Date *_today, bool _wifi_state){
  
  static uint8_t  Sec = 0;
  if(++Sec >= 60){
    Sec = 0;
    if(++_now->Min >= 60){
      _now->Min = 0;
      if(++_now->Hr >= 24){
        _now->Hr = 0;
      }
    }
    
    if(_wifi_state) {                                                  // Test the connection
      Sec = Time_GetSec();                                            // UpDate the now time 
      Time_GetTime(_now);
      if(!_now->Min){
        Time_GetDate(_today);
      }
    }
    EEPROM_Write_Time(_now);
    EEPROM_Write_Date(_today); 
  }
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Return the secondes number and updata the web clinet 
// >167msec
uint8_t Time_GetSec(void) {
  uint8_t i=0;
  while(!timeClient.update()) {                                         // Update the Time
    //timeClient.forceUpdate();
    if(i++ > 5)
      break;
  } 
  return uint8_t(timeClient.getSeconds());                              // Return the Sec
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Return the day name
String Time_GetDay(void) {
  return weekDays[timeClient.getDay()];                                 // Return the Week Day Name
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Return the Month name
String Time_GetMonth(S_Date *_today) {
  return months[_today->MM - 1];                                         // Return the Month Name
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// get the time of now
void Time_GetTime(S_Time *_now){
  _now->Hr  = timeClient.getHours();                                     // Set Time Hour
  _now->Min = timeClient.getMinutes();                                   // Set Time Minute 
}  

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// get the date of today
void Time_GetDate(S_Date *_today) {
 
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((const time_t *)&epochTime);                 // Get a time structure
  if((_today->DD != ptm->tm_mday)){                                     // if the DD is equal tm_mday
    _today->DD = ptm->tm_mday;
    _today->MM = ptm->tm_mon+1;
    _today->YY = ptm->tm_year;
  }
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

