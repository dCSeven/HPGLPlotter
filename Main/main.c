/**
 * Hauptprogramm fuer Plottersteuerung
 * Kommandos(HPGL) werden per UART uebertragen
 * 
 * Used Ports:
 * PORTD:
 * PD0 Rx
 * PD1 Tx
 * 
 * PORTB:
 * PD0 S1Ph1 //Stepper 1 Phase 1
 * PD1 S1Ph2
 * PD2 S2Ph1
 * PD3 S2Ph2
 * PD4 S3Ph1
 * PD5 S3Ph2
 *
 *
 * TODO implement interpreter for HPGL commands
 * XXX  split todo above in some more useful ones
**/

#include "main.h"

void main (void)
{
//	USART_init();	
//	USART_enable_rx_interrrupt();
	stepper_init();

	sei();

	while(true)
	{
		_delay_ms(1000);	
		
		stepx(STEPS_PER_SEC-1);
		stepy(-STEPS_PER_SEC/2);
		stepz(STEPS_PER_SEC/4);
/*	
		char * ptr=rxBuffer;
		uint8_t flags=0;
		uint16_t number=0;
		for(;ptr<=rxPtr;ptr++)
		{
			if(!isdigit(*ptr))
			{
				if(flags==1)break;
				continue;
			}
			flags=1;
			number=number*10+(ptr-'0');
			
			//TODO wait for new data
*/
	}

//		step(atoi(USART_receive())); // XXX make anything like that
}


// vim:set ts=2 sw=2 ai fdm=syntax: 
// vi :set ts=2 sw=2 ai:
