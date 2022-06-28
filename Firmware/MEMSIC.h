/*
 * MEMSIC.h
 *
 * Created: 05/10/2021 16:31:06
 *  Author: Burner
 */ 


#ifndef MEMSIC_H_
	#define MEMSIC_H_
	
	#include "F_CPU.h"
	
	#include "SPI.h"
	#include "ioMC.h"
	#include <util/delay.h>
	
	volatile uint16_t M_Raw[3];
	volatile double M_Field_G[3];
	volatile double M_Field_nT[3];
	volatile double M_Temperature;
	volatile double M_OffsetnT[3][3];
	
	
	uint8_t M_read_reg(uint8_t MEM, uint8_t M_reg);
	void M_write_reg(uint8_t MEM, uint8_t M_reg, uint8_t data);
	void M_set(void);
	void M_reset(void);
	void M_Init(void);
	void M_measure_M(uint8_t MEM);
	void M_measure_T(uint8_t MEM);
	void M_read_M(uint8_t MEM);
	void M_read_T(uint8_t MEM);

#endif /* MEMSIC_H_ */
