
#include "EEPROM.h"                                                     // eeprom library
#include <EEPROM.h>                                                     // eeprom library

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EEPROM Initialization
// >330msec
void EEPROM_Init(S_MyData *_data){
  
  EEPROM.begin(EEPROM_MAX);                                             // Initialasing EEPROM
  
  EEPROM_Read_Time (&_data->Now  );                                      // Read Time
  EEPROM_Read_Date (&_data->Date );                                      // Read Date
  EEPROM_Read_Timer( _data->Timer);                                      // Read Timer
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                             
// Read Time
// >1msec
// Reading EEPROM Time
// Last Time 255:0
void EEPROM_Read_Time(S_Time *_now) {
   
  //Serial.println("Reading EEPROM Time");
  _now->Hr  = uint8_t(EEPROM.read(EEPROM_NOW_HR ));                      // Read the Hr
  _now->Min = uint8_t(EEPROM.read(EEPROM_NOW_MIN));                      // Read the Min
  Serial.println("Last Time " + String(_now->Hr) + ":" + String(_now->Min));

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Write Time
// >21msec
// Writing EEPROM Time
// Write Time 255:3
void EEPROM_Write_Time(const S_Time *_now)  {

  static S_Time last;
  //Serial.println("Writing EEPROM Time");
  if(last.Hr != _now->Hr){
    last.Hr = _now->Hr;
    EEPROM.write(EEPROM_NOW_HR , _now->Hr );                               // Write the Hr
  }
  if(last.Min != _now->Min){
    last.Min = _now->Min;
    EEPROM.write(EEPROM_NOW_MIN, _now->Min);                               // Write the Min
  }
  EEPROM.commit();
  Serial.println("Write Time " + String(_now->Hr) + ":" + String(_now->Min));
   
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                          
// Read Date
// >10msec
// Reading EEPROM Date
// Today Date 255:255:255
void EEPROM_Read_Date(S_Date *_today) {
  
  //Serial.println("Reading EEPROM Date");
  _today->DD = uint8_t(EEPROM.read(EEPROM_DATE_DD));                     // Read DD
  _today->MM = uint8_t(EEPROM.read(EEPROM_DATE_MM));                     // Read MM
  _today->YY = uint8_t(EEPROM.read(EEPROM_DATE_YY));                     // Read YY
  Serial.println("Today Date " + String(_today->DD) + ":" + String(_today->MM) + ":" + String(_today->YY));
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                           
// Write Date
// >28msec
// Writing EEPROM Date
// Write Date 255:255:255
void EEPROM_Write_Date(const S_Date *_today)  {
   
  static S_Date last;
  //Serial.println("Writing EEPROM Date");
  if(last.DD != _today->DD){
    last.DD = _today->DD;
    EEPROM.write(EEPROM_DATE_DD, _today->DD);                              // Read DD 
  }
  if(last.MM != _today->MM){
    last.MM = _today->MM;
    EEPROM.write(EEPROM_DATE_MM, _today->MM);                              // Read MM 
  }
  if(last.YY != _today->YY){
    last.YY = _today->YY;
    EEPROM.write(EEPROM_DATE_YY, _today->YY);                              // Read YY
  }
  EEPROM.commit();
  Serial.println("Write Date " + String(_today->DD) + ":" + String(_today->MM) + ":" + String(_today->YY));
   
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Read Timer
// >300msec
// Reading EEPROM Timer
// Timer[0]= State 1  Start 255:255  END   255:255
// Timer[1]= State 1  Start 255:255  END   255:255
// Timer[2]= State 1  Start 255:255  END   255:255
// Timer[3]= State 1  Start 255:255  END   255:255
// Timer[4]= State 1  Start 255:255  END   255:255

void EEPROM_Read_Timer(S_Timer *_timer){  
     
  //Serial.println("Reading EEPROM Timer");
  for (uint8_t i = 0, j = EEPROM_TIMER_S; i<MAX_TIMER ; i++){
    _timer[i].State     = bool   (EEPROM.read(j++));                     // Timer State
    _timer[i].Start.Hr  = uint8_t(EEPROM.read(j++));                     // Timer Start Hr
    _timer[i].Start.Min = uint8_t(EEPROM.read(j++));                     // Timer Start Min
    _timer[i].End.Hr    = uint8_t(EEPROM.read(j++));                     // Timer End Hr
    _timer[i].End.Min   = uint8_t(EEPROM.read(j++));                     // Timer End Min
    //Serial.println("Timer[" + String(i) + "]= State " + String(_timer[i].State));
    //Serial.println("\tStart " + String(_timer[i].Start.Hr)+ ":" + String(_timer[i].Start.Min));
    //Serial.println("\tEND   " + String(_timer[i].End.Hr)  + ":" + String(_timer[i].End.Min  ));
  }
   
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Write Timer
// >125msec
// Writing EEPROM Timer
// Timer[0]= State 0  Start 0:0  END   0:0
// Timer[1]= State 0  Start 0:0  END   0:0
// Timer[2]= State 0  Start 0:0  END   0:0
// Timer[3]= State 0  Start 0:0  END   0:0
// Timer[4]= State 0  Start 0:0  END   0:0
void EEPROM_Write_Timer(const S_Timer *_timer){
  
  //Serial.println("Writing EEPROM Timer");
  for (uint8_t i = 0, j = 0; i<MAX_TIMER ; i++){
    j = EEPROM_TIMER_S + i * S_TIMER_SIZE;
    if(_timer[i].State != bool(EEPROM.read(j))){
      EEPROM.write(j++, _timer[i].State    );                            // Timer State
      EEPROM.write(j++, _timer[i].Start.Hr );                            // Timer Start Hr
      EEPROM.write(j++, _timer[i].Start.Min);                            // Timer Start Min
      EEPROM.write(j++, _timer[i].End.Hr   );                            // Timer End Hr
      EEPROM.write(j++, _timer[i].End.Min  );                            // Timer End Min
      Serial.println("Timer[" + String(i) + "]= State " + String(_timer[i].State));
      Serial.println("\tStart " + String(_timer[i].Start.Hr)+ ":" + String(_timer[i].Start.Min));
      Serial.println("\tEND   " + String(_timer[i].End.Hr)  + ":" + String(_timer[i].End.Min  ));
    }
  } 
  EEPROM.commit();
   
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Read WiFi EEPROM
// >103msec
// Reading EEPROM SSID
// SSID: MAAM
// Reading EEPROM PASS
// PASS: 12345678
void EEPROM_Read_WiFi(String &_ssid, String &_pass) {
   
  //Serial.println("Reading EEPROM SSID");                                //Read eeprom for ssid
  for (uint8_t i = EEPROM_WIFI_SSID_S; i < EEPROM_WIFI_SSID_E; i++) {
    _ssid += char(EEPROM.read(i));
  }
  Serial.println("SSID: " + _ssid);
  //Serial.println("Reading EEPROM PASS");                                //Read eeprom for pass
  for (uint8_t i = EEPROM_WIFI_PASS_S; i < EEPROM_WIFI_PASS_E; i++) {
    _pass += char(EEPROM.read(i));
  }
  Serial.println("PASS: " + _pass);
   
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////         
// Write WiFi EEPROM
// >300msec
// Clearing EEPROM
// SSID: MAAM
// PASS: 12345678
// Writing EEPROM SSID:
// Wrote: M
// Wrote: A
// Wrote: A
// Wrote: M
// Writing EEPROM PASS:
// Wrote: 1
// Wrote: 2
// Wrote: 3
// Wrote: 4
// Wrote: 5
// Wrote: 6
// Wrote: 7
// Wrote: 8
void EEPROM_Write_WiFi(const String &_ssid, const String &_pass)  {
   
  //Serial.println("Clearing EEPROM");                                    // Clean the EEPROM
  for (uint8_t i = EEPROM_WIFI_SSID_S; i < EEPROM_WIFI_PASS_E; i++) {
    EEPROM.write(i, 0);                                                 // Set Zero in EEPROM 
  }
  Serial.println("SSID: " + _ssid);
  Serial.println("PASS: " + _pass);
  //Serial.println("Writing EEPROM SSID:");                               // Write the SSID
  for (uint8_t  i = 0; i < _ssid.length(); i++) {
    EEPROM.write(EEPROM_WIFI_SSID_S + i, _ssid[i]);
    //Serial.println("Wrote: " + String(_ssid[i]));
  }
  //Serial.println("Writing EEPROM PASS:");                               // Write the PASS
  for (uint8_t i = 0; i < _pass.length(); i++) {
    EEPROM.write(EEPROM_WIFI_PASS_S + i, _pass[i]);
    //Serial.println("Wrote: " + String(_pass[i]));
  }
  EEPROM.commit();
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

