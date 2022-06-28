/*
 * ioMC.h
 *
 * Created: 01/10/2021 11:20:48
 *  Author: Andreas Stren
 */ 


#ifndef OIMC_H_
	#define OIMC_H_
	
	#include <avr/io.h>
	
	// Chip Selects MEMSICs
	#define MEM_1		(1<<PC2)
	#define MEM_2		(1<<PC3)
	#define MEM_3		(1<<PC4)
	
	// Chip Selects DACs
	#define DAC_1		(1<<PA1)
	#define DAC_2		(1<<PA2)
	#define DAC_3		(1<<PA3)
	
	// Chip Selects ADCs
	#define ADC_1		(1<<PA4)
	#define ADC_2		(1<<PA5)
	#define ADC_3		(1<<PA6)
	
	// PWN reset signals
	#define RES_A		(1<<PB5)
	#define RES_B		(1<<PB6)
	
	void InitPins(void);


#endif /* OIMC_H_ */