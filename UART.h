//
// UART.h
//

#ifndef UART_H_
	#define UART_H_

	#include <avr/io.h>
	#include <inttypes.h>
	#include <avr/interrupt.h>

	#include "F_CPU.h"

	#define BAUD0 10400ul								// Baud rate of Interface 0 - FTDI (tiny?)
	#define BAUD1 10400ul								// Baud rate of Interface 1 - LPC (tiny?)
	
	// calculations
	#define UBRR_VAL0 (((F_CPU)/(BAUD0*16))-1)			// round clever
	#define BAUD0_REAL ((F_CPU)/(16*(UBRR_VAL0+1)))		// real Baud rate
	#define BAUD0_ERROR (((BAUD0_REAL)*1000)/(BAUD0))	// error in per mil, 1000 = no error
	
	#if ((BAUD0_ERROR<990) || (BAUD0_ERROR>1010))
		#error Baud rate error greater 1% which is too high!
	#endif
	
	#define UBRR_VAL1 (((F_CPU)/(BAUD1*16))-1)			// round clever
	#define BAUD1_REAL ((F_CPU)/(16*(UBRR_VAL1+1)))		// real Baud rate
	#define BAUD1_ERROR ((BAUD1_REAL*1000)/BAUD1)		// error in per mil, 1000 = no error
	
	#if ((BAUD1_ERROR<990) || (BAUD1_ERROR>1010))
		#error Baud rate error greater 1% which is too high!
	#endif
	
	// string definitions
	#define DATA_REC_LEN	29							// length of the data vector from CC1/CC2

	// global variables
	extern volatile char global_UART0String[DATA_REC_LEN + 2];
	extern volatile uint8_t global_RX0Complete;			// 1 = string completely received
	extern volatile uint8_t global_TX0Complete;			// 1 = string completely transmitted

	extern volatile char global_UART1String[DATA_REC_LEN + 2];
	extern volatile uint8_t global_RX1Complete;			// 1 = string completely received
	extern volatile uint8_t global_TX1Complete;			// 1 = string completely transmitted
	
	// function prototypes
	void UARTInit(void);
	void UART0PutS(uint8_t *s, const uint8_t length);
	void UART1PutS(uint8_t *s, const uint8_t length);
	void UART0DeInit(void);
	void UART0ReInit(void);
	void UART1DeInit(void);
	void UART1ReInit(void);
	uint8_t ValidateChecksum( uint8_t CC );
#endif
