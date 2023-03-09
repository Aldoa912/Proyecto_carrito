#include "TCS230.h"
#include <xc.h>

// Variables globales


// Funciones de configuración
void config_TCS230(){
    T1CONbits.TMR1CS = 0; // Usar reloj interno (Fosc/4)
    T1CONbits.T1CKPS = 0b01; // Prescaler 1:8
    T1CONbits.nT1SYNC = 1; // No sincronizar la entrada de reloj externo
    T1CONbits.TMR1ON = 0; // Disabilitamos el TMR1

}

// Función de lectura de frecuencia para el canal rojo
unsigned long read_red(){
  unsigned long temp_freq;
  S2 = 0; // Selecciona el canal rojo
  S3 = 0;
  __delay_us(30); // Espera 10 microsegundos para que se estabilice la señal
  while(PORTAbits.RA1 == 1){ // Espera hasta que la medición haya terminado
  }
  while(PORTAbits.RA1 == 0){ // Espera hasta que la medición haya terminado
  }
  TMR1 = 0; // Reinicia el contador
  T1CONbits.TMR1ON = 1;
  while(PORTAbits.RA1 == 1){ // Espera hasta que la medición haya terminado

  }
  T1CONbits.TMR1ON = 0;
  temp_freq = TMR1; // Lee el valor del contador
  TMR1 = 0; // Reinicia el contador
  return (unsigned long)((float)temp_freq * 1.0); // Calcula y devuelve la frecuencia en Hz
}

// Función de lectura de frecuencia para el canal verde
unsigned long read_green(){
  unsigned long temp_freq;
  S2 = 1; // Selecciona el canal verde
  S3 = 1;
  __delay_us(30);
  while(PORTAbits.RA1 == 1){ // Espera hasta que la medición haya terminado
  }
  while(PORTAbits.RA1 == 0){ // Espera hasta que la medición haya terminado
  }
  TMR1 = 0; // Reinicia el contador
  T1CONbits.TMR1ON = 1;
  while(PORTAbits.RA1 == 1){ // Espera hasta que la medición haya terminado
  }
  T1CONbits.TMR1ON = 0;
  temp_freq = TMR1; // Lee el valor del contador
  TMR1 = 0; // Reinicia el contador
  return (unsigned long)((float)temp_freq * 1.0);
}

// Función de lectura de frecuencia para el canal azul
unsigned long read_blue(){
  unsigned long temp_freq;
  S2 = 0; // Selecciona el canal azul
  S3 = 1;
  __delay_us(30);
  while(PORTAbits.RA1 == 1){ // Espera hasta que la medición haya terminado
  }
  while(PORTAbits.RA1 == 0){ // Espera hasta que la medición haya terminado
  }
  TMR1 = 0; // Reinicia el contador
  T1CONbits.TMR1ON = 1;
  while(PORTAbits.RA1 == 1){ // Espera hasta que la medición haya terminado
  }
  temp_freq = TMR1; // Lee el valor del contador
  T1CONbits.TMR1ON = 0;

  TMR1 = 0; // Reinicia el contador
  return (unsigned long)((float)temp_freq * 1.0);
}

