/* 
 * File:   main.c
 * Author: Josh
 *
 * Created on April 27, 2024, 12:52 PM
 */
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "Functions.h"
#include "Variables.h"  
#include "Declarations.h"
#include "C:\Program Files\Microchip\xc8\v2.46\pic\include\proc\pic18f47k42.h"      

/*
 * 
 */
void main() 
{
    init();
    /*
     * Pseudocode:
     * Generate fist button
     *  Play tone associated with button
     *  Light up LED associated with button
     * Wait for input
     *  Play tone associated with input button
     *  Light up LED associated with input button  
     * If input is not correct, go to lose
     * If input is correct, generate new button and append to end of list
     * Proceed through list checking
     */
    
    
    
           
    
    for (highestValue = 0; highestValue < SEQUENCE_LENGTH; highestValue++)
    {
//-------------------Generate Value-----------------------       
        //generate value for current cell
        Sequence[highestValue] = ADC_Random();
        
//-------------------Play list-------------------------        
        //loop through entire list to play tones and light LEDs
        for (char index = 0; index <= highestValue; index++)
        {
            //calls display value function to indicate correct input
            displayValue(Sequence[index]);
            //delay for half a second before next plays
            __delay_ms(500);
        }
    
//------------------Player Input        
        //loop through every element from first one to current limit for input
        for (char index = 0; index <= highestValue; index++)
        {
            //wait for player to press button
            wait_for_input();
            //if not right button, player loses
            if (keyPressed != Sequence[index])
            {
                gameOver();
            }
        }
        
        
        
        
    }
    gameWin();
}

