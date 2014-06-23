/**
 * Library for Steppers with usage of a TCA 3727
 * Ports are used as follows:
 uC  --> TCA 3727

 PB0 --> Stepper1 Phase1  //X_AXIS_BIT
 PB1 --> Stepper1 Phase2

 PB2 --> Stepper2 Phase1  //Y_AXIS_BIT
 PB3 --> Stepper2 Phase2

 PB4 --> Stepper3 Phase1  //Z_AXIS_BIT
 PB5 --> Stepper3 Phase2

 The PORT can be changed via STEPPORT macro


 * A stepper needs Input data in Grey-Code as follows:
 Ph1 Ph2
 0   0    would be: 0
 0   1              1
 1   1              3  
 1   0              2 


 DONE make it possible to step backwards
 DONE make usable as library (add some functions for stepping)
 DONE make usable for 3 Steppers (x,y,z)
 TODO make it possible to customize the stepper pins
 TODO make movement linear (that all steppers are finnished at the same time)
 TODO add function/variable to find out when the steppers have finnshed their movement
 TODO implement functions to convert between coordinates and steps

 XXX  use clock 9.830400
 XXX  use more unique function names
 XXX  try to unify STEPPORT STEPPIN and STEPDDR (for the configurator)
**/
#include "genDef.h"


#define STEPPORT PORTB
#define STEPPIN  PINB
#define STEPDDR  DDRB

/* not possible right now
#define X_AXIS_BIT 0
#define Y_AXIS_BIT 2
#define Z_AXIS_BIT 4
*/

// maybe put following definitions in eeprom for quicker change
#define STEPS_PER_REV 30
#define MM_PER_REV    1.25


//#define STEPS_PER_SEC 300 //XXX testing on real board necessary
#define STEPS_PER_SEC 56 // cause of testing board lower steprates possible

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

void stepper_init(void);

void stepx(int16_t steps);
void stepy(int16_t steps);
void stepz(int16_t steps);

// vim:set ts=2 sw=2 ai fdm=syntax: 
// vi :set ts=2 sw=2 ai:
