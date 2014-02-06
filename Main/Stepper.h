/**
 * Library for Steppers with usage of a TCA 3727
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
 TODO make it possible to step backwards
 TODO use more unique function names
 TODO make usable as library (add some functions for stepping)
 TODO make usable for 3 Steppers (x,y,z)

**/
#include "genDef.h"

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

volatile int16_t stepsToDo=0;
//volatile int16_t stepsToDo[3]={0,0,0};

void counter_init(void);

// vim:set ts=2 sw=2 ai fdm=syntax: 
// vi :set ts=2 sw=2 ai:
