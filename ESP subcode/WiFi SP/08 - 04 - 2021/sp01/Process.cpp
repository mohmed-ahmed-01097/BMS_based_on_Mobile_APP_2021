
#include "Process.h"                                                    // Process library
#include "EEPROM.h"                                                     // EEPROM library


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialization of Process
// >1msec
void Process_Init(void) {
  
  Serial.begin(UART_BAUD_RATE, SERIAL_8N1);                   // Initialising if(DEBUG)Serial Monitor
  
  pinMode(BUTTON, INPUT );                                              // Set the Configration of GPIO 2 
  pinMode(RELAY , OUTPUT);                                              // Set the Configration of GPIO 0 
  digitalWrite(RELAY , false);                                          // Set the SP State 
   
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if the button is pressed 
// >1msec
void Button_Check(S_Switch *_s){
   
  static bool Button_f = true;   
  if(!digitalRead(BUTTON) && Button_f) {                                 // check the button
    Button_f = false; 
    _s->Toggle_F = true;
   delayMicroseconds(10000);
  }
  if (digitalRead(BUTTON)){
    Button_f = true;
  }   

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Toggle the relay State if there is a Flage    
// >1msec       
// Relay Toggled __ 1                                      
void Toggle_Relay(S_Switch *_s)  {
     
  if(_s->Toggle_F){                                                      // if there is a Flage
    digitalWrite(RELAY , !digitalRead(RELAY));                           // Toggle the Relay 
    _s->Toggle_F = false;                                                // Release the Flage
    _s->D_State  = (bool)digitalRead(RELAY);                             // Set the State
    Serial.println("Relay Toggled __ " + String(digitalRead(RELAY))); 
  }
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Timer Check
// >1msec
void Timer_Check(S_Timer *_timer,S_Time *_now, S_Switch *_s){

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
        }  
      }
    }
  }
   
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

