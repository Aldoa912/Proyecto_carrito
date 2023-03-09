/*
 * File:   configLCD_4bits.c
 * Author: byron
 *
 * Created on January 26, 2023, 11:06 PM
 */

#include <xc.h>

//LCD Functions Developed by electroSome

#include "configLCD_4bits.h"

void Lcd_Port_4bits(char a) {
    if (a & 1)
        D4 = 1;
    else
        D4 = 0;

    if (a & 2)
        D5 = 1;
    else
        D5 = 0;

    if (a & 4)
        D6 = 1;
    else
        D6 = 0;

    if (a & 8)
        D7 = 1;
    else
        D7 = 0;
}

void Lcd_Cmd_4bits(char a) {
    RS = 0; // => RS = 0 // Dato en el puerto lo va interpretar como comando
    Lcd_Port_4bits(a);
    EN = 1; // => E = 1
    __delay_ms(4);
    EN = 0; // => E = 0
}

void Lcd_Clear_4bits(void) {
    Lcd_Cmd_4bits(0);
    Lcd_Cmd_4bits(1);
}

void Lcd_Set_Cursor_4bits(char a, char b) {
    char temp, z, y;
    if (a == 1) {
        temp = 0x80 + b - 1;
        z = temp >> 4;
        y = temp & 0x0F;
        Lcd_Cmd_4bits(z);
        Lcd_Cmd_4bits(y);
    } else if (a == 2) {
        temp = 0xC0 + b - 1;
        z = temp >> 4;
        y = temp & 0x0F;
        Lcd_Cmd_4bits(z);
        Lcd_Cmd_4bits(y);
    }
}

void Lcd_Init_4bits(void) {
    Lcd_Port_4bits(0x00);
    __delay_ms(20);
    Lcd_Cmd_4bits(0x03);
    __delay_ms(5);
    Lcd_Cmd_4bits(0x03);
    __delay_ms(11);
    Lcd_Cmd_4bits(0x03);
    /////////////////////////////////////////////////////
    Lcd_Cmd_4bits(0x02);
    Lcd_Cmd_4bits(0x02);
    Lcd_Cmd_4bits(0x08);
    Lcd_Cmd_4bits(0x00);
    Lcd_Cmd_4bits(0x0C);
    Lcd_Cmd_4bits(0x00);
    Lcd_Cmd_4bits(0x06);
}

void Lcd_Write_Char_4bits(char a) {
    char temp, y;
    temp = a & 0x0F;
    y = a & 0xF0;
    RS = 1; // => RS = 1
    Lcd_Port_4bits(y >> 4); //Data transfer
    EN = 1;
    __delay_us(40);
    EN = 0;
    Lcd_Port_4bits(temp);
    EN = 1;
    __delay_us(40);
    EN = 0;
}

void Lcd_Write_String_4bits(char *a) {
    int i;
    for (i = 0; a[i] != '\0'; i++)
        Lcd_Write_Char_4bits(a[i]);
}

void Lcd_Shift_Right_4bits(void) {
    Lcd_Cmd_4bits(0x01);
    Lcd_Cmd_4bits(0x0C);
}

void Lcd_Shift_Left_4bits(void) {
    Lcd_Cmd_4bits(0x01);
    Lcd_Cmd_4bits(0x08);
}
