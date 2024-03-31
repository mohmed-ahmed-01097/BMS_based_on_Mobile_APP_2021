
#ifndef EEPROM_H
#define  EEPROM_H

#include "data_type.h"                                                  // DataType library

// EEPROM Constants
#define EEPROM_MAX            512                                       // EEPROM MAX
#define S_TIMER_SIZE          sizeof(S_Timer)                           // Struct Timer size
#define S_POWER_SIZE          4                                         // Struct Power size
#define MAX_MEASURE_HR        96                                        // Day's Hours ==> 4*24 

// EEPROM Pointer
#define EEPROM_WIFI_SSID_S    0                                         // SSID ==> 32
#define EEPROM_WIFI_SSID_E    32                                        // 0 --> 32

#define EEPROM_WIFI_PASS_S    32                                        // PASS ==> 64 
#define EEPROM_WIFI_PASS_E    96                                        // 32 --> 96

#define EEPROM_TIMER_S        96                                        // Timer==> 5*5
#define EEPROM_TIMER_E        121                                       // 96 --> 121

#define EEPROM_DATE_DD        121                                       // Date ==> 3
#define EEPROM_DATE_MM        122
#define EEPROM_DATE_YY        123

#define EEPROM_NOW_HR         124                                       // Now ==> 2
#define EEPROM_NOW_MIN        125                                       

#define EEPROM_STACK_PTR      126                                       // Ptr ==> 2
#define EEPROM_STACK_HR       127

#define EEPROM_POWER_S        128                                       // Power ==> 24*4*4
#define EEPROM_POWER_E        512                                       // 128 --> 512

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// EEPROM Initialization
void EEPROM_Init(S_MyData *_data); 

// WiFi EEPROM
void EEPROM_Read_WiFi (      String &_ssid,       String &_pass);       // Read WiFi EEPROM
void EEPROM_Write_WiFi(const String &_ssid, const String &_pass);       // Write WiFi EEPROM

// Time EEPROM
void EEPROM_Read_Time (      S_Time *_now);                             // Read Time
void EEPROM_Write_Time(const S_Time *_now);                             // Write Time

// Date EEPROM
void EEPROM_Read_Date (      S_Date *_today);                           // Read Date
void EEPROM_Write_Date(const S_Date *_today);                           // Write Date

// Timer EEPROM
void EEPROM_Read_Timer (      S_Timer *_timer);                         // Read Timer
void EEPROM_Write_Timer(const S_Timer *_timer);                         // Write Timer

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

