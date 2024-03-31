/* 
 * File:   setup.h
 * Author: MAAM
 *
 * Created on October 1, 2020, 11:20 AM
 */

#ifndef SETUP_H
#define	SETUP_H

#include "data_type.h"

#define UART_TOGGLE     'T'                     // UART CHAR TO TOGGLE THE RELAY

////////////////////////* Function declaration *////////////////////////////////
void    Init_Setup   (void);                    // INITIALIZE SETUP
void    Toggle_Relay (void);                    // TOGGLE THE RELAY
uint8_t Button_Press (void);                    // BUTTON PRESS STATE
////////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus
extern "C" {
#endif
#ifdef	__cplusplus
}
#endif
#endif	/* SETUP_H */

