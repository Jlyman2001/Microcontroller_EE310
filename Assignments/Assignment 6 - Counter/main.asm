;---------------------
; Title: 
;---------------------
; Program Details:
; 
; Inputs: 
; Outputs: 
; Date: 
; File Dependencies / Libraries: None
; Compiler: xc8, 2.46
; Author: Josh Lyman
; Versions:
; 
; Useful links:
; Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf
; PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4
; List of Instructions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf
;---------------------
; Initialization
;---------------------
#include "./AssemblyConfig.inc"
#include <xc.inc>
;---------------------
; Program Inputs
;---------------------
#define	Button1	PORTC,0	    ;button 1 = count up
#define	Button2	PORTC,1	    ;button 2 = count down

;---------------------
; Program Constants
;---------------------
DataOffset  EQU	0xE0

;---------------------
; Definitions
;---------------------
Digit	EQU	0x10
Loop1	EQU	0x11
Loop2	EQU	0x12

;---------------------
; Main Program
;---------------------
PSECT absdata,abs,ovrld ; Do not change
ORG 0 ;Reset vector
GOTO _start
ORG 0x0020 ; Begin assembly at 0020H


_start:	
    ;Initialize port D as outputs
    BANKSEL	PORTD
    CLRF	PORTD
    BANKSEL	LATD
    CLRF	LATD
    BANKSEL	ANSELD
    CLRF	ANSELD
    BANKSEL	TRISD
    CLRF	TRISD
    
    BANKSEL	PORTC
    CLRF	PORTC
    BANKSEL	LATC
    CLRF	LATC
    BANKSEL	ANSELC
    CLRF	ANSELC
    BANKSEL	TRISC
    MOVLW	0b00000011
    MOVWF	TRISC
    
_reset:    
    MOVLW   0
    MOVWF   Digit
    
    
    
_loop:
    
    CALL    _DisplayDigit
    CALL    _Delay
    CALL    _Delay
    MOVLW   0
    BTFSC   Button1	;buttons are active high with pulldown resistor
	ADDLW	1
    BTFSC   Button2
	ADDLW	2
    ADDLW   0xFD		;Cases for W: Both pushed, w=0. None pushed, W=-3
			;Button 1 pushed: W=-2.	Button 2 pushed, W=-1
    BZ	_reset		;if both are pushed, reset
    ADDLW   1		;None = -2, b1 = -1, b2 = 0
    BZ	_countDown	;if only button 2 is pushed, count down
    ADDLW   1		;none = -1, b1 = 0
    BZ	_countUp	; 
    GOTO    _loop	;no branches taken = no buttons pressed, don't count

    
_DisplayDigit:    
   MOVLW    DataOffset	    ;loads index of data block
   ADDWF    Digit,W	    ;adds offset based on digit select
   MOVWF    TBLPTRL	    
   TBLRD*		    ;reads from program memory
   MOVFF    TABLAT, PORTD   ;outputs to port D
    
   RETURN 0
    
    
    
_Delay:
    RETURN 0
    MOVLW   0	    ;Counter is decremented before any comparison so this = 256
    MOVWF   Loop1
    MOVLW   0
    MOVWF   Loop2   ;197.39 ms delay, will call twice rather than add 3rd loop
		    ;as it is more memory efficient
    _DelayLoop:
	DECFSZ	Loop1
	GOTO	_DelayLoop
	DECFSZ	Loop2
	GOTO	_DelayLoop
    RETURN 0	;Don't care about any changes to WREG or STATUS
	
_countUp:
    MOVF    Digit,0
    INCF    WREG
    ANDLW   0b00001111	;mask upper nibble to avoir index out of bounds
    MOVWF   Digit
    GOTO    _loop
    
_countDown:
    MOVF    Digit,0
    DECF    WREG
    ANDLW   0b00001111	;mask upper nibble to avoir index out of bounds
    MOVWF   Digit
    GOTO    _loop    
    

ORG 0xE0
    DB   0b00111111	;encodes 0    
    DB   0b00000110	;encodes 1    
    DB   0b01011011	;encodes 2    
    DB   0b01001111	;encodes 3    
    DB   0b01100111	;encodes 4    
    DB   0b01101101	;encodes 5    
    DB   0b01111101	;encodes 6    
    DB   0b00100111	;encodes 7    
    DB   0b01111111	;encodes 8    
    DB   0b01101111	;encodes 9    
    DB   0b01110111	;encodes A    
    DB   0b01111100	;encodes B    
    DB   0b00111001	;encodes C     
    DB   0b01011110	;encodes D    	
    DB   0b01111001	;encodes E      
    DB   0b01110001	;encodes F    	

