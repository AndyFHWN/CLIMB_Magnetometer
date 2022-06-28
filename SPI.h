/*
 * SPI.h
 *
 * Created: 01/10/2021 12:28:43
 *  Author: Andreas Stren
 */ 


#ifndef SPI_H_
	#define SPI_H_

	#include "F_CPU.h"
	
	#include <avr/io.h>
	#include <inttypes.h>
	#include <avr/interrupt.h>
	#include <util/delay.h>

	#define SS			PB0
	#define SCK			PB1
	#define MOSI		PB2
	#define MISO		PB3
	
	void SPIInit(void);
	void SPIMode0(void);
	void SPIMode1(void);
	void SPIMode2(void);
	void SPIMode3(void);
	void SPIDeinit(void);
	void SPIReinit(void);
	
	void SPIsendByte(uint8_t byte_send);
	uint8_t SPIreceiveByte();
	
	void SPIsend(uint8_t *data, uint8_t amount);
	void SPIreceive(uint8_t *data, uint8_t amount);


#endif /* SPI_H_ */