/* 
 * File:   adc.h
 * Author: MAAM
 *
 * Created on October 1, 2020, 11:18 AM
 */

#ifndef ADC_H
#define	ADC_H

#include "data_type.h"

#define V_RATE             767          // ACCURACY = 0.84 V 8250  7667    
#define A_RATE             78           // ACCURACY = 0.03 A 0525  0520  
#define PF_RATE            31416        // (2*pi)^2 = (6.285714)^2 = 39.51  
#define AN_RATE            0.15708      // ANGLE RATE   0.121
#define V_AN               0            // ADC CURRENT CHANNEL 
#define A_AN               1            // ADC CURRENT CHANNEL 
#define INDEX              4            // THE ARRAY LENGTH 
#define ITERATION          256          // NUMBER OF MEASUREMENT FOR EACH RMS _MEASUR

////////////////////////////////////////////////////////////////////////////////

uint16_t VOLT_ARRAY[INDEX];             // VOLT ARRAY TO STORE MEASURE FOR ONE SEC
uint16_t CURRENT_ARRAY[INDEX];          // CURRENT ARRAY ~~
uint16_t POWER_FACTOR_ARRAY[INDEX];     // POWER FACTOR ARRAY ~~
uint16_t FREQUENCY_ARRAY[INDEX];        // FREQUENCY ARRAY ~~
uint8_t  PF_FLAG = 0;                   // PF FLAG TO MEASURE THE PF

uint8_t  PF_SIGN;                       // power factor sign
uint16_t V_VALUE, A_VALUE, PF_VALUE, F_VALUE = 50;

////////////////////////* Function declaration *////////////////////////////////
void     ADC_Init(void);                                // INITIALIZE ADC
uint16_t ADC_Read(uint8_t ANC);                         // READ ADC VALUE
void     ADC_RMS(uint16_t *ARRAY, uint8_t ANC, 
                 uint16_t   rate, const uint8_t index); // READ RMS 
void     ADC_PF (uint16_t *ARRAY, const uint8_t index); // READ PF
uint16_t AVG    (uint16_t *ARRAY, const uint8_t index); // AVRAGE THE ARRAY
////////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus
extern "C" {
#endif
#ifdef	__cplusplus
}
#endif
#endif	/* ADC_H */

