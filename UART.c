//---------------------------------------------------------
// Title:		UART.c
//
// Author:		Thomas Riel, Patrick Kappl
//
// Creation:	29.06.2013
//
// Description:	provides functions to use UART0 and UART1
//				of an ATmega128/ATmega128L interrupt based
//				no error handling implemented yet
//---------------------------------------------------------

#include "UART.h"

// global variables
char volatile global_UART0String[DATA_REC_LEN + 2];
uint8_t volatile global_RX0Complete;				// 1 = string completely received
uint8_t volatile global_TX0Complete;				// 1 = string completely transmitted
char volatile global_UART1String[DATA_REC_LEN + 2];
uint8_t volatile global_RX1Complete;				// 1 = string completely received
uint8_t volatile global_TX1Complete;				// 1 = string completely transmitted

// global variables only for UART.c (not in UART.h)
uint8_t volatile UART0StrCount;
uint8_t volatile UART1StrCount;
uint8_t volatile TXSend0;									// current send position in string
uint8_t volatile TXLength0;									// length of data vector
uint8_t volatile *TXBuff0;
uint8_t volatile TXSend1;									// current send position in string
uint8_t volatile TXLength1;									// length of data vector
uint8_t volatile *TXBuff1;

// initialization of UART0 and UART1
void UARTInit(void)
{
	// initialize UART0
  	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);		// enable RX, TX and RX complete interrupt
  	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);				// mode: asynchronous, 8N1
	
  	UBRR0H = UBRR_VAL0 >> 8;						// set Baud rate
  	UBRR0L = UBRR_VAL0 & 0xFF; 
	
	// initialize UART1
  	UCSR1B = (1<<RXEN1)|(1<<TXEN1)|(1<<RXCIE1);		// enable RX, TX and RX complete interrupt
  	UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);				// mode: asynchronous, 8N1
	
  	UBRR1H = UBRR_VAL1 >> 8;						// set Baud rate
  	UBRR1L = UBRR_VAL1 & 0xFF;

	TXSend0  = 0;									// reset send position
	TXLength0 = 0;									// reset length
	TXSend1  = 0;									// reset send position
	TXLength1 = 0;									// reset length
	
	UART0StrCount = 0;								
	global_RX0Complete = 0;
	global_TX0Complete = 0;
	global_UART0String[DATA_REC_LEN] = '\0';

	UART1StrCount = 0;
	global_RX1Complete = 0;
	global_TX1Complete = 0;
	global_UART1String[DATA_REC_LEN] = '\0';
}

// disable receiver and transmitter of UART0
void UART0DeInit(void)
{
	// disable RX, TX, RX complete interrupt and data register empty interrupt
	UCSR0B &= ~((1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0)|(1<<UDRIE0));
}

// initializes UART0 like UARTInit() but without setting the Baud rate or changing the frame format
// use after UART0DeInit()
void UART0ReInit(void)
{
	TXSend0 = 0;									// reset send position
	UART0StrCount = 0;								// reset receive position
	global_RX0Complete = 0;							// 1 = string completely received
	global_TX0Complete = 0;							// 1 = string completely transmitted
	global_UART0String[DATA_REC_LEN] = '\0';
	UCSR0B |= ((1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0));	// enable RX, TX and RX complete interrupt
}

// disable receiver and transmitter of UART1
void UART1DeInit(void)
{
	// disable RX, TX, RX complete interrupt and data register empty interrupt
	UCSR1B &= ~((1<<RXEN1)|(1<<TXEN1)|(1<<RXCIE1)|(1<<UDRIE1));
}

// initializes UART1 like UARTInit() but without setting the Baud rate or changing the frame format
// use after UART1DeInit()
void UART1ReInit(void)
{
	TXSend1 = 0;									// reset send position
	UART1StrCount = 0;								// reset receive position
	global_RX1Complete = 0;							// 1 = string completely received
	global_TX1Complete = 0;							// 1 = string completely transmitted
	global_UART1String[DATA_REC_LEN] = '\0';
	UCSR1B |= ((1<<RXEN1)|(1<<TXEN1)|(1<<RXCIE1));	// enable RX, TX and RX complete interrupt
}

// sends a char vector with a specific length via UART0
void UART0PutS(uint8_t *s, const uint8_t length)
{
	TXBuff0 = s;									// copy string to buffer
	TXSend0 = 1;									// reset send position
	TXLength0 = length;								// set string length
	global_TX0Complete = 0;							// 1 = string completely transmitted
	
	UDR0 = TXBuff0[0];								// send first character
	UCSR0B |= (1<<UDRIE0);							// data register empty interrupt enable
}

// sends a char vector with a specific length via UART1
void UART1PutS(uint8_t *s, const uint8_t length)
{
	TXBuff1 = s;									// copy string to buffer
	TXSend1 = 1;									// reset send position
	TXLength1 = length;								// set string length
	global_TX1Complete = 0;							// 1 = string completely transmitted

	UDR1 = TXBuff1[0];								// send first character
	UCSR1B |= (1<<UDRIE1);							// data register empty interrupt enable
}

// ISR USART0 receive interrupt
ISR(USART0_RX_vect)
{
	uint8_t nextChar;

	nextChar = UDR0;										// get received char from buffer
	
	// if the string is completely received global_RX0Complete = 1 -> reject char
	if(global_RX0Complete == 0)
	{
		if (UART0StrCount == 0) 
		{
			if(nextChar == '$') 
			{
				global_UART0String[UART0StrCount] = nextChar;
				UART0StrCount++;
			}
			else
			{
				UART0StrCount = 0;
			}
		}
		else if (UART0StrCount == 1)
		{
			if(nextChar == 'A')
			{
				global_UART0String[UART0StrCount] = nextChar;
				UART0StrCount++;
			}
			else
			{
				UART0StrCount = 0;
			}			
		}
		else 
		{
			global_UART0String[UART0StrCount] = nextChar;		// add the received char to the string
			UART0StrCount++;									// and increment the array index
			if(UART0StrCount >= DATA_REC_LEN+2)
			{
				global_RX0Complete = 1;
				UART0StrCount = 0;
			}
		}
	}
}

// ISR USART1 receive interrupt
ISR(USART1_RX_vect)
{
	uint8_t nextChar;

	nextChar = UDR1;										// get received char from buffer
	
	// if the string is completely received global_RX1Complete = 1 -> reject char
	if(global_RX1Complete == 0)
	{
		if (UART1StrCount == 0)
		{
			if(nextChar == '$')
			{
				global_UART1String[UART1StrCount] = nextChar;
				UART1StrCount++;
			}
			else
			{
				UART1StrCount = 0;
			}
		}
		else if (UART1StrCount == 1)
		{
			if(nextChar == 'A')
			{
				global_UART1String[UART1StrCount] = nextChar;
				UART1StrCount++;
			}
			else
			{
				UART1StrCount = 0;
			}
		}
		else
		{
			global_UART1String[UART1StrCount] = nextChar;		// add the received char to the string
			UART1StrCount++;									// and increment the array index
			if(UART1StrCount >= DATA_REC_LEN+2)
			{
				global_RX1Complete = 1;
				UART1StrCount = 0;
			}
		}
	}
/*	uint8_t nextChar;

	nextChar = UDR1;										// get received char from buffer

	// if the string is completely received global_RX1Complete = 1 -> reject char
	if(global_RX1Complete == 0)
	{
		global_UART1String[UART1StrCount] = nextChar;		// add the received char to the string
		UART1StrCount++;									// and increment the array index
		if(UART1StrCount == DATA_REC_LEN)
		{
			global_RX1Complete = 1;
			UART1StrCount = 0;
		}
	}
*/
}

// ISR USART0 data register empty interrupt
ISR(USART0_UDRE_vect)
{
	// if the end of the string is not reached
    if(TXSend0 < TXLength0)
    {
		UDR0 = TXBuff0[TXSend0];					// send next char
		TXSend0++;									// increment send position
	}
	else
	{
		TXSend0 = 0;								// reset send position
		TXLength0 = 0;								// reset length
		global_TX0Complete = 1;						// indicate that the transmission is complete
		UCSR0B &= ~(1<<UDRIE0);						// disable data register empty interrupt
	}
}

// ISR USART1 data register empty interrupt
ISR(USART1_UDRE_vect)
{
    // if the end of the string is not reached
	if(TXSend1 < TXLength1)
    {
		UDR1 = TXBuff1[TXSend1];					// send next char
		TXSend1++;									// increment send position
	}
	else
	{
		TXSend1 = 0;								// reset send position
		TXLength1 = 0;								// reset length
		global_TX1Complete = 1;						// indicate that the transmission is complete
		UCSR1B &= ~(1<<UDRIE1);						// disable data register empty interrupt
	}
}


uint8_t ValidateChecksum( uint8_t CC )
{
	char *dataVector;
	uint8_t checksum = 0;
	
	if(CC == 1)
	{
		dataVector = (char*) global_UART1String;
	}
	else
	{
		dataVector = (char*) global_UART0String;
	}
	
	// calculate checksum
	for ( uint8_t n = 0; n <= DATA_REC_LEN; n++ )
	{
		checksum ^= dataVector[n];
	}
	
	// check last byte of dataVector
	return (checksum == dataVector[DATA_REC_LEN+1]);
	
}