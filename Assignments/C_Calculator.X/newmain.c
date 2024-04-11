/*
;---------------------
; Title: C Calculator
;---------------------
; Program Details: Using a 4x4 multiplex keypad, inputs two numbers and performs
  a math operation on them based on which button was pressed
; 
; Inputs: port B is used as the input keypad, though 4 of the pins are set as 
  outputs to enable the multiplexing
; Outputs: Port D is used as the output signals, attached to 8 LEDs with current limiting resistors
; Date: 04/06/2024
; File Dependencies / Libraries: None
; Compiler: xc8, 2.46
; Author: Josh Lyman
; Versions: 
; 2.0   File Finished
; 1.0	File Created
; 
; Useful links:
; Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf
; PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4
; List of Instructions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf
*/

#include <xc.h>
#include "./xc8header.h"
#include <stdlib.h>     //unneeded?
#include <stdio.h>      //unneeded?

#define _XTAL_FREQ 4000000  //4 MHz clock, for //__delay() function
#define FCY _XTAL_FREQ/4
unsigned char input_X,input_Y,Operation,Result,key;

unsigned char getKeypress(void)
{
//polls keypad input and returns value of key pressed.
//Asterisk is 0x0E, pound sign is 0x0F
    key = 0xFF;
    PORTB = 0x01;
    
    switch (PORTB)
    {
        case 0x11:
            {
                return 0x0D;
                //__delay_ms(10);
            }
                
            case 0x21:{
                return 0x0C;
                //__delay_ms(10);
            }
                
            case 0x41:{  
                return 0x0B;
                //__delay_ms(10);
            }
                
            case 0x81:{  
                return 0x0A;
                //__delay_ms(10);
            }
                
    }
    
    PORTB = 0x02;
    //__delay_ms(10);
    switch (PORTB)
    {
            case 0x12:{
                return 0x0F;
                //__delay_ms(10);
            }
                
            case 0x22:{
                return 9;
                //__delay_ms(10);
            }
                
            case 0x42:{  
                return 6;
                //__delay_ms(10);
            }
                
            case 0x82:{  
                return 3;
                //__delay_ms(10);
            }
                
    }
    PORTB = 0x04;
    //__delay_ms(10);
    switch (PORTB)
    {
            case 0x14:{
                return 0;
                //__delay_ms(10);
            }
                
            case 0x24:{
                return 8;
                //__delay_ms(10);
            }
                
            case 0x44:{  
                return 5;
                //__delay_ms(10);
            }
                
            case 0x84:{  
                return 2;
                //__delay_ms(10);
            }
                
    }
    PORTB = 0x08;
    //__delay_ms(10);
    switch (PORTB)
    {
            case 0x18:{
                return 0x0E;
                //__delay_ms(10);
            }
                
            case 0x28:{
                return 7;
                //__delay_ms(10);
            }
                
            case 0x48:{  
                return 4;
                //__delay_ms(10);
            }
                
            case 0x88:{  
                return 1;
                //__delay_ms(10);
            }
                
    }
    key = 0xFF;
    return key;
    //__delay_ms(50);
}

void getInputX()
{
    input_X = 0;
    key = 0xFF;
    while (key >= 0x0A){
        key = getKeypress();
        //__delay_ms(10);
    }
    //__delay_ms(300);
    input_X += 10*key;
    key = 0xFF;
    while (key >= 0x0A){
        key = getKeypress();
        //__delay_ms(10);
    }
    input_X += key;
    
}

void getInputY()
{
    input_Y = 0;
    key = 0xFF;
    while (key >= 0x0A){
        key = getKeypress();
        //__delay_ms(10);
    }
    //__delay_ms(300);
    input_Y += 10*key;
    key = 0xFF;
    while (key >= 0x0A){
        key = getKeypress();
        //__delay_ms(10);
    }
    input_Y += key;
    
}
void getInputOperation()
{
    Operation = 0;
    key = 0xFF;
    while (key > 0x0F || key < 0x0A)         //0x0A is the first value too large fir it to  be a digit 
    {
        key = getKeypress();    //Loop checking input until the value is a digit
        //__delay_ms(10);
    }
        Operation = key;

    
}

unsigned char evaluate(input_X,input_Y,Operation)
{
    unsigned char result = 0;
    switch (Operation)
    {
            case 0x0A:
                result = input_X + input_Y;
                return result;
            case 0x0B:
                result = input_X - input_Y;
                return result;
            case 0x0C:
                result = input_X * input_Y;
                return result;
            case 0x0D:
                if (input_Y != 0)       //divide by zero prevention
                    result = input_X / input_Y;
                return result;
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
        //__delay_ms(500);
        getInputOperation();
        PORTD = 0x03;
        //__delay_ms(500);
        getInputY();
        PORTD = 0x02;
        //__delay_ms(500);
        
        /*
        key = 0xFF;
        while(key != 0x0F)
        {
            //__delay_ms(10);
            key = getKeypress();
        }//wait for pound sign
        */
        
        Result = evaluate(input_X,input_Y,Operation);
        display(Result);
        //__delay_ms(10);
        /*
        key = 0xFF;
        while(getKeypress() != 0x0E)
        {
            //__delay_ms(10);
            key = getKeypress();
        }//wait for asterisk
        PORTD = 0x80;
         */ 
        if (0)
            break;
    }
    return;
}
