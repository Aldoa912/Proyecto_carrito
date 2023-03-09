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
#include "TCS230.h"

unsigned long red_freq = 0;
unsigned long blue_freq = 0;
unsigned long green_freq = 0;
int estado;
char rojo[4];
char azul[4];
char verde[4];
char cont [3];
void setup (void);

void main(void) {
    setup();
    TRISD = 0x00;
    Lcd_Init();
    config_TCS230();

  while(1)
  {
      
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("COLOR:");  
//    Lcd_Write_String("ROJO  VERDE  AZUL");  
    red_freq = read_red();
    green_freq = read_green();
    blue_freq = read_blue();
    
    if (red_freq < 130 && red_freq > 122 && green_freq < 150 && green_freq > 140 && blue_freq < 80 && blue_freq > 65){
        estado = 1;
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("VERDE");
    }
    
    if (red_freq < 180 && red_freq > 170 && green_freq < 185 && green_freq > 170 && blue_freq < 75 && blue_freq > 65){
        estado = 2;
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("AZUL");
    }
//    
    if (red_freq < 100 && red_freq > 70 && green_freq < 210 && green_freq > 160 && blue_freq < 70 && blue_freq > 50){
        estado = 3;
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("ROJO");
    }
    
    else {
        estado = 0;
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("NADA");
    }
    
    
    sprintf(rojo, "%lu", red_freq);
    sprintf(azul, "%lu", blue_freq);
    sprintf(verde, "%lu", green_freq);

    

    Lcd_Set_Cursor(2,6);
    Lcd_Write_String(rojo);
    Lcd_Set_Cursor(2,10);
    Lcd_Write_String(verde);
    Lcd_Set_Cursor(2,14);
    Lcd_Write_String(azul);
    
    __delay_ms(200);
  }
    return;
}


void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0b00000010;
    TRISD = 0;
    PORTD = 0;
    OSCCONbits.IRCF = 0b111;        
    OSCCONbits.SCS = 1;
        // Configuración del reloj interno


    // Configuración del sensor
      
}