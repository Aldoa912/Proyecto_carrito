/*
 * File:   main_M.c
 * Author: byron
 *
 * Created on February 9, 2023, 11:13 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT        // Oscillator Selection bits 
                                            // (INTOSC oscillator: I/O function 
                                            // on RA6/OSC2/CLKOUT pin, I/O 
                                            // function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and 
                                // can be enabled by SWDTEN bit of the WDTCON 
                                // register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR 
                                // pin function is digital input, MCLR 
                                // internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code 
                                // protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code 
                                // protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit 
                                // (Internal/External Switchover mode disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit 
                                // (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin 
                                // has digital I/O, HV on MCLR must be used for 
                                // programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out 
                                // Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 
                                // (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "configINTOSC.h"
#include "configI2C.h"
#include "configLCD_4bits.h"
#include "configSensorUltrasonico.h"
#include "motorDC.h"

#define _XTAL_FREQ 8000000

#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RB5
#define D6 RD6
#define D7 RD7

void setup(void);

float distancia;

char distanciaLCD[16];

void main(void) {
  
    setup();
    Lcd_Clear_4bits();
    
//    setupMotores();
//    setupPWM();

    while(1)
    {   
//        __delay_ms(500);
        I2C_Master_Start();
        I2C_Master_Write(0x11);
        distancia = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        //distancia = medir_distancia();
        sprintf(distanciaLCD, "Distancia: %d cm", distancia);

        Lcd_Set_Cursor_4bits(1,1);
        Lcd_Write_String_4bits(distanciaLCD);
        
        
//        velocidad_motores();
        
    }
    return;
}

//******************************************************************************
// Función para configurar GPIOs
//******************************************************************************

void setup (void){
    
    ANSEL = 0;
    ANSELH = 0;

    TRISA = 0;              //Configuración del PORTA como output
    TRISB = 0b00000010;     //Configuración del PORTB como input
    TRISD = 0;              //Configuración del PORTD como output
    
//    OPTION_REGbits.nRBPU = 0;   //Habilitamos los pull-ups del PORTB
//    WPUBbits.WPUB0 = 1;         //Habilitamos el pull-up del RB0
//    WPUBbits.WPUB1 = 1;         //Habilitamos el pull-up del RB1
//    WPUBbits.WPUB2 = 1;         //Habilitamos el pull-up del RB2
//    IOCB = 0b00000111;

    PORTA = 0;              //Limpiamos el PORTA
    PORTB = 0;              //Limpiamos el PORTB
    PORTD = 0;              //Limpiamos el PORTD

    I2C_Master_Init(100000);    // Inicializar Comuncación I2C
    setupINTOSC(FOSC_8MHZ);     //Configuramos el oscilador interno a 4MHZ
    Lcd_Init_4bits();           //Inicializamos la LCD en modo de 4 bits
    
//    INTCONbits.GIE = 1;     //Habilitamos las interrupciones globales (GIE)
//    INTCONbits.PEIE = 1;    //Habilitamos las interrupción del PEIE
//    INTCONbits.RBIE = 1;    //Habilitamos las interrupciones del PORTB (RBIE)
//    INTCONbits.RBIF = 0;    //Bajamos la bandera de interrupción del PORTB (RBIF)
}
