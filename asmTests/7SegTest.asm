.include "inc/tn25def.inc"
	
; PWM-controlled motor control circuit for ATtiny25
; which will print the actual RPM onto an 7-Seg-Display

; Registers r1 up to r15 are for parameters
; Registers r16 up to r20 are temporary and will NOT be saved in methods

; Register  r21 used for muxxing displays (address which will be printed next time)
; Register  r22 stores the low  part of the number to print on the display (!!BCD CODE!!(TODO))
; Register  r23 stores the high part   ---"---
; Clock is 8 MHz (cause of Multiplexxing)

.equ pwmOut   = PB0 ; Output of PWM-Control to Mosfet
.equ LCDDatOut= PB1 ; Output to Shiftregister (Data) (negative logic)
.equ LCDClkOut= PB2 ; Output to ShiftRegister (Clock)
.equ RPMIn    = PB3 ; Input from RPM-Sensor (rectangle)
.equ potiIn   = PB4 ; Poti-Input for PWM-Control (if changed ADC needs to be reconfigured (ADMUX)

;;;;;;;;StartOfProgram;;;;;;;;;
.org 0x0000 ; start of Interrupt table
rjmp main    ; Reset 
reti         ; External IRQ1
reti         ; Pin Change IRQ1
reti         ; Timer1 Compare Match A
reti         ; Timer1 Overflow
rjmp TIM0_OVF; Timer0 Overflow
reti         ; EEPROM ready
reti         ; Analog Comperator
reti         ; ADC Conversion complete
reti         ; Timer1 Compare Match B
reti         ; Timer0 Compare Match A
reti         ; Timer0 Compare Match B
reti         ; Watchdog Time Out
reti         ; USI START
reti         ; USI Overflow

main:
 ;; init everything ;;

	; initialize Stack 
	ldi r16,RAMEND
	out SPL,r16

	;initialize IO-Ports
	ldi r16,(1<<LCDDatOut)+(1<<LCDClkOut)+(1<<pwmOut)
	clr r17
	out PORTB,r17
	out DDRB,r16

	; setup timer0
	ldi r17,(1<<CS01)+(1<<CS00)
	out TCCR0B,r17 ; set Prescaler to 64 (update with 128kHz (32kHz for whole Display))
	ldi r17,(1<<TOIE0)
	out TIMSK,r17 ; enable Interrupt on Overflow

	; setup ADC
;	sbi DIDR0,potiIn ;disable Digital Input
;	ldi r16,(1<<ADEN)+(1<<ADIE)+(1<<ADATE)+(1<<ADPS2)+(1<<ADPS1)+(1<<ADPS0)
;	out ADCSRA,r16 ; enable adc, enable autotriggering & interrupt and set Prescaler to 128
;	ldi r16,0x02 ; select PB4(ADC2) as Input Pin of ADC
;	out ADMUX,r16
;	sbi ADCSRA,ADSC ; set it after everything other to be sure

	;load 7-Seg Lookup-Table into Y-Pointer 
	ldi YL,LOW(LCDCodes*2)
	ldi YH,HIGH(LCDCodes*2)

	ldi r21,0x10 ; load address of 1st Display

	ldi r23,0xFF ; testing purpose
	ldi r22,0xFF

	sei
 ;; init completed ;;

	ldi r16,0x31
	mov r1,r16
	rcall printLCD
	ldi r16,0xA0
	rcall printLCD
	
  EOF_LOOP: rjmp EOF_LOOP ;everything handled as interrupt

ADCC:  ;stores the digital value in the display register
	in r22,ADCL ; read out ADCL first (important cause otherwise it wouldn't be valid (and lock the ADC))
	in r23,ADCH
;	subi r22,0xFF ; invert (cause poti wired in the wrong way) (but needs a correction (log poti))
;	sbci r23,0x03 ; FIXME high byte has a offset of 1 if ADVal>0
;	neg r22
;	neg r23
	cbi ADCSRA,ADIF ;clear Interrupt flag
	reti

TIM0_OVF: ; muxxes all 4 Displays

;	inc r24
;	sbrs r24,7
;	rjmp T0_start
;	rcall ADCC
; 	clr r24

	T0_Start:
	movw ZL,YL

	cpi r21,0x10
	breq T0O_Mux0
	cpi r21,0x20
	breq T0O_Mux1
	cpi r21,0x40
	breq T0O_Mux2
	cpi r21,0x80
	breq T0O_Mux3
	rjmp T0O_Res

	T0O_Mux0: ;first Display (from left)
		mov r16,r23
		swap r16
		rjmp T0O_Mux_common
	T0O_Mux1: ;second Display
		mov r16,r23
		rjmp T0O_Mux_common
	T0O_Mux2:
		mov r16,r22
		swap r16
		rjmp T0O_Mux_common
	T0O_Mux3:
		mov r16,r22
		;rjmp T0O_Mux_common ;not needed
	T0O_Mux_common:
		andi r16,0x0F
		add ZL,r16
		clr r16
		adc ZH,r16
		lpm r16,Z
		mov r1,r16
		rcall printLCD
		mov r1,r21
		rcall printLCD
		lsl r21
		reti
	T0O_Res:
		ldi r21,0x10
		reti

printLCD: ; needs 1 Param: r1 for Data
	  ; prints 8 Bit out to the LCD
	clr r16
	cbi PORTB,LCDClkOut
	pLCD_loop:
		sbrs r1,7
		cbi PORTB,LCDDatOut
		sbrc r1,7
		sbi PORTB,LCDDatOut
		sbi PORTB,LCDClkOut
		lsl r1
		inc r16
		cbi PORTB,LCDClkOut
		cpi r16,8
		BRLO pLCD_loop
	ret
	
LCDCodes: ; everything inverted
	.db 0x03,0x9F	; 0,1
	.db 0x25,0x0D  ;2,3
	.db 0x99,0x49  ;4,5
	.db 0x41,0x1B  ;6,7
	.db 0x01,0x09  ; 8,9

	.db 0x11,0xC1  ; A,b
	.db 0xE5,0x85  ; c,d
	.db 0x61,0x71  ; E,F

	.db 0xFF,0xFF  ; Off 
; vim: set ts=2 sw=2 ai
