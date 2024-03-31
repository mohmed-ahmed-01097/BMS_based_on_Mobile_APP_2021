
#ifndef FIREBASE_H
#define FIREBASE_H

#include "data_type.h"                                                  // DataType library

#define FIREBASE_HOST   "bms-smart-grid-lab.firebaseio.com"             // the project name address from firebase id
#define FIREBASE_AUTH   "E8ZidnvxgsnDjZwOiRvYLNwMxakhr3hahCL6SHUr"      // the secret key generated from firebase

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// main funtions
void Firebase_Init     (const String _path, const S_MyData *_data);     // initialization firebase
void Firebase_SetData  (const String _path, const S_MyData *_data);     // set the data to firebase
void Firebase_GetData  (const String _path, S_MyData *_data);           // get the data from firebase if the it avilable
void Firebase_SetSwitch(const String _path, const bool *_state);        // set switch

// check functions
bool Firebase_ErrorCheck (const String _path);                          // check the error
bool Firebase_Stream     (const String _path);                          // check if it avilable

// Get data
void Firebase_GetPhone  (const String _path, bool *_flage);             // get phone massage
void Firebase_GetTimer  (const String _path, S_Timer *_timer);          // get timer

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

