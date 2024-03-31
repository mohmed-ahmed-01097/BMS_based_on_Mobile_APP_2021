
#include "EEPROM.h"                                                     // eeprom library
#include <EEPROM.h>                                                     // eeprom library

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EEPROM Initialization
// >32.177msec
void EEPROM_Init(S_MyData *_data){
  
  //Serial.print  (micros()); Serial.println( "\tEE_S"); 
  EEPROM.begin(EEPROM_MAX);                                             // Initialasing EEPROM ~~ 164usec
  EEPROM_Read_Time (&_data->Now  );                                      // Read Time
  EEPROM_Read_Date (&_data->Date );                                      // Read Date
  EEPROM_Read_Timer( _data->Timer);                                      // Read Timer
  //Serial.print  (micros()); Serial.println( "\tEE_E"); 
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                             
// Read Time
// >0.143msec
// Reading EEPROM Time
// Last Time 255:255
void EEPROM_Read_Time(S_Time *_now) {
   
  //Serial.print  (micros()); Serial.println( "\tEt_S"); 
  //Serial.println("Reading EEPROM Time");
  _now->Hr  = uint8_t(EEPROM.read(EEPROM_NOW_HR ));                      // Read the Hr
  _now->Min = uint8_t(EEPROM.read(EEPROM_NOW_MIN));                      // Read the Min
  if(_now->Hr  >= MAX_HOURS || _now->Min >= MAX_MINUTES){
    _now->Hr  = 0;
    _now->Min = 0;
  }   
  Serial.println("Last Time " + String(_now->Hr) + ":" + String(_now->Min));
  //Serial.print  (micros()); Serial.println( "\tEt_E"); 
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Write Time
// >0.145msec
// Writing EEPROM Time
// Write Time 255:3
void EEPROM_Write_Time(const S_Time *_now)  {
     
  //Serial.print  (micros()); Serial.println( "\tEt_S"); 
  //Serial.println("Writing EEPROM Time");
  EEPROM.write(EEPROM_NOW_HR , _now->Hr );                               // Write the Hr
  EEPROM.write(EEPROM_NOW_MIN, _now->Min);                               // Write the Min
  EEPROM.commit();
  Serial.println("Write Time " + String(_now->Hr) + ":" + String(_now->Min));
  //Serial.print  (micros()); Serial.println( "\tEt_E"); 
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                          
// Read Date
// >10msec
// Reading EEPROM Date
// Today Date 255:255:255
void EEPROM_Read_Date(S_Date *_today) {
  
  //Serial.print  (micros()); Serial.println( "\tED_S"); 
  //Serial.println("Reading EEPROM Date");
  _today->DD = uint8_t(EEPROM.read(EEPROM_DATE_DD));                     // Read DD
  _today->MM = uint8_t(EEPROM.read(EEPROM_DATE_MM));                     // Read MM
  _today->YY = uint8_t(EEPROM.read(EEPROM_DATE_YY));                     // Read YY
  if(_today->DD  >= MAX_DAYS || _today->MM  >= MAX_MONTHS){
    _today->DD  = 1;
    _today->MM  = 1;
    _today->YY  = 121;
  }   
  Serial.println("Today Date " + String(_today->DD) + ":" + String(_today->MM) + ":" + String(_today->YY));
  //Serial.print  (micros()); Serial.println( "\tED_E"); 
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                           
// Write Date
// >28msec
// Writing EEPROM Date
// Write Date 255:255:255
void EEPROM_Write_Date(const S_Date *_today)  {
   
  //Serial.print  (micros()); Serial.println( "\tED_S"); 
  //Serial.println("Writing EEPROM Date");
  EEPROM.write(EEPROM_DATE_DD, _today->DD);                              // Read DD 
  EEPROM.write(EEPROM_DATE_MM, _today->MM);                              // Read MM 
  EEPROM.write(EEPROM_DATE_YY, _today->YY);                              // Read YY
  EEPROM.commit();
  Serial.println("Write Date " + String(_today->DD) + ":" + String(_today->MM) + ":" + String(_today->YY));
  //Serial.print  (micros()); Serial.println( "\tED_E"); 
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Read Timer
// >188.343msec
// Reading EEPROM Timer
// Timer[0]= State 1  Start 255:255  END   255:255
// Timer[1]= State 1  Start 255:255  END   255:255
// Timer[2]= State 1  Start 255:255  END   255:255
// Timer[3]= State 1  Start 255:255  END   255:255
// Timer[4]= State 1  Start 255:255  END   255:255

void EEPROM_Read_Timer(S_Timer *_timer){  
     
  //Serial.print  (micros()); Serial.println( "\tET_S"); 
  //Serial.println("Reading EEPROM Timer");
  for (uint8_t i = 0, j = EEPROM_TIMER_S; i<MAX_TIMER ; i++){
    _timer[i].State     = bool   (EEPROM.read(j++));                     // Timer State
    _timer[i].Start.Hr  = uint8_t(EEPROM.read(j++));                     // Timer Start Hr
    _timer[i].Start.Min = uint8_t(EEPROM.read(j++));                     // Timer Start Min
    _timer[i].End.Hr    = uint8_t(EEPROM.read(j++));                     // Timer End Hr
    _timer[i].End.Min   = uint8_t(EEPROM.read(j++));                     // Timer End Min
    
    if(_timer[i].Start.Hr >= MAX_HOURS || _timer[i].Start.Min >= MAX_MINUTES || _timer[i].End.Hr >= MAX_HOURS || _timer[i].End.Min >= MAX_MINUTES){
      _timer[i].State     = false;
      _timer[i].Start.Hr  = 0;
      _timer[i].Start.Min = 0;
      _timer[i].End.Hr    = 0;
      _timer[i].End.Min   = 0;
    } 
    
    Serial.print  ("Timer[" + String(i) + "]= State " + String(_timer[i].State));
    Serial.print  (" Start " + String(_timer[i].Start.Hr)+ ":" + String(_timer[i].Start.Min));
    Serial.println(" END   " + String(_timer[i].End.Hr)  + ":" + String(_timer[i].End.Min  ));
  }
  //Serial.print  (micros()); Serial.println( "\tET_E"); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Write Timer
void EEPROM_Write_Timer(const S_Timer *_timer){
     
  //Serial.print  (micros()); Serial.println( "\tET_S"); 
  //Serial.println("Writing EEPROM Timer");
  for (uint8_t i = 0, j = 0; i<MAX_TIMER ; i++){
    j = EEPROM_TIMER_S + i * S_TIMER_SIZE;
    if(_timer[i].State != bool(EEPROM.read(j)) || _timer[i].State){
      EEPROM.write(j++, _timer[i].State    );                            // Timer State
      EEPROM.write(j++, _timer[i].Start.Hr );                            // Timer Start Hr
      EEPROM.write(j++, _timer[i].Start.Min);                            // Timer Start Min
      EEPROM.write(j++, _timer[i].End.Hr   );                            // Timer End Hr
      EEPROM.write(j++, _timer[i].End.Min  );                            // Timer End Min
      Serial.print  ("Timer[" + String(i) + "]= State " + String(_timer[i].State));
      Serial.print  (" Start " + String(_timer[i].Start.Hr)+ ":" + String(_timer[i].Start.Min));
      Serial.println(" END   " + String(_timer[i].End.Hr)  + ":" + String(_timer[i].End.Min  ));
    }
  } 
  EEPROM.commit();
  //Serial.print  (micros()); Serial.println( "\tET_E");  
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Read WiFi EEPROM
// >103msec
// Reading EEPROM SSID
// SSID: MAAM
// Reading EEPROM PASS
// PASS: 12345678
void EEPROM_Read_WiFi(String &_ssid, String &_pass) {
   
  //Serial.print  (micros()); Serial.println( "\tEW_S"); 
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
  //Serial.print  (micros()); Serial.println( "\tEW_E"); 
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
   
  //Serial.print  (micros()); Serial.println( "\tEW_S"); 
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
  //Serial.print  (micros()); Serial.println( "\tEW_E"); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Read AP EEPROM
// >103msec
// Reading EEPROM SSID
// SSID: MAAM
// Reading EEPROM PASS
// PASS: 12345678
void EEPROM_Read_AP(String &_ssid, String &_pass) {
   
  //Serial.print  (micros()); Serial.println( "\tEW_S"); 
  //Serial.println("Reading EEPROM SSID");                                //Read eeprom for ssid
  for (uint8_t i = EEPROM_AP_SSID_S; i < EEPROM_AP_SSID_E; i++) {
    _ssid += char(EEPROM.read(i));
  }
  Serial.println("SSID: " + _ssid);
  //Serial.println("Reading EEPROM PASS");                                //Read eeprom for pass
  for (uint8_t i = EEPROM_AP_PASS_S; i < EEPROM_AP_PASS_E; i++) {
    _pass += char(EEPROM.read(i));
  }
  Serial.println("PASS: " + _pass);
  //Serial.print  (micros()); Serial.println( "\tEW_E"); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////         
// Write AP EEPROM
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
void EEPROM_Write_AP(const String &_ssid, const String &_pass)  {
   
  //Serial.print  (micros()); Serial.println( "\tEW_S"); 
  //Serial.println("Clearing EEPROM");                                    // Clean the EEPROM
  for (uint8_t i = EEPROM_AP_SSID_S; i < EEPROM_AP_PASS_E; i++) {
    EEPROM.write(i, 0);                                                 // Set Zero in EEPROM 
  }
  Serial.println("SSID: " + _ssid);
  Serial.println("PASS: " + _pass);
  //Serial.println("Writing EEPROM SSID:");                               // Write the SSID
  for (uint8_t  i = 0; i < _ssid.length(); i++) {
    EEPROM.write(EEPROM_AP_SSID_S + i, _ssid[i]);
    //Serial.println("Wrote: " + String(_ssid[i]));
  }
  //Serial.println("Writing EEPROM PASS:");                               // Write the PASS
  for (uint8_t i = 0; i < _pass.length(); i++) {
    EEPROM.write(EEPROM_AP_PASS_S + i, _pass[i]);
    //Serial.println("Wrote: " + String(_pass[i]));
  }
  EEPROM.commit();
  //Serial.print  (micros()); Serial.println( "\tEW_E"); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

