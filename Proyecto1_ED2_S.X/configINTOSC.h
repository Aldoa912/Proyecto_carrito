/* 
 * File:   configINTOSC.h
 * Author: byron
 *
 * Created on January 26, 2023, 11:13 PM
 */

#ifndef CONFIGINTOSC_H
#define	CONFIGINTOSC_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <pic16f887.h>

typedef enum 
{
    FOSC_8MHZ  = 0b0111,
    FOSC_4MHZ = 0b0110,
    FOSC_2MHZ = 0b0101,
    FOSC_1MH = 0b0100,
    FOSC_500KHZ = 0b0011,
    FOSC_250KHZ = 0b0010,
    FOSC_125KHZ = 0b0001
}F_OSC;

void setupINTOSC (F_OSC);

#endif	/* CONFIGINTOSC_H */