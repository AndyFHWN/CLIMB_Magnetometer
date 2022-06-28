//---------------------------------------------------------
// Title:		TWI.c
//
// Author:		Patrick Kappl
//
// Creation:	03.01.2015
//
// Description:	provides functions to use the TWI properly
//				no error handling implemented yet
//---------------------------------------------------------

#include "TWI.h"

// initializes the TWI bus with the bit rate defined in TWI.h
void TWIInit(void)
{
	TWSR &= ~((1<<TWPS1)|(1<<TWPS0));			// set prescaler = 1
	TWBR = TWI_BIT_RATE_DIV;					// set bit rate division factor
	TWCR |= 1<<TWEN;							// enable TWI
}

// deactivates the TWI
void TWIDeInit(void)
{
	TWCR = 0;
}

// reactivates the TWI but doesn't set prescaler or bit rate
void TWIReInit(void)
{
	TWCR |= 1<<TWEN;							// enable TWI
}

// send a start condition to enter master receiver or master transmitter mode
void TWISendStart(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);		// send start condition
}

// send a stop condition to end a transfer and wait till the stop condition
// has been executed on the bus
void TWISendStop(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);		// transmit stop condition
	while((TWCR & 1<<TWSTO) != 0);				// wait till TWSTO is cleared by hardware
}

// wait till the TWI has finished its current job (sending, receiving)
void TWIWaitForComplete(void)
{
	while((TWCR & (1<<TWINT)) == 0);
}

// send a byte in master transmitter mode
void TWISendByte(uint8_t data)
{
	TWDR = data;								// save data to the TWI data register
	TWCR = (1<<TWINT)|(1<<TWEN);				// begin to send
}

// receive a byte in master receiver mode
// ackFlag determines if ACK or NACK is sent after reception
void TWIReceiveByte(uint8_t ackFlag)
{
	if(ackFlag)												// if ackFalg is not 0
	{
		TWCR = (TWCR & TWCR_CMD_MASK)|(1<<TWINT)|(1<<TWEA);	// start receiving and send ACK
	}
	else
	{
		TWCR = (TWCR & TWCR_CMD_MASK)|(1<<TWINT);			// else, start receiving and send NACK
	}
}

// returns the TWI data register which contains the last byte transfered on the bus
// call this after TWIReceiveByte() and TWIWaitForComplete() to get the received byte
uint8_t TWIGetReceivedByte(void)
{
	return(TWDR);
}

// returns the TWI status register
uint8_t TWIGetStatus(void)
{
	return(TWSR);
}

// sends a data vector with given length in master transmitter mode to a slave with given address
// if sendStop = 0 no stop condition is sent after the data transmission, otherwise stop is sent
// this function is not interrupt based but uses waiting loops
// return error cases not implemented yet
uint8_t TWIMasterSendWait(const uint8_t address, const uint8_t *data, const uint8_t length, const uint8_t sendStop)
{
	uint8_t i;

	TWISendStart();								// enter master transmitter mode by sending start
	TWIWaitForComplete();
	TWISendByte((address<<1)|TWIM_WRITE);		// send the address of the slave + write
	TWIWaitForComplete();

	for(i = 0; i < length; i++)
	{
		TWISendByte(data[i]);					// send all bytes in the data vector
		TWIWaitForComplete();
	}

	if(sendStop != 0) TWISendStop();			// send stop after transmission if necessary

	return(0);									// return error code (not implemented yet)
}

// receives a data vector with given length in master receiver mode from a slave with given address
// if sendStop = 0 no stop condition is sent after the data reception, otherwise stop is sent
// this function is not interrupt based but uses waiting loops
// return error cases not implemented yet
uint8_t TWIMasterReceiveWait(const uint8_t address, uint8_t *data, const uint8_t length, const uint8_t sendStop)
{
	uint8_t i;

	TWISendStart();								// enter master transmitter mode by sending start
	TWIWaitForComplete();
	TWISendByte((address<<1)+TWIM_READ);		// send the address of the slave + read
	TWIWaitForComplete();

	for(i = 0; i < length-1; i++)				// for all bytes except the last one
	{
		TWIReceiveByte(1);						// send ACK after the received byte
		TWIWaitForComplete();
		data[i] = TWIGetReceivedByte();			// add the byte to the data vector
	}

	TWIReceiveByte(0);							// send NACK after the last received byte
	TWIWaitForComplete();
	data[i] = TWIGetReceivedByte();				// add the last byte to the data vector

	if(sendStop != 0) TWISendStop();			// send stop after reception if necessary

	return(0);									// return error code (not implemented yet)
}
