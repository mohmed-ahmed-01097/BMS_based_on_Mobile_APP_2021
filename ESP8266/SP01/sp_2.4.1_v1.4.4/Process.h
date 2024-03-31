
#ifndef PROCESS_H
#define PROCESS_H

#include "data_type.h"                                                  // DataType library

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialization of Process
void Process_Init(void);

// Relay
void ICACHE_RAM_ATTR Toggle_Relay(S_Switch *_s);                                          // Toggle the relay State if there is a Flage
void ICACHE_RAM_ATTR Timer_Check(S_Timer *_timer,S_Time *_now, S_Switch *_s);             // Timer Check

// Measure
void Serial_SetData(String _msg, S_Meas *_m, S_Switch *_s);                               // Set the recive Serial data 
void Calc_Power    (S_Power *_p, S_Meas *_m, float *_energy);                             // Calculate the Power 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

