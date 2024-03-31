
// FOR ESP8266-01 v2.4.1
// SELECT GENERIC ESP8266
// SELECT 512K (64K SPIFFS)
// SELECT FLASH MODE =QIO
// TX -->TX"ESP32"
// RX -->RX"ESP32"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "data_type.h"                                                  // DataType library
#include "Process.h"                                                    // Process library
#include "EEPROM.h"                                                     // EEPROM library
#include "Time.h"                                                       // Timer library
#include "WiFi.h"                                                       // WiFi library
#include "Firebase.h"                                                   // Firebase library
#include <Ticker.h>                                                     // Interrupt library

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Attach_ISR(void);
void Detach_ISR(void);

// INTERRUPT
void ICACHE_RAM_ATTR Timer_ISR(void);
void ICACHE_RAM_ATTR Button_PRESS(void);
void ICACHE_RAM_ATTR UART_ISR(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const String ChipID = "MAAM_SP1_" + String( ESP.getChipId() );          //returns the ESP8266 chip ID as a 32-bit integer.

volatile bool UPDATE_F    = false;
volatile bool READ_F      = false;

S_MyData MyData;
Ticker Interrupt;                                                       // Timer Interrupt
String msg = "";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup(void) {

  ESP.wdtDisable();
  Set_Data_Ptr(&MyData);
  Process_Init();                                                       // Process Initialization
  EEPROM_Init(&MyData);                                                 // Initialasing EEPROM                                                        // Process Initialization
  Monitor_Init();
  Attach_ISR();
  WiFi_Init();                                                          // Connect to the network
  while (WiFi.status() != WL_CONNECTED);
  Time_Init(&MyData.Now, &MyData.Date);                                 // time intialization
  UPDATE_F = true;
  Firebase_Init(ChipID, &MyData);                                       // Firebase initilize
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop(void) {

  uint32_t last = millis();
  while(1){
    yield();
    if ((millis() - last) >= 1000 || (last > millis() &&(last - millis()) >= 1000)){
      Serial.println(MyData.Sec);
      last = millis();
    }                                                   
    if (WiFi.status() == WL_CONNECTED) {                                        // Test the connection
      if(UPDATE_F && WiFi.status() == WL_CONNECTED){
        Time_GetSec (&MyData.Sec);
        Time_GetTime(&MyData.Now);                                              // Get the Time
        Time_GetDate(&MyData.Date);                                             // Get the Date
        UPDATE_F = false;
      }
      if (WiFi.status() == WL_CONNECTED) {
        Firebase_GetPhone(ChipID + "/Data/Phone"  , &MyData.Switch.Toggle_F);
        Firebase_SetRelay(ChipID + "/Data"        , &MyData.Switch.D_State);
        Firebase_SetState(ChipID + "/Data"         );
        Firebase_SetMeas (ChipID + "/Data/Measure", &MyData.Measure);
      }
      if(READ_F && WiFi.status() == WL_CONNECTED){     // >2.5sec at normal // worst case 8.512 sec
        Firebase_GetTimer(ChipID + "/Data/Timer",  MyData.Timer, &MyData.Now);
        
        Firebase_SetPower_H(ChipID + "/Monitor/Hours" , MyData.Now.Hr);
        Firebase_SetPower_D(ChipID + "/Monitor/Days"  , MyData.Date.DD);
        Firebase_SetPower_M(ChipID + "/Monitor/Months", MyData.Date.MM);
        READ_F = false;
      }/*
      if(MONITOR_H_F && WiFi.status() == WL_CONNECTED){ 
        Firebase_SetPower_H(ChipID + "/Monitor/Hours", MyData.Now.Hr);
        MONITOR_H_F = false;
      }
      if(MONITOR_D_F && WiFi.status() == WL_CONNECTED){    
        Firebase_SetPower_D(ChipID + "/Monitor/Days", MyData.Date.DD);
        MONITOR_D_F = false;
      }
      if(MONITOR_M_F && WiFi.status() == WL_CONNECTED){ 
        Firebase_SetPower_M(ChipID + "/Monitor/Months", MyData.Date.MM);
        MONITOR_M_F = false;
      }*/
      
    }else{
      WiFi_Check();                                                             // Check the connection
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// >0.159msec
void Timer_ISR(void) {

  Serial.print  (micros()); Serial.print  ( "\tISR_S");
  static bool syn = true;
  static uint8_t i = 0;
  
  
  Toggle_Relay(&MyData.Switch);
  
  while (Serial.available()) {
    char inChar = Serial.read();
    msg += inChar;
  }
  
    
  if (++i >= SEC_TICK) {
    i = 0;
    Timer_Check(MyData.Timer, &MyData.Now, &MyData.Switch);
    
    Serial_SetData(msg, &MyData.Measure, &MyData.Switch);
    msg = "";
    Calc_Power(&MyData.Power, &MyData.Measure, &MyData.Energy);
    Monitor_Calc(&MyData);
    
    if(++MyData.Sec >= MAX_MINUTES){
      MyData.Sec = 0;
      UPDATE_F = true;
      if(++MyData.Now.Min >= MAX_MINUTES){
        MyData.Now.Min = 0;
        //MyData.Energy = 0;
        //MONITOR_D_F = true;
        if(++MyData.Now.Hr >= MAX_HOURS){
          MyData.Now.Hr = 0;
          //MONITOR_M_F = true;
          if(++MyData.Date.DD >= MAX_DAYS){
            MyData.Date.DD = 1;
            if(++MyData.Date.MM >= MAX_MONTHS){
              MyData.Date.MM = 1;
              ++MyData.Date.YY;
            }
          }
        }
      }
      syn ^= 1;
      Detach_ISR();
      Serial.print  (micros()); Serial.println();
      EEPROM_Write_Time (&MyData.Now);
      EEPROM_Write_Date (&MyData.Date);
      EEPROM_Write_Timer( MyData.Timer);
      Monitor_UpDate(&MyData.Now, &MyData.Date); 
      Serial.print  (micros()); Serial.println();
      Attach_ISR();
      //++MyData.Sec;
      if(syn) 
        ++i;
    }
    
  }
  if(MyData.Sec%30==0){
      READ_F   = true;
      //MONITOR_H_F = true; 
  }
  Serial.print  (micros()); Serial.println( "\tISR_E");

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ICACHE_RAM_ATTR Button_PRESS(void) {

  detachInterrupt(BUTTON);
  uint32_t last = millis();
  while ((millis() - last) >= DEBOUNCING_TICK || (last > millis() &&(last - millis()) >= DEBOUNCING_TICK));
  Serial.println("<<<<Press>>>>");
  MyData.Switch.Toggle_F = true;
  attachInterrupt(BUTTON, Button_PRESS, FALLING);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Attach_ISR(void){
  
  Interrupt.attach_ms (MilliSECOND_TICK , Timer_ISR);                   // Start secondsTick() to set a flag every second.
  attachInterrupt(BUTTON, Button_PRESS, FALLING);
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Detach_ISR(void){
  
  Interrupt.detach();                                                   // disable secondsTick() to set a flag every second.
  detachInterrupt(BUTTON);
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


