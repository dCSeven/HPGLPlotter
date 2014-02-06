/**
 * Hauptprogramm fuer Plottersteuerung
 * Kommandos(HPGL) werden per UART uebertragen
 * 
 * Used Ports:
 * PORTD:
 * PD0 Rx
 * PD1 Tx
 *
**/

#include "main.h"

void main (void)
{
	USART_init();	
	//USART_enable_rx_interrrupt();

	sei();

	while(true)
	{
		delay_ms(10);	
		step(atoi(USART_receive()));//FIXME make anything like that
	}

}
