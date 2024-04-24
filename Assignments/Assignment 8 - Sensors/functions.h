/* 
 * File:   functions.h
 * Author: student
 *
 * Created on April 23, 2024, 11:36 AM
 */

#ifndef FUNCTIONS_H
#define	FUNCTIONS_H

#ifdef	__cplusplus
extern "C" {
#endif

//void ADC_Init(void)
//{
//    //------Setup the ADC
//    //DO: using ADCON0 set right justify
//    ADCON0bits.FM = 1;      //right justify is 1  
//    //DO: Using ADCON0 set ADCRC Clock
//    ADCON0bits.CS = 0;      //using Fosc clock
//    //DO: Set RA0 to input
//    TRISAbits.TRISA0 = 1;
//    //DO: Set RA0 to analog
//    ANSELAbits.ANSELA0 = 1;     //1 is analog mode
//    //DO: Set RA0 as Analog channel in ADC ADPCH
//    ADPCH = 0;                  //0 is use ANA0
//    //DO: set ADC CLOCK Selection register to zero
//    ADCLK = 0;
//    //DO: Clear ADC Result registers
//    ADRESL = 0;
//    ADRESH = 0;
//    //DO: set precharge select to 0 in register ADPERL & ADPERH
//    ADPREL = 0;
//    ADPREH = 0;
//    //DO: Set qcquisition LOW and HIGH bytes to zero
//    ADACQL = 0;
//    ADACQH = 0;
//    //DO: Turn ADC On on register ADCON0   
//    ADCON0bits.ON = 1;
//}

void init(void)
{
    //Port initilizations
    //init port D as outputs
    TRISD = 0;
    ANSELD = 0;
    LATD = 0;
    PORTD = 0;
    
    //init port A as outputs
    TRISA = 0;
    ANSELA = 0;
    LATA = 0;
    PORTA = 0;
    
    //init port B0-3 as input, port b4 as out (motor)
    TRISB = 0x0F;
    ANSELB = 0;
    LATB = 0;
    PORTB = 0;
    
    //interrupt handling 
    INTCON0bits.IPEN = 1;
    INTCON0bits.GIEH = 1;
    INTCON0bits.GIEL = 1;
    INTCON0bits.INT0EDG = 0;    //rising edge interrupt on int0
    IPR1bits.INT0IP = 0;
    PIE1bits.INT0IE = 1;
    PIR1bits.INT0IF = 0;
    IVTBASEU = 0;
    IVTBASEH = 0x40;
    IVTBASEL = 0x08;
    displayNumber(0);
}

// Defining Interrupt ISR 
void __interrupt(irq(IRQ_INT0),base(0x4008)) INT0_ISR(void)
{
        // Check if interrupt flag for INT0 is set to 1 - (note INT0 is your input)
    __delay_ms(100);
    if (PIR1bits.INT0IF == 1)
    {
            // if so, do something
        for (unsigned char beeps = 0; beeps < 2; beeps++)
        {
            PORTDbits.RD7 = !PORTDbits.RD7;
            //asm("NOP");
            __delay_ms(250);
            // e.g,blink an LED connected to  PORTDbits.RD0 for 10 times with a delay of __delay_ms(250)
        }
    PIR1bits.INT0IF = 0;  
    PORTDbits.RD7 = 0;
    }
        // always clear the interrupt flag for INT0 when done
        // turn off the led on PORTDbits.RD0 
}

unsigned char getPR1Input()
{
    unsigned char count = 0;
    displayNumber(count);
    while (1)  //allow input until button pressed
    {
        //wait for RB1 (first PR) to be low (active low)
        while (PORTBbits.RB1)
        {
            //__delay_ms(100);
            if (PORTBbits.RB3)
                return count;
        }
        count++;
        displayNumber(count);
        __delay_ms(1000);
    }
}
unsigned char getPR2Input()
{
    unsigned char count = 0;
    displayNumber(count);
    while (1)  //allow input until button pressed
    {
        //wait for RB2 (second PR) to be low (active low)
        while (PORTBbits.RB2)
        {
            //__delay_ms(100);
            if (PORTBbits.RB3)
                return count;
        }
        count++;
        displayNumber(count);
        __delay_ms(1000);
    }
}
void displayNumber(unsigned char number)
{
    if (number > 9)
    {
        number = 0;
    }
    unsigned char lookupTable [10]  = { 
    //xGFEDCBA
    0b00111111,	
	0b00000110,	
	0b01011011,	
	0b01001111,	
	0b01100110,	
	0b01101101,	
	0b01111101,	
	0b00000111,	
	0b01111111,	
	0b01101111
    };
    PORTD = lookupTable[number];
            
}

#ifdef	__cplusplus
}
#endif

#endif	/* FUNCTIONS_H */

