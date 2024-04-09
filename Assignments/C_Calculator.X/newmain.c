/*
;---------------------
; Title: C Calculator
;---------------------
; Program Details: Using a 4x4 multiplex keypad, inputs two numbers and performs
  a math operation on them based on which button was pressed
; 
; Inputs: port B is used as the input keypad, though 4 of the pins are set as 
  outputs to enable the multiplexing
; Outputs: Port D is used as the output signals for the 7 segment display
  Port C pins 2 and 3 are used to select which digit on the seven segment is lit
; Date: 04/06/2024
; File Dependencies / Libraries: None
; Compiler: xc8, 2.46
; Author: Josh Lyman
; Versions: 
; 
; 1.0	File Created
; 
; Useful links:
; Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf
; PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4
; List of Instructions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf
*/

#include <xc.h>
#include <stdlib.h>     //unneeded?
#include <stdio.h>      //unneeded?

#define _XTAL_FREQ 4000000  //4 MHz clock, for __delay() function
unsigned char input_X,input_Y,Operation,Result,key;

unsigned char getKeypress()
{
//polls keypad input and returns value of key pressed.
//Asterisk is 0x0E, pound sign is 0x0F
    key = 0xFF;
    PORTB = 0x01;
    __delay_ms(10);
    unsigned char firstRead = PORTB;
    switch (PORTB)
    {
            case 0x11:
                key = 0x0D;
                break;
            case 0x21:
                key = 0x0C;
                break;
            case 0x41:  
                key = 0x0B;
                break;
            case 0x81:  
                key = 0x0A;
                break;
    }
    
    PORTB = 0x02;
    __delay_ms(10);
    switch (PORTB)
    {
            case 0x12:
                key = 0x0F;
                break;
            case 0x22:
                key = 9;
                break;
            case 0x42:  
                key = 6;
                break;
            case 0x82:  
                key = 3;
                break;
    }
    PORTB = 0x04;
    __delay_ms(10);
    switch (PORTB)
    {
            case 0x14:
                key = 0;
                break;
            case 0x24:
                key = 8;
                break;
            case 0x44:  
                key = 5;
                break;
            case 0x84:  
                key = 2;
                break;
    }
    PORTB = 0x08;
    __delay_ms(10);
    switch (PORTB)
    {
            case 0x18:
                key = 0x0E;
                break;
            case 0x28:
                key = 7;
                break;
            case 0x48:  
                key = 4;
                break;
            case 0x88:  
                key = 1;
                break;
    }
    __delay_ms(50);
    unsigned char secondRead = PORTB;
    if (secondRead != 0xFF && secondRead == PORTB)
        return key;
    return 0xFF;
}

void getInputX()
{
    input_X = 0;
    key = 0xFF;
    while (key >= 0x0A)         //0x0A is the first value too large fir it to  be a digit 
        key = getKeypress();    //Loop checking input until the value is a digit
        __delay_ms(10);
    input_X = (input_X << 4) + key;     //shifts X left 4 and puts new value in lower nibble
    //Value is now 0x0n for n being a digit on the keypad
    
    key = 0xFF;
    while (key >= 0x0A)
        key = getKeypress();
        __delay_ms(10);
    input_X = (input_X << 4) + key;     //shifts X left 4 and puts new value in lower nibble
    //Value is now 0xnm for n being the first digit and m the second
    //This is in packed BCD
    
}

void getInputY()
{
    input_Y = 0;
    key = 0xFF;
    while (key >= 0x0A)
        key = getKeypress();
        __delay_ms(10);
    input_Y = (input_Y << 4) + key;     //shifts Y left 4 and puts new value in lower nibble
    //Value is now 0x0n for n being a digit on the keypad
    
    key = 0xFF;
    while (key >= 0x0A)
        key = getKeypress();
        __delay_ms(10);
    input_Y = (input_Y << 4) + key;     //shifts Y left 4 and puts new value in lower nibble
    //Value is now 0xnm for n being the first digit and m the second
    
}

void getInputOperation()
{
    Operation = 0;
    key = 0xFF;
    while ((key > 0x0D) || (key < 0x0A))    //don't accept values for key lower than 0x0A (digit) or 
                                            //higher than 0x0D (asterisk, pound sign, or no input)
        key = getKeypress();
        __delay_ms(10);
    Operation = key;

    
}

unsigned char evaluate(input_X,input_Y,Operation)
{
    unsigned char result = 0;
    switch (Operation)
    {
            case 0x0A:
                result = input_X + input_Y;
                break;
            case 0x0B:
                result = input_X - input_Y;
                break;
            case 0x0C:
                result = input_X * input_Y;
                break;
            case 0x0D:
                result = input_X / input_Y;
                break;
            default:
                result = 0;
    }
    return result;            
}

void display(unsigned char Result)
{
    PORTD = Result;    
}

void main(void) {
  //init
    //using port B as keypad I/O
    TRISB = 0xF0;   //lower 4 bits are output, upper 4 input
    LATB = 0;
    PORTB = 0;
    ANSELB = 0;
    
    //using port D as output to 7 seg
    TRISD = 0;   //all 8 pins outputs
    LATD = 0;
    PORTD = 0;
    ANSELD = 0;
    
    //loop
    while (1)
    {
       
        
        getInputX();
        PORTD = 0x01;
        
        getInputOperation();
        
        getInputY();
        PORTD = 0x02;
        
        while(getKeypress != 0x0F)
        {__delay_ms(100);}//wait for pound sign
        
        Result = evaluate(input_X,input_Y,Operation);
        display(Result);
        while(getKeypress != 0x0F)
        {__delay_ms(100);}//wait for pound sign
        
    }
    return;
    
}
