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
const unsigned char lookupTable[16] = {
0b11100111,      //encodes 0
0b00100001,      //encodes 1
0b11001011,      //encodes 2
0b01101011,      //encodes 3
0b00101101,      //encodes 4
0b01101110,      //encodes 5
0b11101110,      //encodes 6
0b00100111,      //encodes 7
0b11101111,      //encodes 8
0b01101111,      //encodes 9
0b10101111,      //encodes A
0b11101100,      //encodes B
0b11001000,      //encodes C
0b11101001,      //encodes D
0b11001110,      //encodes E
0b10001110      //encodes F
};

unsigned char getKeypress()
{
//polls keypad input and returns value of key pressed.
//Asterisk is 0x0E, pound sign is 0x0F
    key = 0xFF;
    PORTB = 0x01;
    switch (PORTB)
    {
            case 0x11:
                key = 1;
                return key;
            case 0x21:
                key = 2;
                return key;
            case 0x41:  
                key = 3;
                return key;
            case 0x81:  
                key = 0x0A;
                return key;
    }
    
    PORTB = 0x02;
    switch (PORTB)
    {
            case 0x12:
                key = 4;
                return key;
            case 0x22:
                key = 5;
                return key;
            case 0x42:  
                key = 6;
                return key;
            case 0x82:  
                key = 0x0B;
                return key;
    }
    PORTB = 0x04;
    switch (PORTB)
    {
            case 0x14:
                key = 7;
                return key;
            case 0x24:
                key = 8;
                return key;
            case 0x44:  
                key = 9;
                return key;
            case 0x84:  
                key = 0x0C;
                return key;
    }
    PORTB = 0x08;
    switch (PORTB)
    {
            case 0x18:
                key = 0x0E;
                return key;
            case 0x28:
                key = 0;
                return key;
            case 0x48:  
                key = 0x0F;
                return key;
            case 0x88:  
                key = 0x0D;
                return key;
    }
    return key;
}

void getInputX()
{
    input_X = 0;
    key = 0xFF;
    while (key >= 0x0A)         //0x0A is the first value too large fir it to  be a digit 
        key = getKeypress();    //Loop checking input until the value is a digit
    input_X = (input_X << 4) + key;     //shifts X left 4 and puts new value in lower nibble
    //Value is now 0x0n for n being a digit on the keypad
    
    key = 0xFF;
    while (key >= 0x0A)
        key = getKeypress();
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
    input_Y = (input_Y << 4) + key;     //shifts Y left 4 and puts new value in lower nibble
    //Value is now 0x0n for n being a digit on the keypad
    
    key = 0xFF;
    while (key >= 0x0A)
        key = getKeypress();
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
    
    PORTC = 0b00000100;
    PORTD = lookupTable[Result & 0x0F];    
    //masks lower nibble and pulls encoded value from lookup table, sends to portD
    __delay_ms(10); //wait 10ms
    PORTC = 0b00001000;
    PORTD = lookupTable[(Result & 0xF0) >> 4];
    //masks upper nibble, shifts by 4, and pulls encoded value from lookup table, sends to portD
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
    
    //using port C2 + C3 as output to 7 seg digit switch
    TRISD = 0;   
    LATD = 0;
    PORTD = 0;
    ANSELD = 0;
    
    
    //loop
    while (1)
    {
        getInputX();
        getInputOperation();
        getInputY();
        Result = evaluate(input_X,input_Y,Operation);
        do 
        {
            display(Result);
        } while (getKeypress() != 0x0E || getKeypress() != 0x0F);    
            //wait for input asterisk or pound sign to go to next input
    }
    return;
    
}
