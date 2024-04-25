/* 
 * File:   Variables.h
 * Author: Josh
 *
 * Created on April 25, 2024, 10:04 AM
 */

#ifndef VARIABLES_H
#define	VARIABLES_H

#ifdef	__cplusplus
extern "C" {
#endif
unsigned char SecretCode = 0x12;
unsigned char firstInput, secondInput, readCode;
#define continueButton PORTBbits.RB3
#define Photoresist1 PORTBbits.RB1
#define Photoresist2 PORTBbits.RB2
#define Indicator_LED PORTBbits.RB5
#define motor PORTAbits.RA5
#define buzzer PORTDbits.RD7


#ifdef	__cplusplus
}
#endif

#endif	/* VARIABLES_H */

