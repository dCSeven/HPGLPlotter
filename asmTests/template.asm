.INCLUde "inc/tn25def.inc"
	
; PWM-controlled motor control circuit for ATtiny25
; which will print the actual RPM onto an 7-Seg-Display

; Registers r1 up to r15 are for parameters
; Registers r16 up to r20 are temporary and will NOT be saved in methods

; Clock will (probably) be 1MHz or lower (internal Oszillator)

.equ pwmOut   = PB0 ; Output of PWM-Control to Mosfet
.equ LCDDatOut= PB1 ; Output to Shiftregister (Data) (negative logic)
.equ LCDClkOut= PB2 ; Output to ShiftRegister (Clock)
.equ RPMIn    = PB3 ; Input from RPM-Sensor (rectangle)
.equ potiIn   = PB4 ; Poti-Input for PWM-Control

;;;;;;;;StartOfProgram;;;;;;;;;
.org 0x0000 ; start of Interrupt table
rjmp main    ; Reset 
rjmp EIRQ1   ; External IRQ1
reti         ; Pin Change IRQ1
reti         ; Timer1 Compare Match A
rjmp TIM1_OVF; Timer1 Overflow
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
	; set flags for interrupts
;	ldi r16,(1<<ISC00)+(1<<ISC01)
;	out MCUCR,r16
;	ldi r16,(1<<INT0)+(1<<PCIE)
;	out GIMSK,r16
	sei

	; initialize Stack 
	ldi r16,RAMEND
	out SPL,r16

	;initialize IO-Ports
	ldi r16,(1<<LCDDatOut)+(1<<LCDClkOut)+(1<<pwmOut)
	clr r17
	out PORTB,r17
	out DDRB,r16
 ;; init completed ;;


; vim: set ts=2 sw=2 ai
