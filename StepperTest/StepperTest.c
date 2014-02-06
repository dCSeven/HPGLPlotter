/**
 * Testing circuit for Steppers with usage of a TCA 3727
 * Ports are used as follows:
 uC  --> TCA 3727
 PD0 --> I10,I20
 PD1 --> I11,I21
 PD2 --> Phase1
 PD3 --> Phase2
 PD4 --> Inhibit

 PC0 --> Poti   (ADC0)

 * A stepper needs Input data in Grey-Code as follows:
 Ph1 Ph2 (every change is a phase change (no turning off))
 0   1    would be: 1
 1   1              3
 1   0              2  (6=0b110)
 1   1              3  (7=0b111)


 TODO make interrupt driven (needs a timer)
 TODO use clock 9.830400
**/

#define true (0==0)
#define false (0!=0)

#define I0 PB0
#define I1 PB1
#define Ph1 PB2
#define Ph2 PB3	// Ph1 and Ph2 needs to be like this
#define En PB4

#define Poti PC0

// maybe put following definitions in eeprom for quicker change
#define STEPS_PER_REV 30
#define MM_PER_REV    1.25

#define F_CPU 3686400

#define STEPS_PER_SEC //TODO

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>
//#include <avr/sleep.h>
//#include "StepperTest.h"

/*
struct Stepper
{
	bool i0:1;
	bool i1:1;
	unsigned char Ph:2;
	bool en:1;

} */

uint8_t adcPot=0;

ISR(ADC_vect)
{
	adcPot=ADCL; //read out ADCL first, cause otherwise the ADC would be locked and the result wouldn't be vaild
	adcPot=ADCH;
	ADCSRA&=~(1<<ADIF); //clear Interrupt flag
	reti();	
}

void adc_init(void) //XXX check if it works properly
{
	ADCSRA=(1<<ADEN)+(1<<ADIE)+(1<<ADFR)+(1<<ADPS2)+(1<<ADPS1)+(1<<ADPS0); // enable adc, enable autotriggering & interrupt and set Prescaler to 128
	ADMUX=(1<<REFS0)|(1<<ADLAR)|0x00; // set ref to AVcc, left adjust result and select PC0(ADC0) as Input Pin of ADC
	ADCSRA|=1<<ADSC; // set it after everything other to be sure
//	while(ADCSRA&(1<<ADSC));	//wait for the adc to come up
}

void main(void)
{

	//DDRD=(1<<I0)|(1<<I1)|(1<Ph1)|(1<Ph2)|(1<En);
	DDRB=0xFF;
	DDRC=(0<<Poti);
	adc_init();
	sei();
	
	unsigned char stepCode[4]={0,1,3,2}; // maybe a better idea could be realized
	
//MCUCR&=0x0F;
//MCUCR|=(1<<SE)|(1<<SM0);
	

	

//	PORTD=(0<<I0)|(0<<I1)|(1<En);	// enable and set max I


	while(true)
	{
		unsigned int revs=0;
		unsigned int steps=0;

		unsigned int i;
		register uint16_t delay=0;
		for(i=1;;i=0) //for debugging begin with 1
		{
			for(;i<4;i++)
			{
				PORTB=(stepCode[i]<<Ph1)|(0<<I0)|(0<<I1)|(1<<En);
//				sleep_cpu(); // for testing if ISR is called (FIXME cpu should wake up, but doesn't)

				delay=adcPot;
				_delay_us(10);
				while(delay>0)
				{
					_delay_us(10);
					delay=delay-1;
				}
			}
			steps++;
			if(steps==STEPS_PER_REV) // XXX may be -1
			{
				revs++;
				steps^=steps;
			}
		}
	}
}

// vim:set ts=2 sw=2 ai fdm=syntax: 
// vi: set ts=2 sw=2 ai:
