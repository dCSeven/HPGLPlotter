#include "Stepper.h"

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

void step(int16_t steps)
{
	stepsToDo+=steps;		
}
