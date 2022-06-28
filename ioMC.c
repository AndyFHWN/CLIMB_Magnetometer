/*
 * ioMC.c
 *
 * Created: 06/10/2021 10:56:12
 *  Author: Andreas Stren
 */ 

#include "ioMC.h"

void InitPins(void)
{
	DDRA = (DAC_1|DAC_2|DAC_3|ADC_1|ADC_2|ADC_3);		// setting Port A pins as output
	PORTA = (DAC_1|DAC_2|DAC_3|ADC_1|ADC_2|ADC_3);		// pulling outputs Port A high
	
	DDRB |= (RES_A|RES_B);								// setting reset pins as output
	
	DDRC = (MEM_1|MEM_2|MEM_3);							// setting Port C pins as output
	PORTC = (MEM_1|MEM_2|MEM_3);						// pulling outputs Port C high
}

