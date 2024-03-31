
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

// Get data
void Firebase_GetPhone  (const String _path, volatile bool *_flage);        // get phone massage
void Firebase_GetTimer  (const String _path, S_Timer *_timer, S_Time *_now);// get timer

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

