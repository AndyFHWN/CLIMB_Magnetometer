/*
 * HMC.h
 *
 * Created: 05/10/2021 16:21:06
 *  Author: Andreas Stren
 */ 


/*	
*	Intended for usage with:
*	ADC: LTC2440		in differential mode from -5V to 5V
*	DAC: DAC8831_D		for offset current
*/


#ifndef HMC_H_
	#define HMC_H_
	
	#include "F_CPU.h"
	
	#include <math.h>
	#include <util/delay.h>
	#include <inttypes.h>
	
	#include "ioMC.h"
	#include "SPI.h"
	
	uint32_t HMC_data[3];
	double HMC_value[3];
	double HMC_field_G[3];
	double HMC_field_nT[3];
	double HMC_offset[3];
	double HMC_offset_fac[3];
	double HMC_offsetnT[3];
	
	void ResetInit(void);
	void SetReset(void);
	void ResetSet(void);
	void HMCinit(void);
	void Convert(void);
	void readHMCvalue(void);
	void setHMCoffset(void);

#endif /* SENSORS_H_ */
