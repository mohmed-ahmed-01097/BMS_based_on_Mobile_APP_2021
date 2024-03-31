
#ifndef PROCESS_H
#define PROCESS_H

#include "data_type.h"                                                  // DataType library

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialization of Process
void Process_Init(void);

// Relay
void ICACHE_RAM_ATTR Toggle_Relay(S_Switch *_s);                                          // Toggle the relay State if there is a Flage
void ICACHE_RAM_ATTR Timer_Check(S_Timer *_timer,S_Time *_now, S_Switch *_s);             // Timer Check

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

