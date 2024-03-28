/*
 * ---------------------
 * Title: Simple LED Blinking 
 * ---------------------
 * Program Details:
 *  The purpose of this program is to simply blink an LED 
 * Inputs: myDelay 
 * Outputs: RD0 (output)
 * Setup: C- Simulator
 * Date: Feb 24, 2023
 * File Dependencies / Libraries: It is required to include the 
 * Configuration Header File 
 * Compiler: xc8, 2.4
 * Author: Farid Farahmand
 * Versions:
 *      V1.0: Original
 * Useful links:  
 *      Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
 *      PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
 *      List of Instrcutions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 
 */

#include <xc.h> // must have this
//#include "../../../../../Program Files/Microchip/xc8/v2.40/pic/include/proc/pic18f46k42.h"
#include "C:/Program Files/Microchip/xc8/v2.46/pic/include/proc/pic18f47k42.h"
//#include "C:\Program Files\Microchip\xc8\v2.40\pic\include\proc\pic18f46k42"
#include ".\header1.h"
#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4



void demo (){
TRISD= 0b00000000;//sets PORTB as all outputs
PORTD= 0b00000000;//turns off PORTB outputs so that the LED is initially off

    while(1){
        PORTDbits.RD1=0;//turns on LED
        __delay_ms(500);//delays for 1 second
        //PORTD=0b00000011;//turns off LED
        PORTDbits.RD1=1;//turns on LED    
        __delay_ms(500);//delays for 1 second
        }
}
 


void sample1 ()
{
    unsigned char a,b,c;
    a = 0x2;
    b = 0x3;
    c = a + b;
    while(1);
    
}


void sample2 ()
{
    TRISD = 0b00000000;
    PORTD = 0b00000000;
    unsigned char z;
    
    for (z = 0; z < 20; z++)
    {
        PORTD = z;
    }
    while(1);
}

void MSDelay(unsigned int, unsigned int);

void sample3 () {   
    unsigned char outputValue;
    #define SevenSeg PORTB
    #define mybit PORTBbits.RB7
    /*
    asm("BANKSEL	PORTB") ; // This is not a standard approach! Try not to mix assembly and C
    asm("CLRF	PORTB") ;       //Init PORTB
    asm("BANKSEL	LATB") ;    //Data Latch
    asm("CLRF	LATB") ;
    asm("BANKSEL	ANSELB") ;
    asm("CLRF	ANSELB") ;      //digital I/O
    asm("BANKSEL	TRISB") ;
    asm("MOVLW	0b00000000") ;
    asm("MOVWF	TRISB") ;
    */
    PORTB = 0;
    LATB = 0;
    ANSELB = 0;
    TRISB = 0;
    
    while(1) // run forever
    {
        outputValue = 0x55; 
        SevenSeg = outputValue; 
        MSDelay(25,165);
        SevenSeg++;                 // just increment by one
        mybit = 1;                  // change bit 7 of PORTB
        MSDelay(25,165); 
    }     
} // end of main()

void main()
{
    unsigned char seed;
    PORTB = 0; LATB = 0; ANSELB = 0; TRISB = 0;
    PORTD = 0; LATD = 0; ANSELD = 0;
    TRISDbits.TRISD0 = 1;
    
    seed = 1;
    while(1)
    {
        while (PORTDbits.RD0 == 0)
        {
            seed++;
            if (seed == 7)
                seed = 1;
            PORTB = seed;
        }
    }
}

void MSDelay(unsigned int itime1,unsigned int itime2)
{
    unsigned int i,j;
    for (i = 0; i < itime1; i++)
        for (j = 0; j<itime2; j++);
}