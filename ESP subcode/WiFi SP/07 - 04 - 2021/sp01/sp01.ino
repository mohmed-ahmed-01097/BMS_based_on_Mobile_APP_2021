/*
 * esptool.py v2.8
 * Serial port COM6
 * Connecting.......
 * Chip is ESP8266EX
 * Features: WiFi
 * Crystal is 26MHz
 * MAC: cc:50:e3:20:2c:70
 * Uploading stub...
 * Running stub...
 * Stub running...
 * Configuring flash size...
 * Auto-detected Flash size: 1MB
 * Compressed 398864 bytes to 284352...
Serial port COM6
Connecting....
Chip is ESP8266EX
Features: WiFi
Crystal is 26MHz
MAC: cc:50:e3:20:2c:70
Uploading stub...
Running stub...
Stub running...
Configuring flash size...
Auto-detected Flash size: 1MB
Compressed 394912 bytes to 282753...
 */
// FOR ESP8266-01 v2.6.1
// SELECT GENERIC ESP8266
// SELECT 1M (64K SPIFFS)
// SELECT FLASH MODE = DOUT || QIO
// TX -->TX"ESP32"
// RX -->RX"ESP32"


#include "data_type.h"                                                  // DataType library
#include "Process.h"                                                    // Process library
#include "EEPROM.h"                                                     // EEPROM library
#include "Time.h"                                                       // Timer library
#include "WiFi.h"                                                       // WiFi library
#include "Firebase.h"                                                   // Firebase library
#include <Ticker.h>                                                     // Interrupt library

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Timer_ISR(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const String ChipID = "MAAM_SP1_" + String( ESP.getChipId() );          //returns the ESP8266 chip ID as a 32-bit integer.

S_MyData MyData;
Ticker Interrupt;                                                       // Timer Interrupt 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup(void) {  
  ESP.wdtDisable();
  Process_Init();                                                       // Process Initialization
  EEPROM_Init(&MyData);                                                 // Initialasing EEPROM                                                        // Process Initialization
  Interrupt.attach_ms (MilliSECOND_TICK , Timer_ISR);                   // Start secondsTick() to set a flag every second. 
  
  WiFi_Init();                                                          // Connect to the network
  while(WiFi.status() != WL_CONNECTED);
  Time_Init(&MyData.Now, &MyData.Date);                                 // time intialization 
  Firebase_Init(ChipID, &MyData);                                       // Firebase initilize
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop(void) {
  
   Serial.print  (millis()); Serial.println( "\tCCCC_S");
  WiFi_Check();                                                       // Check the connection 
  while(WiFi.status() == WL_CONNECTED) {                              // Test the connection
    Firebase_SetData(ChipID,&MyData);                                 // check if any data changes to resend it to the databasse
    Firebase_GetData(ChipID,&MyData);                                 // check if there are change in the data and read this data
  }
   Serial.print  (millis()); Serial.println( "\tCCCC_E");

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// >1msec
void Timer_ISR(void){

  static uint8_t i=0;
  i++;
  if(i>=5){
    Time_UpDate(&MyData.Now, &MyData.Date, WiFi.status() == WL_CONNECTED);
    Timer_Check(MyData.Timer, &MyData.Now, &MyData.Switch);
    i = 0;
  }
  Button_Check(&MyData.Switch);
  Toggle_Relay(&MyData.Switch);
 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

