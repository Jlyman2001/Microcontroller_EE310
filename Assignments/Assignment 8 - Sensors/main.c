/*
 * ---------------------
 * Title: Assignment 8: Interfacing with Sensors
 * ---------------------
 * Program Details:
 *  The purpose of this program is to use external sensors and peripherals
 *  to measure and respond to external stimulus
 * Inputs:  Photo resistors 1 and 2
 *          Alarm button
 *          Continue Button 
 * Outputs: 7 Segment Display
 *          Indicator LED
 *          Motor
 *          Buzzer
 * 
 * Setup    : C- Simulator
 * Date: Apr 25, 2024
 * File Dependencies / Libraries: 
 *  ./declarations.h
 *  ./functions.h
 *  ./Variables.h
 * 
 * 
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
//#include "../../../../../Program Files/Microchip/xc8/v2.46/pic/include/proc/pic18f47k42.h"
#include "C:\Program Files\Microchip\xc8\v2.46\pic\include\proc\pic18f47k42.h"      //xc8 2.45 on lab pc I used
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./declarations.h"
#include "./functions.h"
#include "./Variables.h"


void main() {
    init();
    //PORTBbits.RB4 = 1;
    while (1) 
    {
        firstInput = 0;
        secondInput = 0;
        
        firstInput = getPR1Input();
        Indicator_LED = 1;
        __delay_ms(1000);
        
        secondInput = getPR2Input();
        readCode = (firstInput << 4) + secondInput;
        if (readCode == SecretCode)
        {
            motor = 1;      //turn on motor to open door
            __delay_ms(1000);
            motor = 0;
        }
        else
        {
            buzzer = 1;      //beep buzzer
            __delay_ms(1500);
            buzzer = 0;
        }
        Indicator_LED = 0;      //reset LED
        __delay_ms(1000);

    }
}


