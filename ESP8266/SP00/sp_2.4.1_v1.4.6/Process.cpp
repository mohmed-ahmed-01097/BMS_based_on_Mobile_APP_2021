
#include "Process.h"                                                    // Process library

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialization of Process
// >1.53msec
void Process_Init(void) {
  
  Serial.begin(UART_BAUD_RATE, SERIAL_8N1);                   // Initialising if(DEBUG)Serial Monitor
  //Serial.print  (micros()); Serial.println( "\tP_S"); 
  pinMode(BUTTON, INPUT );                                              // Set the Configration of GPIO 2 
  pinMode(RELAY , OUTPUT);                                              // Set the Configration of GPIO 0 
  digitalWrite(RELAY , true);                                          // Set the SP State 
  //Serial.print  (micros()); Serial.println( "\tP_E"); 
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Toggle the relay State if there is a Flage    
// >0.179msec       
// Relay Toggled __ 1                                      
void ICACHE_RAM_ATTR Toggle_Relay(S_Switch *_s)  {
     
  //Serial.print  (micros()); Serial.println( "\tPR_S");
  if(_s->Toggle_F){                                                       // if there is a Flage
    _s->Toggle_F  = false;                                                // Release the Flage
    _s->D_State  ^= 1;                                                    // Set the State
    digitalWrite(RELAY , ! _s->D_State);                                    // Toggle the Relay 
    Serial.println("T");
    //Serial.println("Relay Toggled __ " + String(digitalRead(RELAY))); 
  }
  //Serial.print  (micros()); Serial.println( "\tPR_E");
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Timer Check
// >0.046msec / 0.1msec
void ICACHE_RAM_ATTR Timer_Check(S_Timer *_timer,S_Time *_now, S_Switch *_s){

  //Serial.print  (micros()); Serial.println( "\tPT_S"); 
  for(uint8_t i = 0; i < MAX_TIMER ; i++){                              // Timer for loop
    if(_timer[i].State){                                                 // if the timer state if high
      // Check the Start of the Timer 
      if(_timer[i].Start.Hr == _now->Hr && _timer[i].Start.Min == _now->Min){
        if(!_s->D_State){                                                // if the State is Low
          _s->Toggle_F = true;                                           // Rise the Flage
        }
      }
      // Check the End of the Timer 
      if(_timer[i].End.Hr == _now->Hr && _timer[i].End.Min == _now->Min){
        if(_s->D_State){                                                 // if the State is High
          _s->Toggle_F = true;                                           // Rise the Flage
          _timer[i].State = false;
        }  
      }
    }
  }
  //Serial.print  (micros()); Serial.println( "\tPT_E"); 
   
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

