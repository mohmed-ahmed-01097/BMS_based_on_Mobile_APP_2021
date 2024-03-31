
#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include "Arduino.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ESP8266 Pinout
#define BUTTON                0                                         // ESP_GPIO0
#define RELAY                 2                                         // ESP_GPIO2
#define TXD                   1                                         // ESP_TX
#define RXD                   3                                         // ESP_RX

#define LED_ON                false                                     // ESP_TX
#define LED_OFF               true                                      // ESP_RX

// UART Constants
#define UART_BAUD_RATE        9600                                      // ESP Baud rate

#define MAX_MINUTES           60
#define MAX_HOURS             24
#define MAX_DAYS              32
#define MAX_MONTHS            13

#define MEAS_F                'F'
#define MEAS_V                'V'
#define MEAS_A                'A'
#define MEAS_PF               '%'
#define MEAS_S                '@'

// Function Type Define 
// typedef std::function<void(void)> callback_function_t;

// Timer
// Set offset time in seconds to adjust for your timezone, for example: 
#define TIME_ZONE_OFFSET      7200                                      // GMT +1 = 3600  // GMT +2 = 7200
#define START_DATE            1900                                      // Year Start 
#define MAX_TIMER             5                                         // MAX Number of timer  

// Time ISR
#define MilliSECOND_TICK      100                                       // 100mSec timer interupt
#define SEC_TICK              10                                        // 
#define DEBOUNCING_TICK       1000                                      // 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
  uint8_t Hr  = 00;                                                     // Hour 
  uint8_t Min = 00;                                                     // Minute
}S_Time;

typedef struct {
  bool State  = true;                                                   // State of the timer
  S_Time Start;                                                         // Start Time
  S_Time End;                                                           // End Time
}S_Timer;

typedef struct {
  uint8_t DD = 01;                                                      // Day
  uint8_t MM = 01;                                                      // Mounth
  uint8_t YY = 31;                                                      // Year
}S_Date;

typedef struct {
  volatile bool Toggle_F = false;                                       // toggle flag 
  volatile bool D_State  = false;                                       // Device Stat
}S_Switch;

typedef struct { 
  uint8_t Sec = 0;
  S_Time Now;                                                           // Time of Now
  S_Date Date;                                                          // Date of Today
  S_Timer Timer[MAX_TIMER];                                             // Timers
  S_Switch Switch;                                                      // Switch
}S_MyData;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

