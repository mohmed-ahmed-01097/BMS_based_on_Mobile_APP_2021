
#ifndef EEPROM_H
#define EEPROM_H

#include "data_type.h"                                                  // DataType library

// EEPROM Constants
#define EEPROM_MAX            512                                       // EEPROM MAX
#define S_TIMER_SIZE          sizeof(S_Timer)                           // Struct Timer size
#define S_POWER_SIZE          4                                         // Struct Power size
#define MAX_MEASURE_HR        72                                        // Day's Hours ==> 3*24 

// EEPROM Pointer
#define EEPROM_WIFI_SSID_S    0                                         // SSID ==> 32
#define EEPROM_WIFI_SSID_E    32                                        // 0 --> 32

#define EEPROM_WIFI_PASS_S    32                                        // PASS ==> 64 
#define EEPROM_WIFI_PASS_E    96                                        // 32 --> 96

#define EEPROM_AP_SSID_S      96                                        // SSID ==> 32
#define EEPROM_AP_SSID_E      128                                       // 69 --> 128

#define EEPROM_AP_PASS_S      128                                       // PASS ==> 64 
#define EEPROM_AP_PASS_E      192                                       // 128 --> 192

#define EEPROM_TIMER_S        192                                       // Timer==> 5*5
#define EEPROM_TIMER_E        217                                       // 192 --> 217

#define EEPROM_DATE_DD        217                                       // Date ==> 3
#define EEPROM_DATE_MM        218
#define EEPROM_DATE_YY        219

#define EEPROM_NOW_HR         220                                       // Now ==> 2
#define EEPROM_NOW_MIN        221                                       

#define EEPROM_HOUR_S         222                                       // Hour  ==> 24 * 4
#define EEPROM_HOUR_E         318                                       

#define EEPROM_DAY_S          318                                       // Day   ==> 31 * 4 
#define EEPROM_DAY_E          442

#define EEPROM_MONTH_S        442                                       // Month ==> 12 * 4
#define EEPROM_MONTH_E        490                                       // 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// EEPROM Initialization
void EEPROM_Init(S_MyData *_data); 

// WiFi EEPROM
void EEPROM_Read_WiFi (      String &_ssid,       String &_pass);       // Read WiFi EEPROM
void EEPROM_Write_WiFi(const String &_ssid, const String &_pass);       // Write WiFi EEPROM

// AP EEPROM
void EEPROM_Read_AP (      String &_ssid,       String &_pass);         // Read AP EEPROM
void EEPROM_Write_AP(const String &_ssid, const String &_pass);         // Write AP EEPROM

// Time EEPROM
void EEPROM_Read_Time (      S_Time *_now);                             // Read Time
void EEPROM_Write_Time(const S_Time *_now);                             // Write Time

// Date EEPROM
void EEPROM_Read_Date (      S_Date *_today);                           // Read Date
void EEPROM_Write_Date(const S_Date *_today);                           // Write Date

// Timer EEPROM
void EEPROM_Read_Timer (      S_Timer *_timer);                         // Read Timer
void EEPROM_Write_Timer(const S_Timer *_timer);                         // Write Timer

// Power EEPROM
void EEPROM_Monitor_Write_H(const uint16_t _index, const float _data);
void EEPROM_Monitor_Write_D(const uint16_t _index, const uint8_t _end);
void EEPROM_Monitor_Write_M(const uint16_t _index, const uint8_t _end);

float EEPROM_Monitor_Read_H(const uint16_t _index);
float EEPROM_Monitor_Read_D(const uint16_t _index);
float EEPROM_Monitor_Read_M(const uint16_t _index);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

