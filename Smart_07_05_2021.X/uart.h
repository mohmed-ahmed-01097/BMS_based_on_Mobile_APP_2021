/* 
 * File:   uart.h
 * Author: MAAM
 *
 * Created on October 1, 2020, 11:22 AM
 */

#ifndef UART_H
#define	UART_H

#include "data_type.h"

//#define UART_TOGGLE     'T'             // UART CHAR TO TOGGLE THE RELAY
#define UART_SEND       'S'             // UART CHAR TO SEND VALUES
#define UART_MEASURE    'M'             // UART CHAR TO SEND THE AVG POWER THROW LAST MINUTE
#define UART_PLOT       'P'             // UART CHAR TO SEND THE POWER VALUES THROW LAST MINUTE

#define UART_TX     TRISCbits.TRISC4    // TX PIN TRISC4 
#define UART_RX     TRISCbits.TRISC5    // RX PIN TRISC4

////////////////////////* Function declaration *////////////////////////////////
void    UART_Init (void);                           // INITIALIZE THE UART
uint8_t UART_GetC (void);                           // RECEIVE CHAR  
void    UART_PutC (const int8_t data);              // SEND CHAR
void    UART_Write(const int8_t *data);             // UART WRITE TEXT 
//void    UART_Read (uint8_t *data , uint8_t Length); // UART READ TEXT 
void    UART_Send (uint16_t temp, uint8_t ch);      // UART SEND MASSAGES  
void    floatTostr(uint16_t num, int8_t *txt);      // CONVERT NUMBER TO TEXT
////////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus
extern "C" {
#endif
#ifdef	__cplusplus
}
#endif
#endif	/* UART_H */

