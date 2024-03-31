/* header files */
#include "setup.h"

/* Functions Definition *///////////////////////////////////////////////////////

// SETUP INITIALIZATION
void Init_Setup(void){
    //INT_PIN = 1;                    // PF_INT RA2 as input
    //CURRENT_F = 1;                  // CURRENR_F RC0 as input
    //BUTTON = 1;                     // BUTTON RC1 as input
    //LED = 0;                        // LED RC2 as input
    //RELAY = 0;                      // RELAY RC3 as input
    TRISA  = 0x07; 
    TRISC  = 0x23; 
    
    // TIMER0 INITIALIZATION 
    OPTION_REGbits.T0CS = 0;        // INTERNAL CLOCK CYCLE 
    OPTION_REGbits.PSA  = 0;        // TIMER0 PRESCALER MODULE
    OPTION_REGbits.PS0  = 0;        // 1:8
    OPTION_REGbits.PS1  = 1;
    OPTION_REGbits.PS2  = 0;
    INTCONbits.T0IE = 1;            // TIMER0 INTERRUPT ENABLE
    TMR0 = 6;
    
    // TIMER1 INITIALIZATION
    T1CONbits.TMR1CS  = 0;          // INTERNAL CLOCK CYCLE
    T1CONbits.T1CKPS0 = 0;          // 1:4
    T1CONbits.T1CKPS1 = 1;
    T1CONbits.TMR1ON  = 0;
    
    // INT INTERRUPT INITIALIZATION
    //ANSEL &= ~0x04;                 // INT I/O PIN IS CONFIGURED AS DIGITAL
    OPTION_REGbits.INTEDG = 1;      // RISING EDGE 
    OPTION_REGbits.nRAPU  = 0;      // ENABLE PULL UP RES
    INTCONbits.INTE = 1;            // INT  ENABLE 
    INTCONbits.GIE  = 1;
    INTCONbits.PEIE = 1;
    
    // initial 
    PORTC = 0x04;                   // SET FRIST STATE OF THE RELAY
}

// TOGGLE RELAY STATE
void Toggle_Relay(void){
    static uint8_t read = 0;        // STATIC READ 
    read ^= 1;
    if(read)     PORTC = 0x08;      // SET RELAY
    else         PORTC = 0x04;      // RESET RELAY 
}

// BUTTON PRESS 
uint8_t Button_Press(void){
    static uint8_t switch_state = 0;// STATIC SWITCH STATE
    uint8_t input = (PORTC & 0x02); // BUTTON VALUE
    if((switch_state==0)&&(input)){ // CHECK TO TOGGLE THE RELAY
        __delay_ms(5);              // DELAY BUTTON DEBOUNCING 
        switch_state = 1;           // SET SWITCH STATE 
        return UART_TOGGLE;         // RETURN UART TOGGLE CHAR 
    }
    if(input == 0)
        switch_state = 0;           // RESET SWITCH STATE
        return 0;
}