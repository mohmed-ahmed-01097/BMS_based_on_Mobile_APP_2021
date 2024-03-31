
#include "Time.h"                                                       // Timer library
#include <NTPClient.h>                                                  // time Library 
#include <WiFiUdp.h>
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
// >216.262msec
void Time_Init(S_Time *_now, S_Date *_today) {
  
  Serial.print  (micros()); Serial.println( "\tTS_S"); 
  timeClient.begin();                                                   // Initialize a NTPClient to get time
  timeClient.setTimeOffset(TIME_ZONE_OFFSET);                           // Set the Time Offset
  Serial.print  (micros()); Serial.println( "\tTS_E"); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Return the secondes number and updata the web clinet 
// >167msec
void Time_GetSec (uint8_t *_sec) {
  Serial.print  (micros()); Serial.println( "\tTG_S"); 
  uint8_t i = 0;
  while(!timeClient.update()) {                                         // Update the Time
    //timeClient.forceUpdate();
    if(i++ > 5)
      break;
  } 
  *_sec = timeClient.getSeconds();                              // Return the Sec
  Serial.print  (micros()); Serial.println( "\tTG_E"); 
  
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
// > 0.099msec
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

