//
// TWI.h
//

#ifndef TWI_H_
	#define TWI_H_
	
	#include <avr/io.h>
	#include <inttypes.h>
	#include <avr/interrupt.h>

	#include "F_CPU.h"

	#define PIN_SDA PD1					// SDA pin
	#define DDR_SDA DDRD				// SDA data direction register

	#define TWI_BIT_RATE_KHZ 0,8			// bit rate of the TWI bus in kHz

	// calculate the bit rate division factor with prescaler = 1
	#define TWI_BIT_RATE_DIV_ ((F_CPU)/1000)/(TWI_BIT_RATE_KHZ)
	#if (TWI_BIT_RATE_DIV_ >= 16)
		#define TWI_BIT_RATE_DIV ((TWI_BIT_RATE_DIV_)-16)/2
	#else
		#error Bit rate too high!
	#endif

	//--- TWI State codes -------------------------------------------------------------------------
	// general TWI master status codes                      
	#define TWI_START					0x08  // START has been transmitted  
	#define TWI_REP_START				0x10  // Repeated START has been transmitted
	#define TWI_ARB_LOST				0x38  // Arbitration lost
	// TWI master transmitter status codes                      
	#define TWI_MTX_ADR_ACK				0x18  // SLA+W has been transmitted and ACK received
	#define TWI_MTX_ADR_NACK			0x20  // SLA+W has been transmitted and NACK received
	#define TWI_MTX_DATA_ACK			0x28  // Data byte has been transmitted and ACK received
	#define TWI_MTX_DATA_NACK			0x30  // Data byte has been transmitted and NACK received
	// TWI master receiver status codes  
	#define TWI_MRX_ADR_ACK				0x40  // SLA+R has been transmitted and ACK received
	#define TWI_MRX_ADR_NACK			0x48  // SLA+R has been transmitted and NACK received
	#define TWI_MRX_DATA_ACK			0x50  // Data byte has been received and ACK transmitted
	#define TWI_MRX_DATA_NACK			0x58  // Data byte has been received and NACK transmitted
	// TWI slave transmitter status codes
	#define TWI_STX_ADR_ACK				0xA8  // Own SLA+R has been received; ACK has been returned
	#define TWI_STX_ADR_ACK_M_ARB_LOST	0xB0  // Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned
	#define TWI_STX_DATA_ACK			0xB8  // Data byte in TWDR has been transmitted; ACK has been received
	#define TWI_STX_DATA_NACK			0xC0  // Data byte in TWDR has been transmitted; NOT ACK has been received
	#define TWI_STX_DATA_ACK_LAST_BYTE	0xC8  // Last data byte in TWDR has been transmitted (TWEA = “0”); ACK has been received
	// TWI slave receiver status codes
	#define TWI_SRX_ADR_ACK				0x60  // Own SLA+W has been received ACK has been returned
	#define TWI_SRX_ADR_ACK_M_ARB_LOST	0x68  // Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned
	#define TWI_SRX_GEN_ACK				0x70  // General call address has been received; ACK has been returned
	#define TWI_SRX_GEN_ACK_M_ARB_LOST	0x78  // Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned
	#define TWI_SRX_ADR_DATA_ACK		0x80  // Previously addressed with own SLA+W; data has been received; ACK has been returned
	#define TWI_SRX_ADR_DATA_NACK		0x88  // Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
	#define TWI_SRX_GEN_DATA_ACK		0x90  // Previously addressed with general call; data has been received; ACK has been returned
	#define TWI_SRX_GEN_DATA_NACK		0x98  // Previously addressed with general call; data has been received; NOT ACK has been returned
	#define TWI_SRX_STOP_RESTART		0xA0  // A STOP condition or repeated START condition has been received while still addressed as Slave
	// TWI miscellaneous status codes
	#define TWI_NO_STATE				0xF8  // No relevant state information available; TWINT = “0”
	#define TWI_BUS_ERROR				0x00  // Bus error due to an illegal START or STOP condition
	// masks for TWCR and TWSR
	#define TWCR_CMD_MASK				0x0F
	#define TWSR_STATUS_MASK			0xF8
	// TWI return values
	#define TWI_OK						0x00
	#define TWI_ERROR_NODEV				0x01
	// TWI read/write bit
	#define TWIM_READ					0x01
	#define TWIM_WRITE					0x00

	// function prototypes
	void TWIInit(void);
	void TWIDeInit(void);
	void TWIReInit(void);
	void TWISendStart(void);
	void TWISendStop(void);
	void TWIWaitForComplete(void);
	void TWISendByte(const uint8_t data);
	void TWIReceiveByte(const uint8_t ackFlag);
	uint8_t TWIGetReceivedByte(void);
	uint8_t TWIGetStatus(void);

	uint8_t TWIMasterSendWait(const uint8_t address, const uint8_t *data, const uint8_t length, const uint8_t sendStop);
	uint8_t TWIMasterReceiveWait(const uint8_t address, uint8_t *data, const uint8_t length, const uint8_t sendStop);
#endif
