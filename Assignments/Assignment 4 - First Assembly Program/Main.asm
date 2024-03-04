;---------------------
; Title: Assignment 4 - Thermostat
;---------------------
; Program Details:
; 
; Inputs: Reference Temperature, Measured Temperature
; Outputs: PORTD
; Date: Feb 29, 2024
; File Dependencies / Libraries: None
; Compiler: xc8, 2.46
; Author: Josh Lyman
; Versions:
; v1.0: Program completed, seems to work with no bugs
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
refTempInput	    EQU	35
measuredTempInput   EQU	-24
;---------------------
; Program Constants
;---------------------
;---------------------
; Definitions
;---------------------

;Registers for storing values 
    refTemp	    EQU	0x20
    measuredTemp    EQU	0x21
    contReg	    EQU	0x22
    
    
;Registers for decimal value versions of numbers
    refTempDecLow		EQU  0x60
    refTempDecHigh		EQU  0x61
    refTempDecUpper		EQU  0x62
    measuredTempDecLow		EQU  0x70
    measuredTempDecHigh		EQU  0x71
    measuredTempDecUpper	EQU  0x72	
	
	
#define LED1 PORTD,1
#define LED2 PORTD,2
;---------------------
; Main Program
;---------------------
PSECT absdata,abs,ovrld ; Do not change
ORG 0 ;Reset vector
GOTO _initialize
ORG 0x0020 ; Begin assembly at 0020H

;This block represents the other software that would have run to put 
;values where we need them
_initialize:		    
    MOVLW   refTempInput
    MOVWF   refTemp
    MOVLW   measuredTempInput
    MOVWF   measuredTemp
    
    MOVLW   0b11111100	    ;Ensures output bits are set without impacting 
    ANDWF   TRISD	    ;other assignments on port D
    
    GOTO    _storeDecimals

    
;Stores decimal values of refTemp and measuredTemp in their respective registers
;Assumes all registers are initalized to zero beforehand
_storeDecimals:    
    MOVF    refTemp,0
 
    _refTensLoop:	    
    ADDLW   0b11110110	    ;Adds 2's complement of 10, effectively subtracting 
    BTFSC   WREG,7	    ;if negative, we've gone 1 too far
	GOTO    _refOnes
    INCF    refTempDecHigh  ;increment tens place
    GOTO    _refTensLoop    ;repeat
    
    _refOnes:
    ADDLW   10		    ;counteract going 1 too far by adding 10
    MOVWF   refTempDecLow   ;WREG is now between 0-9 and can be stored as ones
    
    
    
    MOVF    measuredTemp,0
    BTFSS   WREG,7	    ;if negative bit is not set, skip next block
	GOTO    _measuredTensLoop
    MOVLW   0xA
    MOVWF   measuredTempDecUpper ;If negative, put value 0xA in upper bit to encode 
				 ;negative sign on 7 segment (hypothetically)
    MOVF    measuredTemp,0			 
    NEGF    WREG
    
    _measuredTensLoop:	    ;merge flow from conditional
    ADDLW   0b11110110	    ;Adds 2's complement of 10, effectively subtracting 
    BTFSC   WREG,7	    ;if negative, we've gone 1 too far
	GOTO    _measuredOnes
    INCF    measuredTempDecHigh  ;increment tens place
    GOTO    _measuredTensLoop    ;repeat
    
    
    _measuredOnes:
    ADDLW   10		    ;counteract going 1 too far by adding 10
    MOVWF   measuredTempDecLow   ;WREG is now between 0-9 and can be stored as ones
    
    
    
    GOTO    _compare
    
    
    
;Compares     
_compare:
    MOVF    measuredTemp,0	;load the measured temp
    SUBWF   refTemp		;ref-measured -> W
				;if W > 0, too cold. W < 0, too hot
    BN	    _tooHot
    BZ	    _justRight
    ;if not too hot or just right, it is too cold
    MOVLW   0x01
    MOVWF   contReg
    BSF	    LED1
    GOTO _end
    
_tooHot:
    MOVLW   0x02
    MOVWF   contReg
    BSF	    LED2
    GOTO _end
    
    
_justRight:
    MOVLW   0x0
    MOVWF   contReg
     

_end:
    SLEEP
END



