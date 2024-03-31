
#ifndef TIME_H
#define TIME_H

#include "data_type.h"                                                  // DataType library

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// time init and update
void    Time_Init(S_Time *_now, S_Date *_today);                        // initialization web time

// return Time
String  Time_GetDay  (void);                                            // Return the day name 
String  Time_GetMonth(S_Date *_today);                                  // Return the Month name 

// get time and date
void Time_GetSec (uint8_t *_sec);                                     // Return the secondes number and updata the web clinet 
void Time_GetTime(S_Time  *_now);                                     // get the time of now
void Time_GetDate(S_Date  *_today);                                   // get the date of today

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

