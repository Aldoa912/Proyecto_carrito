/* 
 * File:   TCS230.h
 * Author: aldoa
 *
 * Created on 24 de febrero de 2023, 12:47 PM
 */

#ifndef TCS230_H
#define	TCS230_H

#define _XTAL_FREQ 8000000

// Definición de pines
#define S2 RD0
#define S3 RD1
#define OUT RA0

void config_TCS230();
unsigned long read_red();
unsigned long read_green();
unsigned long read_blue();

#endif	/* TCS230_H */

