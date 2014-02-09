/**
 * Testing circuit for Steppers with usage of a TCA 3727
 * Ports are used as follows:
 uC  --> TCA 3727
 PB0 --> Phase1
 PB1 --> Phase2

 * A stepper needs Input data in Grey-Code as follows:
 Ph1 Ph2
 0   0    would be: 0
 0   1              1
 1   1              3  
 1   0              2 


 TODO use clock 9.830400
**/

#define F_CPU 4000000

#define true (0==0)
#define false (0!=0)

#define Ph1 PB0
#define Ph2 PB1

// maybe put following definitions in eeprom for quicker change
#define STEPS_PER_REV 30
#define MM_PER_REV    1.25


//#define STEPS_PER_SEC 300 //XXX testing necessary
#define STEPS_PER_SEC 56 // cause of testing board lower steps possible

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//#include <inttypes.h>
//#include "StepperTest.h"

/*
struct Stepper
{
	bool i0:1;
	bool i1:1;
	unsigned char Ph:2;
	bool en:1;

} */

volatile const uint8_t stepCode[4]={0,1,3,2};
volatile uint8_t curStep=0;
//volatile uint8_t curStep[3]={0,0,0}

volatile uint16_t stepsToDo=0;
//volatile uint16_t stepsToDo[3]={0,0,0};

ISR(TIMER0_OVF_vect)
{
	TCNT0=255-((F_CPU/STEPS_PER_SEC)/1024);
	if(stepsToDo)
	{
		PORTB=stepCode[curStep];
	//	Ph2=(stepCode[curStep]>>1)&1;
		curStep=(curStep+1)%4;

		stepsToDo--;
	}

}

void counter_init(void)
{
	TCCR0=(1<<CS02)|(1<<CS00); // set prescaler to 1024
	TIMSK=(1<<TOIE0); // enable Interrupt on Overflow
}


void main(void)
{

	//DDRD=(1<<I0)|(1<<I1)|(1<Ph1)|(1<Ph2)|(1<En);
	DDRB=(1<<Ph1)|(1<<Ph2);
	counter_init();

	stepsToDo=10;

	sei();
	

//	PORTD=(0<<I0)|(0<<I1)|(1<En);	// enable and set max I


	while(true)
	{
		_delay_ms(800);
		stepsToDo+=30;
	}
}

// vim:set ts=2 sw=2 ai fdm=syntax: 
// vi: set ts=2 sw=2 ai:
