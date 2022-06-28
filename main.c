/*
 * main.c
 *
 * Created: 10/1/2021 10:18:15 AM
 *  Author: Andreas Stren
 */ 

#include "main.h"

#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "modes.h"				// Mode definitions
#include "ioMC.h"				// Pin definitions
#include "SPI.h"				// SPI commands
#include "UART.h"				// UART commands
#include "TWI.h"				// Two Wire Interface, I²2
#include "HMC.h"				// HMC sensory
#include "MEMSIC.h"				// Memsic Sensory
#include "simpleTimer.h"

#define Buffer_Max 100
uint8_t UART_Buffer[Buffer_Max];
uint16_t w = 0;

int main(void)
{
	sei();						// enable global interrupt
	
	_delay_ms(5000);
	
	InitPins();
	UARTInit();
	TWIInit();
	SPIInit();
	ResetInit();
//	simpleTimer0Init();

// 	HMC_offsetnT[0]=0;
// 	HMC_offsetnT[1]=0;
// 	HMC_offsetnT[2]=0;

// 	HMC_offsetnT[0]=6281;
// 	HMC_offsetnT[1]=13866;
// 	HMC_offsetnT[2]=13264;

	HMC_offsetnT[0]=-5531;
	HMC_offsetnT[1]=-11447;
	HMC_offsetnT[2]=7121;

// 	HMC_offset_fac[0]=1;
// 	HMC_offset_fac[1]=1;
// 	HMC_offset_fac[2]=1;

// 	HMC_offset_fac[0]=1.6753;
// 	HMC_offset_fac[1]=1.6478;
// 	HMC_offset_fac[2]=1.5004;

	HMC_offset_fac[0]=1.2873;
	HMC_offset_fac[1]=1.3094;
	HMC_offset_fac[2]=1.2096;
	
	SPIMode0();
	HMCinit();
	ResetSet();
	setHMCoffset();
	
// 	SPIMode3();
// 	M_Init();
	
	M_OffsetnT[0][0] = 0;
	M_OffsetnT[0][1] = 0;
	M_OffsetnT[0][2] = 0;
	M_OffsetnT[1][0] = 0;
	M_OffsetnT[1][1] = 0;
	M_OffsetnT[1][2] = 0;
	M_OffsetnT[2][0] = 0;
	M_OffsetnT[2][1] = 0;
	M_OffsetnT[2][2] = 0;
	
// 	sprintf(UART_Buffer, "\n Initialized \n");
// 	UART0PutS(UART_Buffer, strlen(UART_Buffer));
// 	_delay_ms(100);

	set_second = 0;
	set_minute = 0;
	set_hour = 0;
	set_day = 0;
	
//	set_clock();
//	enable_simpleTimer0();
	
    while(1)
    {
// TODO:: modes

// MEMSIC TEST

  	SPIMode3();
		
	_delay_ms(1);
	M_reset();
	M_set();
	M_reset();
	M_set();
// 	M_reset();
// 	M_set();

	_delay_ms(1);
	M_measure_M(MEM_1);
	M_measure_T(MEM_1);
	M_read_M(MEM_1);
	M_read_T(MEM_1);
	_delay_ms(1);
		
	sprintf(UART_Buffer, "MMA1 %.0f %.0f %.0f %.1f\n", M_Field_nT[0], M_Field_nT[1], M_Field_nT[2], M_Temperature);
	_delay_ms(10);
	UART0PutS(UART_Buffer, strlen(UART_Buffer));
	_delay_ms(50);
		
	_delay_ms(1);
	M_measure_M(MEM_2);
	M_measure_T(MEM_2);
	M_read_M(MEM_2);
	M_read_T(MEM_2);
	_delay_ms(1);
		
	sprintf(UART_Buffer, "MMA2 %.0f %.0f %.0f %.1f\n", M_Field_nT[0], M_Field_nT[1], M_Field_nT[2], M_Temperature);
	_delay_ms(10);
	UART0PutS(UART_Buffer, strlen(UART_Buffer));
	_delay_ms(50);
			
	_delay_ms(1);
	M_measure_M(MEM_3);
	M_measure_T(MEM_3);
	M_read_M(MEM_3);
	M_read_T(MEM_3);
	_delay_ms(1);
		
	sprintf(UART_Buffer, "MMA3 %.0f %.0f %.0f %.1f\n", M_Field_nT[0], M_Field_nT[1], M_Field_nT[2], M_Temperature);
	_delay_ms(10);
	UART0PutS(UART_Buffer, strlen(UART_Buffer));	
	_delay_ms(50);

//HMC TEST

		SPIMode0();
		
		ResetSet();

 
// 		if (w==200)
// 		{
// 			ResetSet();
// 				HMC_offsetnT[0]=-5531;
// 				HMC_offsetnT[1]=-11447;
// 				HMC_offsetnT[2]=7121;
// 				setHMCoffset();
// 		}
// 		if (w==400)
// 		{
// 			SetReset();
// 				HMC_offsetnT[0]=5531;
// 				HMC_offsetnT[1]=11447;
// 				HMC_offsetnT[2]=-7121;
// 				setHMCoffset();
// 			w = 0;
// 		}
// 		w++;

// 		if (w==400 || w==800 || w==1200 || w==1600 || w==2000 || w==2400)
// 		{
// 			HMC_offsetnT[0]=HMC_offsetnT[0]+10000;
// 			HMC_offsetnT[1]=HMC_offsetnT[1]+10000;
// 			HMC_offsetnT[2]=HMC_offsetnT[2]+10000;
// 			setHMCoffset();
// 		}
// 		if (w==2800)
// 		{
// 			HMC_offsetnT[0]=-30000;
// 			HMC_offsetnT[1]=-30000;
// 			HMC_offsetnT[2]=-30000;
// 			setHMCoffset();
// 			w = 0;
// 		}
// 		w++;
		
		_delay_ms(5);
		
		readHMCvalue();
		
		_delay_ms(5);
		
		sprintf(UART_Buffer, "HMC %.0f %.0f %.0f\n", HMC_field_nT[0], HMC_field_nT[1], HMC_field_nT[2]);
		_delay_ms(10);
		UART0PutS(UART_Buffer, strlen(UART_Buffer));
		_delay_ms(50);
		

// Clock Test

//  		sprintf(UART_Buffer, "%d : %d : %d : %d\n", day, hour, minute, second);
//  		UART0PutS(UART_Buffer, strlen(UART_Buffer));
// 		_delay_ms(100);
		
    }
}




