/*
 * simpleTimer.c
 *
 * Created: 21/01/2022 08:23:38
 *  Author: Andreas Stren
 */ 


#include "simpleTimer.h"

void simpleTimer0Init(void)
{
	TCCR0 = (1<<CS02)|(1<<CS01)|(1<<CS00);		// prescaler 1024
	
	if (F_CPU == 16000000)						// timer frequency = F_CPU / 1024 = 15625Hz
	{											// largest factor below 256 = 125
		OCR0 = 125-1;							// compare value = 15625 / 125 = 125 starting at 0
	}
	if (F_CPU == 32768)							// timer frequency = F_CPU / 1024 = 32Hz
	{
		OCR0 = 32-1;							// compare value = 32 starting at 0
	}
	
	TIMSK |= (1<<OCIE0);						// enable interrupt on compare match
}

void enable_simpleTimer0(void)
{
	sreg = SREG;
	cli();
	
	TCCR0 |= (1<<WGM01);
	TCNT0 = 0;
	
	SREG = sreg;
}

void disable_simpleTimer0(void)
{
	sreg = SREG;
	cli();
	
	TCCR0 &= ~(1<<WGM01);
	TCNT0 = 0;
	
	SREG = sreg;
}

void set_clock(void)
{
	sreg = SREG;
	cli();
	
	second = set_second;;
	minute = set_minute;
	hour = set_hour;
	day = set_hour;
	
	SREG = sreg;
}

ISR(TIMER0_COMP_vect)
{
	counter_ISR += 1;
	cli();
	if (F_CPU == 16000000)					// for faster ISR delete F_CPU check and keep 	
	{										// block according to your F_CPU
		if (counter_ISR	== 125)
		{
			second += 1;
			counter_ISR = 0;
		}				
	}
	if (F_CPU == 32768)						
	{
		if (counter_ISR	== 32)
		{
			second += 1;
			counter_ISR = 0;
		}						
	}
	sei();
	if (second >= 60)
	{
		minute += 1;
		second = 0;
		if (minute >= 60)
		{
			hour += 1;
			minute = 0;
			if (hour >=24)
			{
				day += 1;
				hour = 0;
			}
		}
	}
}
