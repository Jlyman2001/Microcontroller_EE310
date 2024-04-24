/*
 * The purpose of this program is demonstrate How ADC operates. 
 * Conenct your input to RA0. 
 * Complete the code by modifying all the places identified by "DO:"
 * Use a port to represent the input voltage in binary.  
 * Author: Josh Lyman
 */


#include <xc.h> // must have this
//#include "../../../../../Program Files/Microchip/xc8/v2.46/pic/include/proc/pic18f47k42.h"
#include "C:\Program Files\Microchip\xc8\v2.45\pic\include\proc\pic18f47k42.h"      //xc8 2.45 on lab pc I used
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./declarations.h"
#include "./functions.h"


unsigned char SecretCode = 0x12;
unsigned char firstInput, secondInput, readCode;
void main() {
    init();
    //PORTBbits.RB4 = 1;
    while (1) 
    {
        firstInput = 0;
        secondInput = 0;
        
        firstInput = getPR1Input();
        PORTBbits.RB5 = 1;
        __delay_ms(1000);
        
        secondInput = getPR2Input();
        readCode = (firstInput << 4) + secondInput;
        if (readCode == SecretCode)
        {
            PORTAbits.RA5 = 1;      //motor
            __delay_ms(1000);
            PORTAbits.RA5 = 0;
        }
        else
        {
            PORTDbits.RD7 = 1;      //beep buzzer
            __delay_ms(1500);
            PORTDbits.RD7 = 0;
        }
        PORTBbits.RB5 = 0;      //reset LED
        __delay_ms(1000);

    }
}


