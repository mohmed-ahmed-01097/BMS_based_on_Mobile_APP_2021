
#ifndef DATA_TYPE_H
#define  DATA_TYPE_H

#include "Arduino.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_TIMER             5                                         // MAX Number of timer  
#define START_DATE            1900                                      // Year Start 
#define MilliSECOND_TICK      200                                       // 200mSec timer interupt

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
  bool Toggle_F = false;                                                // toggle flag 
  bool D_State  = false;                                                // Device Stat
}S_Switch;


typedef struct { 
  S_Time Now;                                                           // Time of Now
  S_Date Date;                                                          // Date of Today
  S_Timer Timer[MAX_TIMER];                                             // Timers
  S_Switch Switch;                                                      // Switch
}S_MyData;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

