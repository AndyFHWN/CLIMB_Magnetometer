/*
 * SPI.c
 *
 * Created: 01/10/2021 13:01:25
 *  Author: Andreas Stren
 */ 

#include "SPI.h"

// Globals used only in SPI
uint8_t volatile byte_send;
uint8_t volatile byte_receive;
uint8_t volatile *data;
uint8_t volatile amount;
uint8_t volatile i;

// Initialize SPI
void SPIInit(void)
{
	DDRB |= (1<<MOSI)|(1<<SCK)|(1<<SS);				// set MOSI, SCK, SS as output
	DDRB &= ~(1<<MISO);								// set MISO as input
	PORTB |= (1<<SS);								// set SS high
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1)|(1<<SPR0);	// enable SPI, Master, clock rate fck/128, MSB
} 

void SPIMode0(void)
{
	SPCR &= ~(1<<CPOL) & ~(1<<CPHA);
}

void SPIMode1(void)
{
	SPCR &= ~(1<<CPOL);
	SPCR |= (1<<CPHA);
}

void SPIMode2(void)
{
	SPCR |= (1<<CPOL);
	SPCR &= ~(1<<CPHA);
}

void SPIMode3(void)
{
	SPCR |= (1<<CPOL)|(1<<CPHA);
}

// Disable SPI if enabled
void SPIDeinit(void)
{
	if ((SPCR & (1<<SPE)) == 1)				// if SPI enabled
	{
		SPCR &= ~(1<<SPE);					// disable
	}
}

// Enable SPI if disabled
void SPIReinit(void)
{
	if ((SPCR & (1<<SPE)) == 0)				// if SPI disabled
	{
		SPCR |= (1<<SPE);					// enable
	}
}

// sends single byte
void SPIsendByte(uint8_t byte_send)
{
	SPDR = byte_send;						// send byte
	while (!(SPSR & (1<<SPIF)));			// wait for transmission to complete
}

// receives single byte and returns it
uint8_t SPIreceiveByte()
{
	SPDR = 0x00;							// dummy byte
	while (!(SPSR & (1<<SPIF)));			// wait for transmission to complete
	return SPDR;							// receive byte
}

// sends multiple bytes
void SPIsend(uint8_t *data, uint8_t amount)
{
	for (i=1;i<=amount;i++)
	{
		SPIsendByte(data[i-1]);
		_delay_us(100);
	}
}

// receives multiple bytes and puts into *data
void SPIreceive(uint8_t *data, uint8_t amount)
{
	for (i=1;i<=amount;i++)
	{
		data[i-1] = SPIreceiveByte();
		_delay_us(100);
	}
}
