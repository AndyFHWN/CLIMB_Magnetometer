/*
 * simpleTimer.h
 *
 * Created: 21/01/2022 08:24:04
 *  Author: Andreas Stren
 */ 

// simple timer for use with Atmega128 Timer0 at 16Mhz or 32,768KHz
// for other frequencies adaption is needed

#ifndef SIMPLETIMER_H_
#define SIMPLETIMER_H_

	#include "F_CPU.h"

	#include <inttypes.h>
	#include <avr/interrupt.h>

	uint8_t sreg;
	uint16_t counter_ISR;				// counter until one second
	
	volatile uint8_t second;
	volatile uint8_t minute;
	volatile uint8_t hour;
	volatile uint8_t day;
	
	volatile uint8_t set_second;
	volatile uint8_t set_minute;
	volatile uint8_t set_hour;
	volatile uint8_t set_day;
	
	void simpleTimer0Init(void);
	void enable_simpleTimer0(void);
	void disable_simpleTimer0(void);
	void set_clock(void);

	ISR(TIMER0_COMP_vect);

#endif /* SIMPLETIMER_H_ */