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


#include "TCS230.h"
#include "configI2C.h"

unsigned long red_freq = 0;
unsigned long blue_freq = 0;
unsigned long green_freq = 0;
uint8_t estado;
char rojo[4];
char azul[4];
char verde[4];
char cont [3];
void setup (void);

uint8_t z;

void setup(void);

//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() isr(void){
   if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            PORTD = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = estado;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
}

void main(void) {
  
    setup();
    TRISD = 0x00;
    config_TCS230();

    while(1)
    {   
        // se leen las frecuencias de cada uno de los colores
        red_freq = read_red();
        green_freq = read_green();
        blue_freq = read_blue();
        
        // se comparan las frecuencias para decir si el color detectado es verde
        if (red_freq < 185 && red_freq > 170 && green_freq < 220 && green_freq > 210 && blue_freq < 70 && blue_freq > 60){
            estado = 1;
        }
        // se comparan las frecuencias para decir si el color detectado es azul
        else if (red_freq < 195 && red_freq > 180 && green_freq < 235 && green_freq > 220 && blue_freq < 68 && blue_freq > 50){
            estado = 2;
        }
        // se comparan las frecuencias para decir si el color detectado es azul  
        else if (red_freq < 130 && red_freq > 115 && green_freq < 260 && green_freq > 240 && blue_freq < 68 && blue_freq > 50){
            estado = 3;
        }
        // si no es ninguno de esos colores, envia 0
        else {
            estado = 0;
    }
    
    
    __delay_ms(100);
    
    
        
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
    I2C_Slave_Init(0x30);     // Inicializar Comuncación I2C
        // Configuración del reloj interno


    // Configuración del sensor
      
}