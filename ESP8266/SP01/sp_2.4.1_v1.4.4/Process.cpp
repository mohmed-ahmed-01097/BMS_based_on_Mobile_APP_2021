
#include "Process.h"                                                    // Process library

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialization of Process
// >1.53msec
void Process_Init(void) {
  
  Serial.begin(UART_BAUD_RATE, SERIAL_8N1);                   // Initialising if(DEBUG)Serial Monitor
  //Serial.print  (micros()); Serial.println( "\tP_S"); 
  pinMode(BUTTON, INPUT );                                              // Set the Configration of GPIO 2 
  pinMode(RELAY , OUTPUT);                                              // Set the Configration of GPIO 0 
  digitalWrite(RELAY , false);                                          // Set the SP State 
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
    digitalWrite(RELAY , _s->D_State);                                    // Toggle the Relay 
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
// Set the recive Serial data
/*
F000.50
V060.00
A001.00
%100.00
@000.00
toInt()
 */
void Serial_SetData(String _msg, S_Meas *_m, S_Switch *_s){

  static uint8_t no_data = 0;
  uint8_t index;
  if(_msg.length()){
    Serial.println("////////////////////");
    index = _msg.indexOf('@');
    if(index > -1){
      _s->D_State  = bool(_msg.substring( index+6,  index+7).toInt());                            // Set the State to Device  
      Serial.println("@ " + String(_s->D_State));
    }
    index = _msg.indexOf('F');
    if(index > -1){
      _m->F  = _msg.substring( index+5,  index+7).toInt();
      Serial.println("F " + String(_m->F));
    }
    index = _msg.indexOf('V');
    if(index > -1){
      _m->V = _msg.substring( index+1,  index+7).toFloat();
      Serial.println("V " + String(_m->V));
    }
    index = _msg.indexOf('A');
    if(index > -1){
      _m->A = _msg.substring( index+1,  index+7).toFloat();
      Serial.println("A " + String(_m->A));
    }
    index = _msg.indexOf('%');
    if(index > -1){
      _m->PF = _msg.substring( index+1,  index+7).toFloat()/100;
      if(_m->PF == 0) _m->PF = 1.0;
      Serial.println("% " + String(_m->PF));
    }
    Serial.println("////////////////////");
    no_data = 0;
  }else{
    if(++no_data <= 200)
      return;
    _m->V  = 0;
    _m->A  = 0;
    _m->PF = 1.0;
    _m->F  = 0;
    no_data = 0;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calculate the Power 
void Calc_Power(S_Power *_p, S_Meas *_m, float *_energy)  {
  
  // Calc the Power
  _p->WATT = float(_m->V * _m->A *               _m->PF );
  _p->VAR  = float(_m->V * _m->A * (1 - _m->PF * _m->PF));  
  *_energy+= (_p->WATT/60);
  Serial.println(*_energy);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

