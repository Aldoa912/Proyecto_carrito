/*
 * File:   motorDC.c
 * Author: byron
 *
 * Created on March 3, 2023, 10:02 AM
 */

#include <xc.h>
#include "motorDC.h"

#define _XTAL_FREQ 8000000

int velocidad = 0;

void setupMotores(void){
    
    ANSEL = 0;
    ANSELH = 0;

    TRISA = 0;              //Configuración del PORTA como output
    TRISB = 0b00011111;     //Configuración del PORTB como input
    TRISD = 0;              //Configuración del PORTD como output
    
    OPTION_REGbits.nRBPU = 0;   //Habilitamos los pull-ups del PORTB
    WPUBbits.WPUB0 = 1;         //Habilitamos el pull-up del RB0
    WPUBbits.WPUB1 = 1;         //Habilitamos el pull-up del RB1
    WPUBbits.WPUB2 = 1;         //Habilitamos el pull-up del RB2
    WPUBbits.WPUB3 = 1;         //Habilitamos el pull-up del RB3
    WPUBbits.WPUB4 = 1;         //Habilitamos el pull-up del RB4
    IOCB = 0b00011111;

    PORTB = 0;              //Limpiamos el PORTB
    PORTD = 0;              //Limpiamos el PORTD
    
    PORTDbits.RD0 = 1;
    PORTDbits.RD1 = 0;
    PORTDbits.RD2 = 1;
    PORTDbits.RD3 = 0;      //Los motores empiezan yendo hacia adelante
    
}

void setupPWM(void){
   
    // Paso 1
    
    TRISCbits.TRISC2 = 1;           //Configuración del RC1 como input (CCP1)
    TRISCbits.TRISC1 = 1;           //Configuración del RC2 como input (CCP2)
    
    // Paso 2
    
    PR2 = 255;                  // Frecuencia de PWM de aproximadamente 490 Hz
    
    // Paso 3
    
    CCP1CONbits.P1M = 0b00;     //Selección del modo Single Output
    
    CCP1CONbits.CCP1M = 0b1100;     // P1A como PWM 
    CCP2CONbits.CCP2M = 0b1100;     // P2A como PWM
            
   // Paso 4
    
    CCP1CONbits.DC1B = 0b11;    
    CCP2CONbits.DC2B1 = 0b1;    
    CCP2CONbits.DC2B0 = 0b1;    // CCPxCON<5:4>
    
    CCPR1L = 0;                // CCPR1L
    CCPR2L = 0;                // CCPR2L
    
                                // Cálculo para 1.5mS de ancho de pulso
    
    // Paso 5
    
    PIR1bits.TMR2IF = 0;        // Bajamos la bandera de interrupción TMR2
    T2CONbits.T2CKPS = 0b11;    // Prescaler de 1:16
    T2CONbits.TMR2ON = 1;       // Se enciende el TMR2
    
    // Paso 6
    
    while(!PIR1bits.TMR2IF){};
    
    TRISCbits.TRISC2 = 0;       // Habilitamos la salida del PWM (RC2)
    TRISCbits.TRISC1 = 0;       // Habilitamos la salida del PWM (RC1)
    
}
            
void velocidad_motores(){
            
    if (!PORTBbits.RB0){
        
        CCPR1L = 62;  // 25% de ciclo de trabajo (Velocidad Baja)
        CCPR2L = 62;
        
    } // Botón en PORTB0
            
    if (!PORTBbits.RB1){
        
        CCPR1L = 125; // 50% de ciclo de trabajo (Velocidad Media)
        CCPR2L = 125;
        
    } // Botón en PORTB1
    
    if (!PORTBbits.RB2){
        
        CCPR1L = 187; // 75% de ciclo de trabajo (Velocidad Alta)
        CCPR2L = 187;
        
    } // Botón en PORTB2
            
    if (!PORTBbits.RB3){
        
        PORTDbits.RD0 = 0; // Se configuran en alto todos los pines IN para forzarlo a detenerse
        PORTDbits.RD1 = 0;
        PORTDbits.RD2 = 0;
        PORTDbits.RD3 = 0;
        
    } // Botón en PORTB3
    
    if (!PORTBbits.RB4){
        
        PORTDbits.RD0 = 1; // Se configuran los pines IN para ir hacia adelante
        PORTDbits.RD1 = 0;
        PORTDbits.RD2 = 1;
        PORTDbits.RD3 = 0;
        
    } // Botón en PORTB4

    // Esperar un poco antes de leer los botones de nuevo
    __delay_ms(50);
    
}