
#ifndef TIME_H
#define TIME_H

#include "data_type.h"                                                  // DataType library

// Set offset time in seconds to adjust for your timezone, for example: 
#define TIME_ZONE_OFFSET  7200                                          // GMT +1 = 3600  // GMT +2 = 7200
#define START_DATE        1900                                          // Year Start 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// time init and update
void    Time_Init(S_Time *_now, S_Date *_today);                        // initialization web time
void    Time_UpDate(S_Time *_now, S_Date *_today, bool _wifi_state);    // updata the Time and the Date

// return Time
uint8_t Time_GetSec  (void);                                            // Return the secondes number and updata the web clinet 
String  Time_GetDay  (void);                                            // Return the day name 
String  Time_GetMonth(S_Date *_today);                                  // Return the Month name 

// get time and date
void    Time_GetTime(S_Time *_now);                                     // get the time of now
void    Time_GetDate(S_Date *_today);                                   // get the date of today

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

