.include "inc/m8def.inc"

; PWM controlled test program for testing max speed (and correct function) of steppers 
; a TCA 3727 is used as H-bridge between uC and Stepper

; uses ADC with poti on it to find out the right speed for the stepper TODO

; r2020 used for saving actual stepping state
; 


; to step with 10 RPS (30 steps/R) needs a prescaler of 16 and should only count to 208
;                                                    or 64 and count to 52

; Clock is (meanwhile) 1 MHz

; Port Definitons

.equ Ph1Out = PB0  ; Direction output for Phase 1  (to TCA 3727)
.equ Ph1EOut= PB1  ; Enable Output Phase 1         (to TCA 3727)

.equ Ph2Out = PB3  ; Direction output for Phase 2  (to TCA 3727)
.equ Ph2EOut= PB4  ; Enable Output Phase 2         (to TCA 3727)

;;;;;;;;StartOfProgram;;;;;;;;;
.org 0x0000 ; start of Interrupt table
rjmp main   ; Reset
reti         ; External IRQ0
reti         ; External IRQ1
reti         ; Timer2 Compare Match
reti         ; Timer2 Overflow
reti         ; Timer1 Capture Event
reti         ; Timer1 Compare Match A
reti         ; Timer1 Compare Match B
reti         ; Timer1 Overflow
rjmp TIM0_OVF; Timer0 Overflow
reti         ; SPI Transfer complete
reti         ; USART, Rx complete
reti         ; USART, Data Register empty
reti         ; USART, Tx complete
reti         ; ADC Conversion complete
reti         ; EEPROM ready
reti         ; Analog Comperator
reti         ; TWI
reti         ; SPM ready

main: 
	; setup stack

	; setup pins
	clr r16
	out PORTB,r16
	ldi r17,(1<<Ph2Out)+(1<<Ph1Out)+(1<<Ph1EOut)+(1<<Ph2EOut)
	out DDRB,r17
	

	; setup Timer0
	ldi r17,(1<<CS01)+(1<<CS00)
	out TCCR0B,r17 ; set Prescaler to 64 (step with 61Hz--> (30S/R) 2RPs)

	ldi r17,(1<<TOIE0)
	out TIMSK,r17 ; enable Interrupt on Overflow

	; setup ADC  TODO (later)

	ldi r20,0x10
	sei

EOF_LOOP: rjmp EOF_LOOP ;everything done in interrupt routine

TIM0_OVF: ; used for stepping forward

	cpi r20,0x01
	breq t0o_s1
	cpi r20,0x02
	breq t0o_s2
	cpi r20,0x03
	breq t0o_s3
	

	reti
	
	
		
; vi:  set ai ts=5 sw=5:
; vim: set ai ts=5 sw=5:
;
