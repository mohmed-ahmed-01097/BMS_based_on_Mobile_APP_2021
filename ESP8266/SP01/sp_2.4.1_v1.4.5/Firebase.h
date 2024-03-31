
#ifndef FIREBASE_H
#define FIREBASE_H

#include "data_type.h"                                                  // DataType library

#define FIREBASE_HOST   "bms-smart-grid-lab.firebaseio.com"             // the project name address from firebase id
#define FIREBASE_AUTH   "E8ZidnvxgsnDjZwOiRvYLNwMxakhr3hahCL6SHUr"      // the secret key generated from firebase

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// main funtions
void Firebase_Init     (const String _path, const S_MyData *_data);         // initialization firebase

// check functions
bool Firebase_ErrorCheck (const String _path);                              // check the error

// Set data
void Firebase_SetRelay(const String _path, const volatile bool *_state);    // set switch
void Firebase_SetState(const String _path);
void Firebase_SetMeas (const String _path, const S_Meas *_measure);         // set measure to firebase 

// Set power
void Firebase_SetPower  (const String _path);                               // set Power to firebase 
void Firebase_SetPower_H(const String _path, const uint8_t _h);
void Firebase_SetPower_D(const String _path, const uint8_t _d); 
void Firebase_SetPower_M(const String _path, const uint8_t _m); 

// Get data
void Firebase_GetPhone  (const String _path, volatile bool *_flage);        // get phone massage
void Firebase_GetTimer  (const String _path, S_Timer *_timer, S_Time *_now);// get timer

// Monitor
void Monitor_Init  (void);
void Monitor_Calc  (S_MyData *_data);
void Monitor_UpDate(const S_Time *_now, const S_Date *_today);

String Monitor_Graph_H(void);
String Monitor_Graph_D(void);
String Monitor_Graph_M(void);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

