/** Simple UART library for interrupt driven communication
 (and to crate a library for that (interrupt driven))
 
 The uC is a ATMega8-16PU operating on 9.83040MHz (at the moment at 4MHz (internal oszillator))
 
 uses PD0(rx) and PD1(tx)

 TODO import handshaking from testing library
 TODO link handshaking with enable/disable_rx_interrupt
 TODO check if a problem occurs, when a nullbyte is transmitted (will probably be)
 TODO if problem occured fix it

 XXX  examine if USART_TXC_vect needed for verifiing transmitted data
**/

#ifndef _HPGL_UART_
#define _HPGL_UART_

#include "genDef.h" // include genDef.h before everything other to get F_CPU

#define BAUD 9600
#define UART_BITS 8 //4<x<9
#define MYUBRR ((F_CPU/16/BAUD)-1)
#define RX_BUFFER_LENGTH 50 // sizeof(char)=1 therefore not needed

//#define HARD_FLOW_CONTROL //TODO implement in a correct manner (doesn't work with minicom)
//#define SOFT_FLOW_CONTROL //TODO implement in a correct manner

#define XON  (0x11)
#define XOFF (0x13)

#define FLOW_PORT PORTD
#define FLOW_DDR  DDRD
#define CTS       PD3
#define RTS       PD2

#if (defined HARD_FLOW_CONTROL) && (defined SOFT_FLOW_CONTROL)
#error ONLY one of HARD_FLOW_CONTROL and SOFT_FLOW_CONTROL is allowed 
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#include <avr/pgmspace.h>

#include <stdlib.h>
#include <string.h>

//function prototypes


void USART_init();

void USART_transmit(char data);
void USART_transmit_string (char *data);


char USART_receive(void);
void USART_enable_rx_interrrupt(void);
void USART_disable_rx_interrupt(void);
char* USART_get_rx_buffer(void);


#endif // _HPGL_UART

// vi :set ai ts=2 sw=2:
// vim:set ai ts=2 sw=2 fdm=syntax:
