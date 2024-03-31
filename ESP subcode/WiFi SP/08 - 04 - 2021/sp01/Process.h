
#ifndef PROCESS_H
#define PROCESS_H

#include "data_type.h"                                                  // DataType library

// ESP8266 Pinout
#define BUTTON                0                                         // ESP_GPIO0
#define TXD                   1                                         // ESP_TX
#define RELAY                 2                                         // ESP_GPIO2
#define RXD                   3                                         // ESP_RX

// UART Constants
#define UART_BAUD_RATE        9600                                      // ESP Baud rate

// Function Type Define 
// typedef std::function<void(void)> callback_function_t;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialization of Process
void Process_Init(void);

// Relay
void Button_Check(S_Switch *_s);                                          // Check if the button is pressed 
void Toggle_Relay(S_Switch *_s);                                          // Toggle the relay State if there is a Flage
void Timer_Check(S_Timer *_timer,S_Time *_now, S_Switch *_s);             // Timer Check

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

