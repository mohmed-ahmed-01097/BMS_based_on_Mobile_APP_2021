
#include "Firebase.h"                                                   // firebase library
#include "EEPROM.h"                                                     // eeprom library 
#include <FirebaseArduino.h>                                            // firebase library

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// initialization firebase
// >13msec
void Firebase_Init (const String _path, const S_MyData *_data){  
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                       // connect to firebase  ~~0.5msec

  Firebase.setBool(_path + "/Data/State" , true);               
  if(Firebase.getString(_path + "/Data/Phone") == "")
     Firebase.setString(_path + "/Data/Phone", "MAAM");
  if(Firebase.getString(_path + "/Data/Timer/T0/Start/Min") == "")
     for(int i = 0; i < MAX_TIMER; i++){
        Serial.print  (micros()); Serial.println( "\tF"); 
        Firebase.setBool(_path + "/Data/Timer" + "/T" + String(i) + "/State"    ,_data->Timer[i].State);
        Firebase.setInt (_path + "/Data/Timer" + "/T" + String(i) + "/Start/Min",_data->Timer[i].Start.Min);
        Firebase.setInt (_path + "/Data/Timer" + "/T" + String(i) + "/Start/Hr" ,_data->Timer[i].Start.Hr);  
        Firebase.setInt (_path + "/Data/Timer" + "/T" + String(i) + "/End/Min"  ,_data->Timer[i].End.Min);
        Firebase.setInt (_path + "/Data/Timer" + "/T" + String(i) + "/End/Hr"   ,_data->Timer[i].End.Hr); 
    } 
    
  Firebase_Stream(_path + "/Data");
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// check if it avilable
// >970msec
bool Firebase_Stream (const String _path){

  Firebase.stream(_path);
  return Firebase.success();
  
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
// set the data to firebase
void Firebase_SetData (const String _path, const S_MyData *_data){ 

    Firebase_SetSwitch(_path + "/Data"         , &_data->Switch.D_State);
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set relay state
// >400msec
void Firebase_SetSwitch(const String _path, const bool *_state){  
 
  //if(!Firebase.getBool  (_path + "/State")){
    Firebase.setBool  (_path + "/State" , true);
    if(Firebase_ErrorCheck(_path + "/State")) {
      return;
    }
  //}
  if(Firebase.getBool  (_path + "/RELAY") == *_state){
    Firebase.setBool  (_path + "/RELAY" , *_state);
    if(Firebase_ErrorCheck(_path + "/RELAY")) {
      return;
    }
  }  
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// get the data from firebase if the it avilable
void Firebase_GetData (const String _path, S_MyData *_data){          

   Serial.print  (micros()); Serial.println( "\tFBG_S"); 
  //check if streaming failed and try to restart
  if (Firebase.failed()) {
    //Serial.println("streaming error");
    //Serial.println(Firebase.error());
    delayMicroseconds(10000);
    Firebase_Stream(_path + "/Data");
    return;
  }
  if(Firebase.available()){
    /*
    if(Firebase.getString(_path + "/Data/Phone") == "")
      Firebase_GetTimer(_path + "/Data/Timer", _data->Timer);
    else
      Firebase_GetPhone(_path + "/Data/Phone", &_data->Switch.Toggle_F);
    Firebase_Stream(_path + "/Data");
    */
      Firebase_GetPhone(_path + "/Data/Phone", &_data->Switch.Toggle_F);
      Firebase_GetTimer(_path + "/Data/Timer",  _data->Timer);
  }
   Serial.print  (micros()); Serial.println( "\tFBG_E");
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// get phone massage
// >590msec
void Firebase_GetPhone(const String _path, bool *_flage){

   Serial.print  (micros()); Serial.println( "\tFBGP_S");
  String Phone_msg = Firebase.getString(_path);
  if(Phone_msg == "T")
    *_flage = true;
  
  Firebase.setString(_path, "");
  if(Firebase_ErrorCheck(_path)) {
    return;
  }
   Serial.print  (micros()); Serial.println( "\tFBGP_E");

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// get timer
void Firebase_GetTimer (const String _path, S_Timer *_timer){              // get timer

   Serial.print  (micros()); Serial.println( "\tFBGT_S");
  bool change = false;
  for(int i = 0; i < MAX_TIMER; i++){
    bool state = Firebase.getBool(_path + "/T" + String(i) + "/State");
    if( state != _timer[i].State){
      _timer[i].State     = Firebase.getBool(_path + "/T" + String(i) + "/State");
      _timer[i].Start.Min = Firebase.getInt (_path + "/T" + String(i) + "/Start/Min");
      _timer[i].Start.Hr  = Firebase.getInt (_path + "/T" + String(i) + "/Start/Hr");  
      _timer[i].End.Min   = Firebase.getInt (_path + "/T" + String(i) + "/End/Min");
      _timer[i].End.Hr    = Firebase.getInt (_path + "/T" + String(i) + "/End/Hr"); 
      change = true;
    } 
  }
  if(change){
    EEPROM_Write_Timer(_timer);   
  }
   Serial.print  (micros()); Serial.println( "\tFBGT_E");
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

