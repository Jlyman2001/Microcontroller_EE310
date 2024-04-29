/* 
 * File:   Functions.h
 * Author: Josh
 *
 * Created on April 27, 2024, 1:11 PM
 */
#include <xc.h>
#ifndef FUNCTIONS_H
#define	FUNCTIONS_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "Variables.h"

    
// Defining Interrupt ISR 
void __interrupt(irq(IRQ_INT0),base(0x4008)) INT0_ISR(void)
{
    // Check if this was a button press interupt
    if (PIR0bits.IOCIF == 1)
    {
        IOCAF = 0;              //ignore any interrupt from port A
        IOCCF = 0;              //ignore any interrupt from port C
        IOCEF = 0;              //ignore any interrupt from port F
        IOCBF = (IOCBF & 0x1F); //ignore the top 3 bits of port B; unused
        
        //If RB0 pressed, reset
        if(IOCBFbits.IOCBF0)
        {
            asm("RESET");       
        }
        //If RB1 pressed, log keypress
        if(IOCBFbits.IOCBF1)
        {
            keyPressed = 0x01;     
        }
        //If RB2 pressed, log keypress
        if(IOCBFbits.IOCBF1)
        {
            keyPressed = 0x02;     
        }
        //If RB1 pressed, log keypress
        if(IOCBFbits.IOCBF3)
        {
            keyPressed = 0x03;     
        }
        //If RB2 pressed, log keypress
        if(IOCBFbits.IOCBF4)
        {
            keyPressed = 0x04;     
        }
        
        
    }

}

//Initialization Function
void init(void){
    //Port Initializations
    TRISB = 0xFF;       //Port B is all inputs
    ANSELB = 0;
    LATB = 0;
    
    TRISA = 0x01;       //Pin RA1 is analog input   
    ANSELA = 0x01;
    LATA = 0;
    PORTA = 0;
    
    TRISD = 0;          //Port D is all outputs
    ANSELD = 0;
    LATD = 0;
    PORTD = 0;
    
    
    
    
    //------Setup the ADC
    //DO: using ADCON0 set right justify
    ADCON0bits.FM = 1;      //right justify is 1  
    //DO: Using ADCON0 set ADCRC Clock
    ADCON0bits.CS = 0;      //using Fosc clock
    //DO: Set RA0 to input
    TRISAbits.TRISA0 = 1;
    //DO: Set RA0 to analog
    ANSELAbits.ANSELA0 = 1;     //1 is analog mode
    //DO: Set RA0 as Analog channel in ADC ADPCH
    ADPCH = 0;                  //0 is use ANA0
    //DO: set ADC CLOCK Selection register to zero
    ADCLK = 0;
    //DO: Clear ADC Result registers
    ADRESL = 0;
    ADRESH = 0;
    //DO: set precharge select to 0 in register ADPERL & ADPERH
    ADPREL = 0;
    ADPREH = 0;
    //DO: Set acquisition LOW and HIGH bytes to zero
    ADACQL = 0;
    ADACQH = 0;
    //DO: Turn ADC On on register ADCON0   
    ADCON0bits.ON = 1;
    
    //interrupt handling 
    INTCON0bits.IPEN = 1;
    INTCON0bits.GIEH = 1;
    INTCON0bits.GIEL = 1;
    PIE0bits.IOCIE   = 1;   //enable interrupt on change for inputs
    
    
    
    
    
    IVTBASEU = 0;
    IVTBASEH = 0x40;
    IVTBASEL = 0x08;
    
    


}

//ADC based random number generator
unsigned char ADC_Random(void){
    //Sample ADC input
    ADCON0bits.GO = 1;
    //Wait for conversion done
    while (ADCON0bits.GO);      
    //Return 2 least significant bits of ADC read value. 
    //These will be essentially random and will return a value from 0 to 3
    return (ADRESL & 0x03);    
}

//Test function to measure the randomness of ADC RNG function
//Used to perform test T1 and ensure no significant bias to generated numbers
void ADC_Test(void){
    char sample = 0;
    char Zeros = 0;
    char Ones = 0;
    char Twos = 0;
    char Threes = 0;
    char Errors = 0;
    for (int tests = 0; tests < 32; tests++)
    {
        sample = ADC_Random();
        __delay_ms(100);
        switch (sample)
        {
                case 0:
                    Zeros++;
                    break;
                case 1:
                    Ones++;
                    break;
                case 2:
                    Twos++;
                    break;
                case 3:
                    Threes++;
                    break;
                default:
                    Errors++;
                    break;
        }
    }
    //display number counts
    PORTD = (Zeros << 1);       //display number of zeros read
    while (!Button1);           //wait for button 1 to be pressed
    __delay_ms(200);            //delay
    PORTD = (Ones << 1);        //display number of ones read 
    while (!Button1);           //wait for button 1 to be pressed
    __delay_ms(200);            //delay
    PORTD = (Twos << 1);        //display number of twos read
    while (!Button1);           //wait for button 1 to be pressed
    __delay_ms(200);            //delay
    PORTD = (Threes << 1);      //display number of threes read
    while (!Button1);           //wait for button 1 to be pressed
    __delay_ms(200);            //delay
    PORTD = (Errors << 1);      //display number of errors detected
    while (!Button1);           //wait for button 1 to be pressed
    __delay_ms(200);            //delay
    //beep to indicate program done
    Buzzer = 1;                 //turn on buzzer
    __delay_ms(100);            //delay
    Buzzer = 0;                 //turn off buzzer
    return;
    
}

//waits for input caused by interrupt handle function.
//if button is pressed, lights up LED for button and plays tone
void wait_for_input()
{
    keyPressed = 0;
    //wait for keyPressed to update from interrupt function
    while (!keyPressed);  
    displayValue(keyPressed);
    
    return;
}

//game over subroutine; flashes LEDs and plays song 
void gameOver(void)
{
    
}

//plays given frequency note for specified duration using timers
void playTone(unsigned char frequency, unsigned char duration)
{
    //TODO: learn this 
    return;
}

//takes an input from 1-4 and lights up LED for that input and plays a tone
void displayValue(unsigned char value)
{
    switch (value)
    {
        case 0x01:
            Button1LED = 1;
            playTone(0x01, 0x00);//placeholder; replace with correct value input
            Button1LED = 0;
            break;
        case 0x02:
            Button2LED = 1;
            playTone(0x02, 0x00);//placeholder; replace with correct value input
            Button2LED = 0;            
            break;    
        case 0x03:
            Button3LED = 1;
            playTone(0x03, 0x00);//placeholder; replace with correct value input
            Button3LED = 0;
            break;
        case 0x04:
            Button4LED = 1;
            playTone(0x04, 0x00);//placeholder; replace with correct value input
            Button4LED = 0;
            break;   
        default:
            break;
    }
}

#ifdef	__cplusplus
}
#endif

#endif	/* FUNCTIONS_H */

