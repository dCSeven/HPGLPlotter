#include "Stepper.h"


/*
struct Stepper //TODO make use of this
{
	uint8_t curStep:2; // TODO check if overflow results in changing ph1Bit
	uint8_t ph1Bit :6;
	(*(volatile uint8_t *) port;
	int16_t stepsToDo;

};
*/

volatile const uint8_t stepCode[4]={0,1,3,2};
//volatile uint8_t curStep=0;
volatile uint8_t curStep[3]={0,0,0};

//volatile int16_t stepsToDo=0;
volatile int16_t stepsToDo[3]={0,0,0};


ISR(TIMER0_OVF_vect)
{
	TCNT0=255-((F_CPU/STEPS_PER_SEC)/1024);
	int i;
	for(i=0;i<3;i++)
	{
		if(!stepsToDo[i])return;
		uint8_t pb=STEPPIN&~(0x3<<(2*i)); // clear bits which are going to be changed

		if(stepsToDo[i]>0)
		{
			STEPPORT=pb|((stepCode[curStep[i]])<<(2*i));
		//	Ph2=(stepCode[curStep]>>1)&1;
			curStep[i]=(curStep[i]+1)%4;
			stepsToDo[i]--;
		}else{
			curStep[i]=(curStep[i]+3)%4;
			STEPPORT=pb|(stepCode[curStep[i]]<<(2*i));
			stepsToDo[i]++;
		}
	}
}

void counter_init(void)
{
	TCCR0=(1<<CS02)|(1<<CS00); // set prescaler to 1024
	TIMSK=(1<<TOIE0); // enable Interrupt on Overflow
}

void stepper_init(void)
{
	counter_init();
	STEPDDR=0x7F;     // make pins 0 to 5 to outputs for stepping XXX needs to be made dynamically
}

void stepx(int16_t steps)
{
	stepsToDo[0]+=steps;		
}
void stepy(int16_t steps)
{
	stepsToDo[1]+=steps;		
}
void stepz(int16_t steps)
{
	stepsToDo[2]+=steps;		
}

// vim:set ts=2 sw=2 ai fdm=syntax: 
// vi :set ts=2 sw=2 ai:
