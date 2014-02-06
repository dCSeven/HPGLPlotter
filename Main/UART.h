
/** Simple UART library for interrupt driven communication
 * (and to crate a library for that (interrupt driven))
 * 
 * The uC is a ATMega8-16PU operating on 9.83040MHz (at the moment at 4MHz (internal oszillator))
 * 
**/

#define BAUD 9600
#define UART_BITS 8 //4<x<9
#define MYUBRR ((F_CPU/16/BAUD)-1)
#define RX_BUFFER_LENGTH 5 // sizeof(char)=1 therefore not needed


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include <stdlib.h>
#include <string.h>

volatile char *txBuffer;
volatile char *txPtr; // used in ISR USART_UDRE to store position of currently transmitting character in txBuffer

volatile char *rxBuffer;
volatile char *rxPtr; // used in ISR USART_RXC the same way as txPtr in UDRE ISR


void USART_init(unsigned int ubrr)
{
	// Set baud rate
	UBRRH = (unsigned char)(ubrr>>8)&0xFF;
	UBRRL = (unsigned char)ubrr&0xFF;
	// Enable receiver and transmitter
	UCSRB = (1<<RXEN)|(1<<TXEN);
	// Set frame format: 8data(UCSZx), 1stop bit(USBS) TODO check if it is right
	UCSRC = (1<<URSEL)|(0<<USBS)|((UART_BITS-5)<<UCSZ0);

	rxPtr=rxBuffer=(char *) malloc(RX_BUFFER_LENGTH);

	DDRD=(0<<PD0)|(1<<PD1);
}

void USART_transmit(char data)
{
	// Wait for empty transmit buffer
	while ( !( UCSRA & (1<<UDRE)) );
	// Put data into buffer, which sends the data
	UDR = data;
	UCSRB|=(1<<TXEN);
}

// this function enables the DataRegisterEmpty-Interrupt
// to process the data there
void USART_transmit_string (char *data)
{
	UCSRB&=~(1<<UDRIE); // disable Interrupt (USART UDRE)
	if(txBuffer)free((void *)txBuffer); //FIXME don't free previous message
	txPtr=txBuffer=data;
	UCSRB|=(1<<UDRIE)|(1<<TXEN);
}

// receive 1 Byte (blocking)
char USART_receive( void )
{
	// Wait for data to be received
	if(!(UCSRB & (1<<RXEN)))UCSRB|=(1<<RXEN);
	while ( !(UCSRA & (1<<RXC)) );
	// Get status bits
	if(UCSRA & ((1<<PE)|(1<<DOR)|(1<<PE)))return -1;
	// Get and return received data from buffer
	return UDR;
}

// if enabled rxbuf
void USART_enable_rx_interrrupt(void)
{
	UCSRB|=(1<<RXCIE);
}

void USART_disable_rx_interrupt(void)
{
	UCSRB&=~(1<<RXCIE);
}

//returns a copy of the received data and clears the rx buffer if it is not empty
char* USART_get_rx_buffer(void)
{
	int8_t len=rxPtr-rxbuf;
	if(len>0)
	{
		cli();
		char * chr= (char*) malloc(len);
		memcpy(chr,rxbuf,len)
		//TODO clear rx buffer

		sei();
		return chr;
	}
	return 0;
}

/* USART Data Register Empty */
ISR(USART_UDRE_vect)	// maybe use USART_TXC_vect for verifying transmission
{
	if(txPtr && *txPtr!='\0') 
	{
		UDR=*txPtr++; 
	}
	else // Communication done 
	{
//		__asm__("cli UCRSB,UDRIE");
//		__asm__("cli UCRSB,TXEN");
//	UCSRB&=~((1<<UDRIE)|(1<<TXEN));
		UCSRB&=~(1<<UDRIE);
		
		free((void*)txBuffer);
		txPtr=txBuffer=0;
	}
}

/* USART, Rx Complete
 * maybe alter to split strings on \n*/
ISR(USART_RXC_vect)
{
	if(rxPtr && (rxPtr-rxBuffer)<(RX_BUFFER_LENGTH-2)) // if at least 2 fields are free
	{
		//TODO check error state
		*rxPtr=UDR;
		rxPtr++;
	}else if (rxPtr) // Buffer Overflow on next call
	{
		*rxPtr=UDR;
		rxPtr++;
		//TODO Hardware Handshaking and clear Interrupt
		UCSRB&=~(1<<RXCIE);
	} //else probably clear Interrupt
}

// vi :set ai ts=2 sw=2:
// vim:set ai ts=2 sw=2 fdm=syntax:
