/* 
 * File:   data_type.h
 * Author: MAAM
 *
 * Created on October 1, 2020, 11:16 AM
 */

#ifndef DATA_TYPE_H
#define	DATA_TYPE_H

#include <xc.h>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////

#define _XTAL_FREQ      8000000      //8MHz
#define UART_BAUD_RATE  9600 

////////////////////////////////////////////////////////////////////////////////

typedef char   int8_t;
typedef int    int16_t;
typedef long   int32_t;

#define INT8_MIN    (-128)
#define INT8_MAX    (127)
#define INT16_MIN   (-32768)
#define INT16_MAX   (32767)
#define INT32_MIN   (-2147483648L)
#define INT32_MAX   (2147483647L)

typedef unsigned char   uint8_t;
typedef unsigned int    uint16_t;
typedef unsigned long   uint32_t;
typedef volatile unsigned char   vuint8_t;
typedef volatile unsigned int    vuint16_t;
typedef volatile unsigned long   vuint32_t;

#define UINT8_MAX   (255)
#define UINT16_MAX  (65535U)
#define UINT32_MAX  (4294967295UL)

////////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus
extern "C" {
#endif
#ifdef	__cplusplus
}
#endif
#endif	/* DATA_TYPE_H */

