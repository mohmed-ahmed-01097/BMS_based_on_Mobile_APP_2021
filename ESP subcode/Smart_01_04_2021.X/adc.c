/* header files */
#include "adc.h"

/* Functions Definition *///////////////////////////////////////////////////////

// ANALOG DIGITAL CONVERTER INITIALIZATION
void ADC_Init(void){
    CMCON0 = 0x07 ;                 // DISABLE COMPARATORS
    ANSEL  = 0x03;                  // ENABLE AN0 AN1 AS ANALOG 
    ADCON0 = 0x81;                  // TURN ADC ON, SELECT AN0 CHANNEL, Vref = Vdd, LEFT JUSTIFICATION
    ADCON1 = 0x50;                  // ADC CLOCK = Fosc/16
    ADRESH = 0;                     // FLUSH ADC OUTPUT REGISTER HIGH & LOW
    ADRESL = 0;
}

// ANALOG DIGITAL CONVERTER READ DATA
 uint16_t ADC_Read(uint8_t ANC){
   if(ANC<0 || ANC>7)   return 0;   // CHECK CHANNEL NUMBER VALIDIY
   ADCON0 &= ~(7<<2);               // CLEAR THE CHANNEL SELECTION BITS
   ADCON0 |= ANC<<2;                // SELECT THE REQUIRED CHANNEL (ANC)
   __delay_us(20);                  // WAIT THE ACQUISITION TIME (T_ACQ=5us)(T_AD=2us,T_OSC=0.125us,T_AMP=2us) 
   GO_DONE = 1;                     // START A/D CONVERSION(POLLING GO_DONE BIT)
   while(ADCON0bits.GO_DONE);       // PROVIDES DELAY UNTILL CONVERSION IS COMPLETE
   return ((ADRESH << 8) + ADRESL); // RETURN THE RIGHT_JUSTIFIED 10-BIT RESULT
}

// READ RMS VALUE OF (V,A) AND THE OFFSET IS 2.5V
void ADC_RMS(uint16_t *ARRAY,uint8_t ANC, uint16_t rate, const uint8_t index){
    uint8_t i;                      // ITIRATION
    uint16_t read;                  // THE ADC READ
    uint16_t max = 0;               // RESET MAX VALUE
    uint16_t min = 1023;            // RESET MIN VALUE
    for(i=0;i<ITERATION;i++){       // REPEAT READING TO GET 
        read = ADC_Read(ANC);       // READ THE ADC  
        if(read > max)  max = read; // SAVE MIN AND MAX READ
        if(read < min)  min = read; 
    }   
    ARRAY[index] = (uint16_t)(((float)((max-min)/2)*rate)/10);   // SAVE THE VALUE IN THE ARRAY             
}

// READ THE POWER FACTOR VALUE 
void ADC_PF (uint16_t *ARRAY, const uint8_t index){
    float step = 1;                 // STEP VALUE FOR CALCULAT COS(@)
    float temp = 1;                 // TEMP VALUE FOR CALCULAT COS(@)
    float rad_angle;                // RAD ANGLE VALUE FOR CALCULAT COS(@)
    uint8_t i;                      // ITIRATION
    uint16_t angle;
    
    TMR1H = 0;    TMR1L = 0;        // RESET TIMER1 VALUE REGISTER
    PF_FLAG = 0;                    // RESET PF FLAG UNTILL VOLT ZERO CROSS INTERRUPT
    while(!PF_FLAG);                // WAIT UNTILL THE INT
    
    T1CONbits.TMR1ON = 1;           // ENABLE TIMER1
    //while(PORTC&0x01);              // WAIT UNTILL START
    PF_FLAG = 0;                    // RESET PF_FLAG
    while(PORTC&0x01)               // CHECK CURRENT ZERO CROSS
        if(PF_FLAG)         break;
    T1CONbits.TMR1ON = 0;           // DISABLE TIMER1
                                    // STORE THE TIMER DIFFERANCE
    
    rad_angle = ((float)((uint16_t)(TMR1H<<8)+TMR1L) * F_VALUE) * AN_RATE;
    angle     = (uint16_t)rad_angle;
    
    switch((uint8_t)(angle/(PF_RATE/2))){
        case 0:
            PF_FLAG = 1;
            break;
        case 1:
            PF_FLAG = 0;
            angle = PF_RATE - angle;
            break;
        case 2:
            PF_FLAG = 0;
            angle = angle - PF_RATE;
            break;
        case 3:
            PF_FLAG = 1;
            angle = (PF_RATE*2) - angle;            
            break;
        default:
            PF_FLAG = 1;
            break;
    }
    
        
    if(angle<(PF_RATE/4)){
        rad_angle  /= 10000.0;
        rad_angle *= rad_angle;
        for(i=1;i<=5;i++){              // CALCULATE THE COS
            step *= ((-1*rad_angle)/(2*i*(2*i-1)));
            temp += step;             
            // COS = 1-STEP(X^2/2!)+STEP(X^4/4!)-STEP(X^6/6!)+STEP(X^8/8!)
        }
    }else{
        rad_angle = ((float)((PF_RATE/2)- rad_angle)/10000.0);
        temp = rad_angle;
        step = rad_angle;
        rad_angle *= rad_angle;
        for(i=1;i<=5;i++){              // CALCULATE THE SIN
            step *= ((-1*rad_angle)/(2*i*(2*i+1)));
            temp += step;             
            // SIN = X-STEP(X^3/3!)+STEP(X^5/5!)-STEP(X^7/7!)+STEP(X^9/9!)
        }
    }
    
    ARRAY[index]  = (uint16_t)(temp*10000);     // STORE THE ARRAY VALUE
    if(!index)
        PF_SIGN = 0;
    PF_SIGN += (temp >= 0 ? 0 : 1);        // CHECK THE SIGN OF THE PF
    
             
}

// CALCULATE THE AVRAGE VALUE 
uint16_t AVG(uint16_t *ARRAY, const uint8_t index){
    uint8_t i;                      // ITERATION
    uint8_t avg = 0;                // AVERAGE VALUE
    uint32_t ret = 0;               // RETURN VALUE
    for(i=1;i<index;i++){
        if(ARRAY[i]>0){
            ret += ARRAY[i];        // SUM OF VALUES
            avg++;                  // INCREAS AVRAGE NUMBER
        } 
    }
    return ((uint16_t)(ret / avg));  // RETURN THE AVRAGE VALUE
}

