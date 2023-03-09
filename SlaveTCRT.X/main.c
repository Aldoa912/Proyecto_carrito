/*
 * File:   main.c
 * Author: Pablo
 * Ejemplo de uso de la LCD 16x2 en modo 4 bits
 * Se utilizó y se adaptaron las librerías de Ligo George 
 * de la página www.electrosome.com
 * Enlace: https://electrosome.com/lcd-pic-mplab-xc8/
 * Created on 31 de enero de 2020, 11:20 AM
 */

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdint.h>
#include <stdio.h>

#define _XTAL_FREQ 8000000
#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

#include "LCD.h"

int estado;
int estado2;
int enviar;
int lectura1;
int lectura2;
void setup (void);

void main(void) {
    setup();
    TRISD = 0x00;
    Lcd_Init();

  while(1)
  {
      
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("ESTADO");
    
    lectura1 = PORTBbits.RB0;
    lectura2 = PORTBbits.RB1;
    
    if (lectura1 == 0){
        estado = 1;
       Lcd_Set_Cursor(2,1);
       Lcd_Write_String("ENCENDIDO"); 
    }
    else if (lectura1 == 1) {
        estado = 0;
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("APAGADO");
    }    
    else if (lectura2 == 0){
        estado2 = 1;
       Lcd_Set_Cursor(2,1);
       Lcd_Write_String("ENCENDIDO"); 
    }
    else if (lectura2 == 1) {
        estado2 = 0;
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("APAGADO");
    }    
    
    else if (estado == 1){
        enviar = 1;
    }
    
    else if (estado2 == 1){
        enviar = 2;
    }
    
    else if (estado == 0 && estado2 == 0){
        estado = 0;
    }

            

    __delay_ms(500);
  }
    return;
}


void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0b00000011;
    TRISD = 0;
    PORTD = 0;
    PORTB = 0;
    OSCCONbits.IRCF = 0b111;        
    OSCCONbits.SCS = 1;
        // Configuración del reloj interno


    // Configuración del sensor
      
}