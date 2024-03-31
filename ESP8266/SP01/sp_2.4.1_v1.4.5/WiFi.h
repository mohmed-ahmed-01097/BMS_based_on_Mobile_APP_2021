
#ifndef WIFI_H
#define WIFI_H

#include "data_type.h"                                                  // DataType library
#include <ESP8266WiFi.h>                                                // esp8266 library

//#define WIFI_SSID       "MAAM"                                          // input your home or public wifi name 
//#define WIFI_PASS       "01097502015Newmgh"                             // password of wifi ssid
#define WIFI_SSID       "Nasem"                                    // input your home or public wifi name 
#define WIFI_PASS       "Nasem90909"                                      // password of wifi ssid

#define MAX_CONN        4
#define CHANNEL         6
#define WIFI_SCAN_RUNNING   (-1)
#define WIFI_SCAN_FAILED    (-2)
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const String menu[]    = {"Home"             , "Measure"                , "Timers"                                       , "Config"                                                                    };
const String head[]    = {"ON - OFF Control" , "Parameters Measurement" , "Timer Control"                                , "ESP8266 Wi-Fi Maneger"                           };
const String comment[] = {""                 , ""                       , "five Timer start from Start Timr to End Time" , "ESP8266 Web Server Wifi Credentials Update page"};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ESP.getFlashChipId();                    //returns the flash chip ID as a 32-bit integer.
// ESP.getFlashChipSize();                  //returns the flash chip size, in bytes, as seen by the SDK (may be less than actual size).
// ESP.getFlashChipSpeed(void);             //returns the flash chip frequency, in Hz.
// ESP.restart();                           //restarts the CPU.
// WiFi.softAPgetStationNum()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Set_Data_Ptr(S_MyData *_data);

// connect the WiFf
void WiFi_Init(void);                                                   // Initilize the WiFi
bool WiFi_Test(void);                                                   // Testing the WiFi
void WiFi_ShortTest(void);                                              // Testing the WiFi
void WiFi_Check(void);                                                  // stay check the WiFi untell Connecting

// setup the WiFi
void WiFi_Setup(void);                                                  // Setup to change the WiFi
void WiFi_LaunchWeb(void);                                              // Launch the web server
void WiFi_SetupMode(void);                                              // Setup the AP+STA mode  

// Web server 
void WiFi_CreatWebServer(void);                                         // Creat Web server
void WiFi_ScanAP(void);                                                 // Scan For near AP 

String processor(const String& var);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

