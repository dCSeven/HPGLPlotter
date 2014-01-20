/** Simple test class for uart/usart communication
 * (and to crate a library for that (interrupt driven))
 * 
 * The uC is a ATMega8-16PU operating on 9.83040MHz
 * 
**/
 
//#define F_CPU 9830400
#define F_CPU 4000000

#define BAUD 9600
#define UART_BITS 8 //4<x<9
//#define BAUD 19200
#define MYUBRR ((F_CPU/16/BAUD)-1)
#define RX_BUFFER_LENGTH 4 // sizeof(char)=1 therefore not needed

#define true (0==0)
#define false (0!=0)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include <stdlib.h>
#include <string.h>


enum USART_Error 
{
	err_success=0,
	err_frame, // represents 1st stop bit error (out of sync/...)
	err_dataOverrun, // data loss due to Reciever buffer full condition
	err_parity
};

struct USART_Frame //TODO use in methods
{
	unsigned char data;
	enum USART_Error err;
};

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
	txPtr=txBuffer=memcpy(malloc(strlen(data)),data,strlen(data)); //XXX maybe malloc and memcpy is not necessary or wanted
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

void main(void)
{
	//init everything
	USART_init(MYUBRR);
	DDRD=(0<<PD0)|(1<<PD1);
	DDRB=(1<<PB0);
	
	rxBuffer=malloc(RX_BUFFER_LENGTH);

//	char* str1=malloc(20*sizeof(char));
//	strcpy_P(str1, PSTR ("Hello UART World"));

	//at last enable interrupts
	sei();
//	USART_transmit_string(str1);

	USART_transmit_string("Hello UART World\r\n"); //FIXME Strings  should be appended not removed
	USART_transmit_string("Print some statments to fill the buffer:\r\n");
	PORTB=(1<<PB0);

	uint8_t cycleEven=0;

//	UCSRB|=(1<<RXCIE);

	while(true)
	{
		_delay_ms(100);	
//		USART_transmit((char)46);
//		USART_transmit((cycleEven)?~'c':'\n');

		USART_transmit(*rxPtr);
		if((rxPtr-rxBuffer)>=(RX_BUFFER_LENGTH/2)) //FIXME any error in this call
		{
			rxPtr='\0'; // terminate String
			USART_transmit_string(rxBuffer);
			rxPtr=rxBuffer;
			memset((void*)rxBuffer,0,RX_BUFFER_LENGTH/2); //could be interrupted
		}
		PORTB=(cycleEven<<PB0);
		cycleEven=(cycleEven+1)%2;
	}
}

// vi:set ai ts=2 sw=2 fdm=syntax:
