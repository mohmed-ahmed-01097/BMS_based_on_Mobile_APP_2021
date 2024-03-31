
#include "Firebase.h"                                                   // firebase library
#include "EEPROM.h"                                                     // eeprom library
#include <FirebaseArduino.h>                                            // firebase library
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// initialization firebase
// >1.045sec Normal 
// >6.5 sec Start
void Firebase_Init (const String _path, const S_MyData *_data){  
  
  //Serial.print  (micros()); Serial.println( "\tFB_S"); 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                       // connect to firebase ~~ 220usec
  //Serial.print  (micros()); Serial.println( "\tFB_S"); 
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
  //Serial.print  (micros()); Serial.println( "\tFB_E"); 
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// check the error
bool Firebase_ErrorCheck (const String _path){                            
  
  if (Firebase.failed()) {                                              //Handle error 
     //Serial.print(_path + "_____________failed: "); 
     //Serial.println(Firebase.error());  
     return true; 
  } 
  return false;
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set relay state
// >220msec Relay
void Firebase_SetRelay(const String _path, const volatile bool *_state){  
  
  static bool change = !*_state;
  //Serial.print  (micros()); Serial.println( "\tFBSR_S"); 
  if(change != *_state){
    change = *_state;
    Firebase.setBool  (_path + "/RELAY" , *_state);
    if(Firebase_ErrorCheck(_path + "/RELAY")) {
      return;
    }
  }
  //Serial.print  (micros()); Serial.println( "\tFBSR_E"); 
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set relay state
// >220msec State
void Firebase_SetState(const String _path){  
  
  //Serial.print  (micros()); Serial.println( "\tFBSS_S"); 
  Firebase.setBool  (_path + "/State" , true);
  if(Firebase_ErrorCheck(_path + "/State")) {
    return;
  }
  //Serial.print  (micros()); Serial.println( "\tFBSS_E"); 
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// get phone massage
// >3090msec ~ >376.9msec
void Firebase_GetPhone(const String _path, volatile bool *_flage){

  //Serial.print  (micros()); Serial.println( "\tFBGP_S");
  String msg = Firebase.getString(_path);
  //Serial.println(msg);
  if(msg == "T"){
    *_flage = true;
  }
  Firebase.setString(_path, "");
  if(Firebase_ErrorCheck(_path)) {
    return;
  }
  //Serial.print  (micros()); Serial.println( "\tFBGP_E");

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// get timer
// >1sec ~ 935msec
void Firebase_GetTimer (const String _path, S_Timer *_timer,S_Time *_now){              // get timer

  static uint8_t i = 0;
  //Serial.print  (micros()); Serial.println( "\tFBGT_S");
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
  //Serial.print  (micros()); Serial.println( "\tFBGT_E");
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

