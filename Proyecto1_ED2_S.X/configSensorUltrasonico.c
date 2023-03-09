/*
 * File:   configSensorUltrasonico.c
 * Author: byron
 *
 * Created on February 24, 2023, 11:09 AM
 */


#include <xc.h>
#include "configSensorUltrasonico.h"

#define _XTAL_FREQ 8000000 // Frecuencia de operación del microcontrolador

// Inicialización del TMR1

void tmr1_init() {
    T1CONbits.TMR1CS = 0; // Usar reloj interno (Fosc/4)
    T1CONbits.T1CKPS = 0b11; // Prescaler 1:8
    T1CONbits.nT1SYNC = 1; // No sincronizar la entrada de reloj externo
    T1CONbits.TMR1ON = 0; // Disabilitamos el TMR1
}

/* Función que mide la distancia utilizando el sensor ultrasónico
 * Recibe como parámetro el pin TRIGGER y ECHO del sensor, respectivamente
 * Retorna la distancia medida en centímetros
*/

float medir_distancia() {
    
    // Configurar los pines de TRIGGER y ECHO
    TRISAbits.TRISA0 = 0; // TRIGGER como salida
    TRISAbits.TRISA1 = 1; // ECHO como entrada
    
    // Configurar el Timer1
    tmr1_init();
    
    // Generar un pulso de 10us en el pin TRIGGER
    PORTAbits.RA0 = 1; // Establecer el pin TRIGGER en estado alto
    __delay_us(10); // Esperar 10us
    PORTAbits.RA0 = 0; // Establecer el pin TRIGGER en estado bajo
    
    // Esperar hasta que el pin ECHO cambie a estado alto
    while (PORTAbits.RA1 == 0);
    
    // Iniciar el TMR1
    TMR1 = 0;
    T1CONbits.TMR1ON = 1;
    
    // Esperar hasta que el pin ECHO cambie a estado bajo
    while (PORTAbits.RA1 == 1);
    
    // Detener el TMR1
    T1CONbits.TMR1ON = 0;
    
    // Calcular la duración del eco
    unsigned int echo_duration = TMR1 * 4; // Multiplicar por el prescaler
    
    // Calcular la distancia medida en centímetros
    float distancia = echo_duration / 58.0; // Dividir por el factor de conversión (58us/cm)
    
    return distancia;
}
