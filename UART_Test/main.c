/** Simple test class for uart communication
 * (and to crate a library for that (interrupt driven))
 * 
 * The uC is a ATMega8-16PU operating on 9.83040MHz
 * TODO add ISR for External Interrupt 1 for handling RTS
 * XXX if done remove defines for flow control (FLOW_PORT,FLOW_DDR(,CTS,RTS))
**/
 
//#define F_CPU 9830400
//#define F_CPU 4000000
#define F_CPU 12000000

//#define BAUD 19200
#define BAUD 9600
#define UART_BITS 8 //4<x<9
#define MYUBRR ((F_CPU/16/BAUD)-1)
#define RX_BUFFER_LENGTH 5 // sizeof(char)=1 therefore not needed

#define HARD_FLOW_CONTROL //TODO implement in a correct manner (doesn't work with minicom)
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

#define true (0==0)
#define false (0!=0)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

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

/* TODO use a kind of bitfield (like this one) for flow control
volatile struct
{
 uint8_t rx_turn_off:1;
 uint8_t rx_turn_on:1;
 uint8_t rx_is_turned_off:1;
 uint8_t tx_is_turned_off:1;
} flow_flags;
*/

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
#ifdef HARD_FLOW_CONTROL
		//TODO Hardware Handshaking 
		FLOW_PORT&=~(1<<CTS);

#elif defined SOFT_FLOW_CONTROL 
/*		if(txBuffer)
		{
			if(txPtr!=txBuffer)
			{
				*--txPtr=XOFF; // only if buffer not full
			}else
		}
		else
		{
			txBuffer=txPtr=(char *)malloc(2);
			*txBuffer=XOFF;
			*(txBuffer+1)='\0';
		}
*/	
		UCSRB|=(1<<UDRIE)|(1<<TXEN);
		//maybe clear interrupt too
#else // no flow control defined
		UCSRB&=~(1<<RXCIE); // clear interrupt to protect from buffer overflow (XXX find better solution)
#endif
	} //else clear interrupt (nothing to do)
		UCSRB&=~(1<<RXCIE);
}

void main(void)
{
	//init everything
	USART_init(MYUBRR);
	DDRD=(0<<PD0)|(1<<PD1);
	DDRB=(1<<PB0);
#ifdef HARD_FLOW_CONTROL
	FLOW_DDR|=(0<<RTS)|(1<<CTS);
#endif
	
	rxBuffer=malloc(RX_BUFFER_LENGTH);

	//at last enable interrupts
	sei();

	USART_transmit_string("Hello UART World\r\n"); //FIXME Strings  should be appended not removed
	USART_transmit_string("Print some statments to fill the buffer:\r\n");

	PORTB=(1<<PB0);

	uint8_t cycleEven=0;

	UCSRB|=(1<<RXCIE);

	uint8_t rxLen=0; //stores length of received string
	while(true)
	{
		_delay_ms(10);	
//		USART_transmit((char)46);
//		USART_transmit((cycleEven)?~'c':'\n');

//		USART_transmit(*rxPtr);

		if(rxBuffer && (rxLen=(rxPtr-rxBuffer))) // if buffer exists and data is received
		{
			cli(); //disable interrupts FIXME maybe do any other thing, cause it halts everything (also Stepper Movement)
			*rxPtr='\0'; // terminate String
			
			USART_transmit_string(strcpy(malloc(rxLen),(const char *)rxBuffer));
			rxPtr=rxBuffer;
			memset((void*)rxBuffer,0,rxLen-1); // clear buffer (cause of string termination we can say rxLen-1)
#ifdef HARD_FLOW_CONTROL
			FLOW_PORT|=(1<<CTS);
#elif SOFT_FLOW_CONTROL
			if(txBuffer && (txPtr-txBuffer))*--txPtr=XON;
			else if(txBuffer) // buffer full
			{
				char* str=malloc(strlen(txBuffer+1)); //realloc could also be used (but needs more time but maybe not as much memory) XXX find out if it would be better in this case (buffer >RAM/2)
				*str=XON;
				strcpy(str+1,txBuffer);
				free(txBuffer);
				txBuffer=txPtr=str;
			}else // no buffer there
			{
				char* str=malloc(sizeof(char));
				*str=XON;
				USART_transmit_string(str);
			}
#else // NO FLOW CONTROL
		UCSRB|=(1<<RXCIE); // enable receiver again
#endif
			sei(); // enable interrupts again
		}

		PORTB=(cycleEven<<PB0);
		cycleEven=(cycleEven+1)%2;
	}
}

// vi:set ai ts=2 sw=2 fdm=syntax:
