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
// se definen las variables
float distancia;
uint8_t dis;
uint8_t color;
uint8_t linea;

char distanciaLCD[16];

void main(void) {
  
    __delay_ms(500);
    // se inician los puertos y el oscilador
    setup();
    // se inicia el LCD y los motores
    Lcd_Clear_4bits();
    setupMotores();
    setupPWM();
    
    
    CCPR1L = 125; // 50% de ciclo de trabajo (Velocidad Media)
    CCPR2L = 125;

    while(1)
    {   
        // Se mandan a traer los datos de los slaves
//        __delay_ms(500);
        I2C_Master_Start();     // se inicia la comunicacion
        I2C_Master_Write(0x11); //direccion del pic1
        distancia = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(10);
        I2C_Master_Start();     // se inicia la comunicacion
        I2C_Master_Write(0x31); // Direccion del pic 2
        color = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(10);
        I2C_Master_Start();     // se inicia la comunicacion
        I2C_Master_Write(0x21); // Direccion del pic3
        linea = I2C_Master_Read(0);
        I2C_Master_Stop();      
        __delay_ms(10);
        
        
        //Enviamos información al ESP32
        I2C_Master_Start();
        I2C_Master_Write(0x90);
        I2C_Master_Write(color);
        I2C_Master_Write(distancia);
        I2C_Master_Write(linea);
        I2C_Master_Stop();
        __delay_ms(10);
        
        
        //distancia = medir_distancia();
        // Se convierte la variable de distancia en un string
        sprintf(distanciaLCD, "Distancia: %.1f", distancia);    
        //se escribe el valor de la distancia en el LCD
        Lcd_Set_Cursor_4bits(1,1);
        Lcd_Write_String_4bits(distanciaLCD);
        
        // se verifica si no esta leyendo el sensor algun color
        if (color == 0){
            Lcd_Set_Cursor_4bits(2,1);
            Lcd_Write_String_4bits("NADA"); // no hay ningun color

        } 
            
        // se verifica si no esta leyendo el verde
        else if (color == 1){

            CCPR1L = 62;  // 25% de ciclo de trabajo (Velocidad Baja)
            CCPR2L = 62;
            Lcd_Set_Cursor_4bits(2,1);
            Lcd_Write_String_4bits("VERDE");   // se escribe que se lee el verde
        } // Botón en PORTB0

        // se verifica si no esta leyendo el azul
        else if (color == 2){

            CCPR1L = 125; // 50% de ciclo de trabajo (Velocidad Media)
            CCPR2L = 125;
            Lcd_Clear_4bits();
            Lcd_Set_Cursor_4bits(2,1);
            Lcd_Write_String_4bits("AZUL");     // se escribe que se lee el azul

        } // Botón en PORTB1

        // se verifica si no esta leyendo el azul
        else if (color == 3){

            CCPR1L = 187; // 75% de ciclo de trabajo (Velocidad Alta)
            CCPR2L = 187;
            Lcd_Clear_4bits();
            Lcd_Set_Cursor_4bits(2,1);
            Lcd_Write_String_4bits("ROJO");     // se escribe que se lee el rojo

        }

        // se verifica que la distancia sea menor de 5 cm
        if (distancia < 5){

            PORTAbits.RA0 = 0; // Se configuran en alto todos los pines IN para forzarlo a detenerse
            PORTAbits.RA1 = 0;
            PORTAbits.RA2 = 0;
            PORTAbits.RA3 = 0;
            Lcd_Set_Cursor_4bits(2,7);
            Lcd_Write_String_4bits("TAPADO");  // se escribe que el camino esta tapado
            Lcd_Set_Cursor_4bits(2,16);
            Lcd_Write_String_4bits("N");      // indiica que ningun sensor de linea esta cubierto

        } 

        // se verifica que la distancia sea mayor de 5 cm
        if (distancia > 5){

            PORTAbits.RA0 = 0; // Se configuran los pines IN para ir hacia adelante
            PORTAbits.RA1 = 1;
            PORTAbits.RA2 = 1;
            PORTAbits.RA3 = 0;
            Lcd_Set_Cursor_4bits(2,7);
            Lcd_Write_String_4bits("LIBRE");     // se escribe que el camino esta libre
            Lcd_Set_Cursor_4bits(2,16);
            Lcd_Write_String_4bits("N");        // indiica que ningun sensor de linea esta cubierto
        } // Botón en PORTB4

        if (linea == 1){

            PORTAbits.RA0 = 0; // Se configuran los pines IN para ir hacia adelante solo el motor derecho
            PORTAbits.RA1 = 1;
            PORTAbits.RA2 = 0;
            PORTAbits.RA3 = 0;
            Lcd_Set_Cursor_4bits(2,16);
            Lcd_Write_String_4bits("!");    // indiica que un sensor de linea esta cubierto

        } // Botón en PORTB4
        
        if (linea == 2){

            PORTAbits.RA0 = 0; // Se configuran los pines IN para ir hacia adelante solo el motor izquierdo
            PORTAbits.RA1 = 0;
            PORTAbits.RA2 = 1;
            PORTAbits.RA3 = 0;
            Lcd_Set_Cursor_4bits(2,16);
            Lcd_Write_String_4bits("!");   // indiica que un sensor de linea esta cubierto

        } // Botón en PORTB4
        // Esperar un poco antes de leer los botones de nuevo
        __delay_ms(10);

        
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
    TRISB = 0b00000000;     //Configuración del PORTB como input
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
