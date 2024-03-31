
#ifndef WIFI_H
#define WIFI_H

#include "data_type.h"                                                  // DataType library
#include <ESP8266WiFi.h>                                                // esp8266 library

#define WIFI_SSID       "MAAM"                                          // input your home or public wifi name 
#define WIFI_PASS       "01097502015Newmgh"                             // password of wifi ssid

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ESP.getFlashChipId();                    //returns the flash chip ID as a 32-bit integer.
// ESP.getFlashChipSize();                  //returns the flash chip size, in bytes, as seen by the SDK (may be less than actual size).
// ESP.getFlashChipSpeed(void);             //returns the flash chip frequency, in Hz.
// ESP.restart();                           //restarts the CPU.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// connect the WiFf
void WiFi_Init(void);                                                   // Initilize the WiFi
bool WiFi_Test(void);                                                   // Testing the WiFi
void WiFi_Check(void);                                                  // stay check the WiFi untell Connecting

// setup the WiFi
void WiFi_Setup(void);                                                  // Setup to change the WiFi
void WiFi_LaunchWeb(void);                                              // Launch the web server
void WiFi_SetupMode(void);                                              // Setup the AP+STA mode  

// Web server 
void WiFi_CreatWebServer(void);                                         // Creat Web server
String HTTP_Content(void);                                              // Http page massege
String WiFi_ScanAP(void);                                               // Scan For near AP 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

