/* 
 * File:   main.c
 * Author: MAAM
 *
 * Created on September 29, 2020, 5:18 PM
 */

/* Header Files *///////////////////////////////////////////////////////////////
#include "config.h"
#include "data_type.h"
#include "setup.h"
#include "adc.h"
#include "uart.h"

/* Functions Declaration *//////////////////////////////////////////////////////
void Init    (void);                        // INITIALIZR THE MODULES
void Send    (void);                        // SEND MEASURING VALUES  
void Measure (void);                        // MEASURE V, A, PF VALUES 

/* Global Variables *///////////////////////////////////////////////////////////
uint16_t TIME_MS = 0;                       // TIME FOR MILLI SECOND  
uint8_t  TIME_S  = 0;                       // TIME FOR SECOND
uint8_t ZERO_CROSS  = 0;                    // ZERO CROSS PULSE
uint8_t  CHAR_STATE  = 0;                   // UART CHAR_STATE

/* Main Function *//////////////////////////////////////////////////////////////
void main(void) {
    Init();                                 // INITIALIZR THE MODULES
    while(1){
        CHAR_STATE = Button_Press();        // BUTTON PRESS STATE 
        if(TIME_MS%200==0)                  // EACH 200ms  
            Measure();                      // MEASURE THE VALUES 
        if(CHAR_STATE != 0)                 // UART CHAR STATE 
            Send();                         // SEND VALUES
    }
}

/* interrupt service routine *//////////////////////////////////////////////////
void __interrupt() ISR(void){
    if(INTCONbits.INTF){                    // CHECK INT INTERRUPT FLAG
        PF_FLAG = 1;                        // SET PF FLAG 
        ++ZERO_CROSS;                       // INCREASE ZERO CROSS 
        INTCONbits.INTF = 0;                // RESET INT INTERRUPT FLAG
    }
    if(INTCONbits.T0IF){                    // CHECK RC INTERRUPT FLAG
        TMR0 = 6;
        if((++TIME_MS)>=1000){              // CHECK TIME_MS 
            TIME_MS = 0;                    // RESET TIME_MS
            if((++TIME_S)>60){              // CHECK TIME_S
                TIME_S = 0;                 // RESET TIME_S   
            }
            CHAR_STATE = UART_SEND;
            ZERO_CROSS = 0;                 // RESET ZERO CROSS
        }
        INTCONbits.T0IF = 0;                // RESET TIMER0 INTERRUPT FLAG
    }
    if(PIR1bits.RCIF){                      // CHECK RC INTERRUPT FLAG
        CHAR_STATE = UART_GetC();           // RECEIVE CHAR TO CHAR STATE
        PIR1bits.RCIF = 0;                  // RESET RC INTERRUPT FLAG
    }
}

/* Functions Definition *///////////////////////////////////////////////////////

// INITIALIZR THE MODULES 
void Init(void){ 
    ADC_Init();                             // INITIALIZE ADC
    UART_Init();                            // INITIALIZE UART
    UART_Write("WELCOME MAAM");             // WELCOME MASSAGE 
    Init_Setup();                           // INITIALIZE SETUP 
}

// SEND MEASURING VALUES
void Send(void){
    uint8_t i;                                      // ITERATION
    switch(CHAR_STATE){                             // UART STATE CHAR 
        case UART_TOGGLE:
            Toggle_Relay();                         // TOGGLE THE RELAY STATE
            UART_Send(((PORTC&0X08)>>3),'@');       // UART SEND
            CHAR_STATE = 0;                         // RESET THE UART STATE CHAR
            break;                            
        case UART_SEND:
            UART_Send(F_VALUE           ,'F');      // UART SEND F
            UART_Send(V_VALUE           ,'V');      // UART SEND V
            UART_Send(A_VALUE           ,'A');      // UART SEND A
            UART_Send(PF_VALUE          ,'%');      // UART SEND PF
            UART_Send(((PORTC&0X08)>>3) ,'@');      // UART SEND RELAY
            CHAR_STATE = 0;                         // RESET THE UART STATE CHAR
            break;
        case UART_MEASURE:
            UART_Send((uint16_t)((float)(V_VALUE*A_VALUE*PF_VALUE)/100000000.0) ,'P');                // UART SEND AVRAGE POWER
            CHAR_STATE = 0;                         // RESET THE UART STATE CHAR
            break;
    }
}

// MEASURE V, A, PF VALUES 
void Measure(void){
    static uint8_t in = 0;                          // STATIC INDEX
    ADC_RMS(VOLT_ARRAY   , V_AN, V_RATE, in);       // MEASURE V RMS
    ADC_RMS(CURRENT_ARRAY, A_AN, A_RATE, in);       // MEASURE A RMS
    if(ZERO_CROSS){                                 // CHECK FREQUENCY NOT ZERO
        ADC_PF(POWER_FACTOR_ARRAY,       in);       // MEASURE PF
    }
    if(++in >= INDEX){
        in = 0;                                     // SET THE INDEX VALUE
        F_VALUE  = AVG(FREQUENCY_ARRAY   , INDEX);  // F VALUE
        V_VALUE  = AVG(VOLT_ARRAY        , INDEX);  // V VALUE
        A_VALUE  = AVG(CURRENT_ARRAY     , INDEX);  // A VALUE
        PF_VALUE = AVG(POWER_FACTOR_ARRAY, INDEX);  // PF VALUE  
        PF_VALUE += PF_SIGN > (INDEX/2) ? 0:100000;
    } 
}
