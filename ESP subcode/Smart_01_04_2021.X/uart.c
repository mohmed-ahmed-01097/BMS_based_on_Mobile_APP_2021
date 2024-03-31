/* header files */
#include "uart.h"

/* Functions Definition *///////////////////////////////////////////////////////

// UART INITIALIZATION 
void UART_Init(void){
    UART_TX = 0;         UART_RX = 1;
    SPBRG = 51;                     //BAUD RATE = 9600
    TXSTAbits.BRGH = 1;
    
    TXSTAbits.SYNC = 0;             // ENABLE ASYNCHRONOUS SERIAL PORT
    RCSTAbits.SPEN = 1;             // ENABLE SERIAL PORT PIN
    TXSTAbits.TXEN = 1;             // ENABLE UART TRANSSMIT 
    RCSTAbits.CREN = 1;             // ENABLE UART RECEPT
    TXSTAbits.TX9  = 0;             // SELSCT 8BIT TRANSMISSION 
    RCSTAbits.RX9  = 0;             // SELECT 8BIT RECEIVING 
    
    PIE1bits.RCIE   = 1;            // ENABLE RC INTERRUPT 
    PIR1bits.RCIF   = 0;            // DISABLE RC INTERRUPT FLAG 
    INTCONbits.GIE  = 1;            // ENABLE GENERAL INTERRUPT
    INTCONbits.PEIE = 1;            // ENABLE PREPHIRAL INTERRUPT 
}

// UART RECECIVE CHAR  
uint8_t UART_GetC(void){
    if (OERR) {                     // IF THERE IS OVER RUN ERROR
        CREN = 0;                   // CLEAR OVER RUN ERROR BIT 
        CREN = 1;                   // SET OVER RUN ERROR BIT 
    }
    return RCREG;                   // RETURN THE UART RC DATA REGISTER
}

// UART SEND CHAR 
void UART_PutC(const int8_t data){
    while(TXIF==0);                 // WAIT TO EMPTY THE TRANSMITTER RIGISTER
    TXIF=0;                         // CLEAR TRANSMITTER FLAG 
    TXREG = data;                   // SEND CHAR TO UART TX DATA REGISTER
}

// UART SEND TEXT 
void UART_Write(const int8_t *data){
    uint8_t i = 0;                  // ITERATION 
    while (data[i] != '\0')         // RECEIVE DATA UNTILL NULL CHAR 
        UART_PutC (data[i++]);      // SEND CHAR 
    UART_PutC('\n');                // SEND NEWLINE
    UART_PutC('\r');                // SEND FRIST LINE
}  

// UART SEND MASSAGES
void UART_Send(uint16_t temp, uint8_t ch){
    int8_t txt [8] = "0000000\0";   // TEXT FORM
    txt[0] = ch;                    // SET FRIST CHAR
    floatTostr(temp, txt);          // CONVERT THE NUMBRE TEXT FORM
    if(ch == '%'){                  // PUT THE PF SIGN
        if(temp>100000)     txt[1] = '-';
        else                txt[1] = '+'; 
    }
    UART_Write(txt);                //SEND THE TEXT
    if(ch == '@'){                  // END LINE AND NEW LINE
        UART_PutC('\n');
        UART_PutC('\r');
    } 
}

// CONVERT INTEGER VALUE INTO STRING
void floatTostr(uint16_t num, int8_t *txt){       
    txt[6] = ((num      )%10)  + '0';
    txt[5] = ((num/10   )%10)  + '0';
    txt[4] = '.';
    txt[3] = ((num/100  )%10)  + '0';
    txt[2] = ((num/1000 )%10)  + '0';
    txt[1] = ((num/10000)%10)  + '0';
}  
