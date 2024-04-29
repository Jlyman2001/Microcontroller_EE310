/* 
 * File:   Variables.h
 * Author: Josh
 *
 * Created on April 27, 2024, 1:08 PM
 */
#include <xc.h>
#ifndef VARIABLES_H
#define	VARIABLES_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    
//Inputs
#define ResetINT    PORTBbits.RB0       //Reset button, interrupt attached
#define Button1     PORTBbits.RB1       //Input button 1 for gameplay
#define Button2     PORTBbits.RB2       //Input button 2 for gameplay
#define Button3     PORTBbits.RB3       //Input button 3 for gameplay
#define Button4     PORTBbits.RB4       //Input button 4 for gameplay
#define AnalogIn    PORTAbits.RA0       //Analog input to ADC
//Outputs
#define Buzzer      PORTDbits.RD0       //Active buzzer output for beeping
#define Speaker     PORTDbits.RD5       //Buzzer output for playing tones
#define Button1LED  PORTDbits.RD1       //LED associated with button 1
#define Button2LED  PORTDbits.RD2       //LED associated with button 2
#define Button3LED  PORTDbits.RD3       //LED associated with button 3
#define Button4LED  PORTDbits.RD4       //LED associated with button 4   

//Program Variables
//Maximum length of game before player wins
#define SEQUENCE_LENGTH 4  
//Preallocated space for sequence of buttons, limited to SEQUENCE_LENGTH long
unsigned char Sequence [SEQUENCE_LENGTH];
//variable to track which button in the sequence the player is on
unsigned char highestValue;
unsigned char keyPressed;
unsigned char index;


// Fosc  frequency for _delay()  library
#define _XTAL_FREQ 4000000                 
#define FCY    _XTAL_FREQ/4

#ifdef	__cplusplus
}
#endif

#endif	/* VARIABLES_H */

