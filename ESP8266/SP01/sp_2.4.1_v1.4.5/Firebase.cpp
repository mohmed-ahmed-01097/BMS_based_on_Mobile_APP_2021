
#include "Firebase.h"                                                   // firebase library
#include "EEPROM.h"                                                     // eeprom library
#include <FirebaseArduino.h>                                            // firebase library
S_Monitor Monitor;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// initialization firebase
// >1.045sec Normal 
// >6.5 sec Start
void Firebase_Init (const String _path, const S_MyData *_data){  
  
  Serial.print  (micros()); Serial.println( "\tFB_S"); 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                       // connect to firebase ~~ 220usec
  Serial.print  (micros()); Serial.println( "\tFB_S"); 
  if(Firebase.getString(_path + "/Users") != "MAAM"){
    Firebase.setString(_path + "/Users", "MAAM");
    Firebase.setString(_path + "/Data/Phone", "/");
    for(int i = 0; i < MAX_TIMER; i++){
        Firebase.setBool(_path + "/Data/Timer" + "/T" + String(i) + "/State"    ,_data->Timer[i].State);
        Firebase.setInt (_path + "/Data/Timer" + "/T" + String(i) + "/Start/Min",_data->Timer[i].Start.Min);
        Firebase.setInt (_path + "/Data/Timer" + "/T" + String(i) + "/Start/Hr" ,_data->Timer[i].Start.Hr);  
        Firebase.setInt (_path + "/Data/Timer" + "/T" + String(i) + "/End/Min"  ,_data->Timer[i].End.Min);
        Firebase.setInt (_path + "/Data/Timer" + "/T" + String(i) + "/End/Hr"   ,_data->Timer[i].End.Hr); 
    } 
  }
  Serial.print  (micros()); Serial.println( "\tFM_S");
  //Firebase_SetPower  (_path + "/Monitor");
  Serial.print  (micros()); Serial.println( "\tFB_E"); 
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// check the error
bool Firebase_ErrorCheck (const String _path){                            
  
  if (Firebase.failed()) {                                              //Handle error 
     Serial.print(_path + "_____________failed: "); 
     Serial.println(Firebase.error());  
     return true; 
  } 
  return false;
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set relay state
// >220msec Relay
void Firebase_SetRelay(const String _path, const volatile bool *_state){  
  
  static bool change = !*_state;
  Serial.print  (micros()); Serial.println( "\tFBSR_S"); 
  if(change != *_state){
    change = *_state;
    Firebase.setBool  (_path + "/RELAY" , *_state);
    if(Firebase_ErrorCheck(_path + "/RELAY")) {
      return;
    }
  }
  Serial.print  (micros()); Serial.println( "\tFBSR_E"); 
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set relay state
// >220msec State
void Firebase_SetState(const String _path){  
  
  Serial.print  (micros()); Serial.println( "\tFBSS_S"); 
  Firebase.setBool  (_path + "/State" , true);
  if(Firebase_ErrorCheck(_path + "/State")) {
    return;
  }
  Serial.print  (micros()); Serial.println( "\tFBSS_E"); 
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set measure to firebase
void Firebase_SetMeas(const String _path, const S_Meas *_measure){                   // set measure to firebase 
  
  static S_Meas meas;
  
  if(_measure->F != meas.F){
    Firebase.setInt   (_path + "/01F" , _measure->F);
    if(Firebase_ErrorCheck(_path + "/01F")) {
      return;
    }
    meas.F = Firebase.getInt   (_path + "/01F");
  }
  
  if(_measure->V != meas.V){
    Firebase.setFloat (_path + "/02V" , _measure->V);
    if(Firebase_ErrorCheck(_path + "/02V")) {
      return;
    }
    meas.V = Firebase.getFloat (_path + "/02V");
  }
  
  if(_measure->A != meas.A){
    Firebase.setFloat (_path + "/03A" , _measure->A);
    if(Firebase_ErrorCheck(_path + "/03A")) {
      return;
    }
    meas.A = Firebase.getFloat (_path + "/03A");
  }
  
  if(_measure->PF != meas.PF){
    Firebase.setFloat (_path + "/04PF" , _measure->PF);
    if(Firebase_ErrorCheck(_path + "/04PF")) {
      return;
    }
    meas.PF = Firebase.getFloat(_path + "/04PF"); 
  }
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Firebase_SetPower  (const String _path){            

  for(uint8_t i=0 ; i<24 ; i++){
    Firebase_SetPower_H(_path + "/Hours" ,i+1);
  }
  for(uint8_t i=0 ; i<31 ; i++){
    Firebase_SetPower_D(_path + "/Days"  ,i+1);
  }
  for(uint8_t i=0 ; i<12 ; i++){
    Firebase_SetPower_M(_path + "/Months",i+1);
  }
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Firebase_SetPower_H(const String _path, const uint8_t _h){
  
  static uint8_t last_h = 0xFF;  
  if(last_h != _h){
    Firebase.setFloat(_path + "/"  + String(_h), Monitor.H[_h - 1]);
    last_h = _h;
  }
  Firebase.setFloat(_path + "/"  + String(_h), Monitor.H[_h]);
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Firebase_SetPower_D(const String _path, const uint8_t _d){
  
  static uint8_t last_d = 0xFF;  
  if(last_d != _d){
    Firebase.setFloat(_path + "/"  + String(_d), Monitor.D[_d - 2]);
    last_d = _d;
  }
  Firebase.setFloat(_path + "/"  + String(_d), Monitor.D[_d - 1]);
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Firebase_SetPower_M(const String _path, const uint8_t _m){
  
  static uint8_t last_m = 0xFF;  
  if(last_m != _m){
    Firebase.setFloat(_path + "/"  + String(_m), Monitor.M[_m - 2]);
    last_m = _m;
  }
  Firebase.setFloat(_path + "/"  + String(_m), Monitor.M[_m - 1]);
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// get phone massage
// >3090msec ~ >376.9msec
void Firebase_GetPhone(const String _path, volatile bool *_flage){

  Serial.print  (micros()); Serial.println( "\tFBGP_S");
  String msg = Firebase.getString(_path);
  Serial.println(msg);
  if(msg == "T"){
    *_flage = true;
  }
  Firebase.setString(_path, "");
  if(Firebase_ErrorCheck(_path)) {
    return;
  }
  Serial.print  (micros()); Serial.println( "\tFBGP_E");

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// get timer
// >1sec ~ 935msec
void Firebase_GetTimer (const String _path, S_Timer *_timer,S_Time *_now){              // get timer

  static uint8_t i = 0;
  Serial.print  (micros()); Serial.println( "\tFBGT_S");
  bool state = Firebase.getBool(_path + "/T" + String(i) + "/State");
  if( state || state != _timer[i].State ){
    _timer[i].Start.Min = Firebase.getInt (_path + "/T" + String(i) + "/Start/Min");
    _timer[i].Start.Hr  = Firebase.getInt (_path + "/T" + String(i) + "/Start/Hr");  
    _timer[i].End.Min   = Firebase.getInt (_path + "/T" + String(i) + "/End/Min");
    _timer[i].End.Hr    = Firebase.getInt (_path + "/T" + String(i) + "/End/Hr");
    if(_timer[i].End.Hr != _now->Hr || _timer[i].End.Min != _now->Min){ 
      _timer[i].State     = state;
    }
  }
  if(++i > MAX_TIMER){
    i = 0;
  }
  Serial.print  (micros()); Serial.println( "\tFBGT_E");
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// >400msec
void Monitor_Init(void){

  Serial.print  (micros()); Serial.println( "\tMP_S$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
  for(uint8_t i=0 ; i<24 ; i++){
    Monitor.H[i] = EEPROM_Monitor_Read_H(i);
  }
  Serial.println();
  for(uint8_t i=0 ; i<31 ; i++){
    Monitor.D[i] = EEPROM_Monitor_Read_D(i);
  }
  Serial.println();
  for(uint8_t i=0 ; i<12 ; i++){
    Monitor.M[i] = EEPROM_Monitor_Read_M(i);
  }
  Serial.print  (micros()); Serial.println( "\tMP_E$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// >835.895msec
void Monitor_UpDate(const S_Time *_now, const S_Date *_today){

  static uint8_t last_h = 0xFF;
  Serial.print  (micros()); Serial.println( "\tMP_S///////////////////////////");
  
  if(last_h != _now->Hr){
    EEPROM_Monitor_Write_H(_now->Hr      , Monitor.H[_now->Hr - 1]);
    last_h = _now->Hr;
  }
  EEPROM_Monitor_Write_H(_now->Hr      , Monitor.H[_now->Hr]);
  Serial.println("/////////////////" + String(Monitor.H[_now->Hr]));
  
  EEPROM_Monitor_Write_D(_today->DD - 1, _now->Hr);
  Monitor.D[_today->DD - 1] = EEPROM_Monitor_Read_D(_today->DD - 1);
  Serial.println("/////////////////" + String(Monitor.D[_today->DD - 1]));
  
  EEPROM_Monitor_Write_M(_today->MM - 1, _today->DD - 1);
  Monitor.M[_today->MM - 1] = EEPROM_Monitor_Read_M(_today->MM - 1);
  Serial.println("/////////////////" + String(Monitor.M[_today->MM - 1]));
  
  Serial.print  (micros()); Serial.println( "\tMP_E///////////////////////////");
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Path Monitor
void Monitor_Calc(S_MyData *_data){

  static float last = 0;
  static uint8_t last_h = 0xFF;
  if(last_h != _data->Now.Hr){
    _data->Energy -= Monitor.H[_data->Now.Hr - 1]; 
    last = 0;    
    last_h = _data->Now.Hr;
  }
  Monitor.H[_data->Now.Hr     ]  =  _data->Energy;
  Monitor.D[_data->Date.DD - 1] += (_data->Energy > last) ? (_data->Energy - last) : 0;
  Monitor.M[_data->Date.MM - 1] += (_data->Energy > last) ? (_data->Energy - last) : 0;
  last = _data->Energy;
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String Monitor_Graph_H(void){
  String ret = "[";
  for(uint8_t i=0 ; i<23 ; i++){
    ret += String(Monitor.H[i]) + ",";
  }
  ret += String(Monitor.H[23]);
  return ret + "]";
}
String Monitor_Graph_D(void){
  String ret = "[";
  for(uint8_t i=0 ; i<29 ; i++){
    ret += String(Monitor.D[i]) + "," ;
  }
  ret += String(Monitor.D[29]);
  return ret + "]";  
}
String Monitor_Graph_M(void){
  String ret = "[";
  for(uint8_t i=0 ; i<11 ; i++){
    ret += String(Monitor.M[i]) + ",";
  }
  ret += String(Monitor.M[11]);
  return ret + "]";
}
