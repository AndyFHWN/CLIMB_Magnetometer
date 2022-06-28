/*
 * HMC.c
 *
 * Created: 05/10/2021 16:21:22
 *  Author: Andreas Stren
 */ 

#include "HMC.h"

// Iterators
uint8_t i;
uint8_t j;

// data handling
uint8_t received[4];				// receive buffer
uint8_t temporary[4];				// shift buffer
uint8_t sign;
double ADC_resolution = 16777216.0;						
double ADC_FactorG = 0.000000298;
double ADC_FactornT = 0.0298;

// offset handling
double offset_d[3];					// offset input as double
uint16_t offset_i[3];					// offset prepared for DAC
//double DAC_Factor = 6553;
//double DAC_Factor = 10684.5;
double DAC_Factor = 7123;
uint8_t MSB[3];						// offset transfer bytes MSB
uint8_t LSB[3];						// offset transfer bytes LSB

void ResetInit(void)
{
	PORTB |= RES_A;
	PORTB &= ~RES_B;
}

void ResetSet(void)
{
	for (i=0;i<=9;i++)
	{
		PORTB |= RES_B;
		PORTB &= ~RES_A;
		_delay_us(4);
		PORTB |= RES_A;
		PORTB &= ~RES_B;
		_delay_us(4);
	}
}

void SetReset(void)
{
	for (i=0;i<=9;i++)
	{
		PORTB |= RES_A;
		PORTB &= ~RES_B;
		_delay_us(4);
		PORTB |= RES_B;
		PORTB &= ~RES_A;
		_delay_us(4);
	}
}

void HMCinit(void)
{
	for(j=0;j<=2;j++)
	{
		if(j==0){PORTA &= ~ADC_1;}
		if(j==1){PORTA &= ~ADC_2;}
		if(j==2){PORTA &= ~ADC_3;}
			
		_delay_ms(5);
		while ( (PINB & (1 << MISO))!=0 ){}
		_delay_ms(1);
//		SPIsendByte(0b01111000);		//OSR 32768
		SPIsendByte(0b01001000);		//OSR 16384
//		SPIsendByte(0b00001000);		//OSR 64
		_delay_ms(1);

		if(j==0){PORTA |= ADC_1;}
		if(j==1){PORTA |= ADC_2;}
		if(j==2){PORTA |= ADC_3;}
		_delay_us(5);
	}
}

void Convert(void){
	for (i=0;i<=2;i++)
	{
		HMC_field_nT[i] = ADC_FactornT*HMC_value[i];
		HMC_field_G[i] = ADC_FactorG*HMC_value[i];
	}
}

void readHMCvalue(void)
{
	for(j=0;j<=2;j++)
	{
		EOC:
		_delay_us(5);
// 		if(j==0){PORTA &= ~ADC_1;}
// 		if(j==1){PORTA &= ~ADC_2;}
// 		if(j==2){PORTA &= ~ADC_3;}
// 			
//  		if ( (PINB & (1<<MISO))!=0 )
// 		{
// 			if(j==0){PORTA |= ADC_1;}
// 			if(j==1){PORTA |= ADC_2;}
// 			if(j==2){PORTA |= ADC_3;}
// 			goto EOC;
// 		}
// 		else
// 		{
// 			if(j==0){PORTA |= ADC_1;}
// 			if(j==1){PORTA |= ADC_2;}
// 			if(j==2){PORTA |= ADC_3;}
// 		}
			
		if(j==0){PORTA &= ~ADC_1;}
		if(j==1){PORTA &= ~ADC_2;}
		if(j==2){PORTA &= ~ADC_3;}
		
		_delay_us(1000);
		SPIreceive(received, 4);
		_delay_us(1000);

		if(j==0){PORTA |= ADC_1;}
		if(j==1){PORTA |= ADC_2;}
		if(j==2){PORTA |= ADC_3;}
		_delay_us(5);
		
		if ((received[0] & 0b10000000) != 0)
		{
			goto EOC;
		}

		for (i=0;i<=2;i++)
		{
			temporary[i] = received[i]<<3;
			temporary[i+1] = received[i+1]>>5;
			temporary[i] = temporary[i] | temporary[i+1];
		}
		
		HMC_data[j]=0;
		for (i=0;i<=2;i++)
		{
			HMC_data[j] |= temporary[i];
			if (i<2)
			{
				HMC_data[j] = HMC_data[j]<<8;
			}
		}	
		
		HMC_value[j] = (double) HMC_data[j];
		
		sign = (received[0] & 0b0010000);
		if (sign != 0)
		{
			HMC_value[j] = HMC_value[j] - ADC_resolution;
			//HMC_value[j] -= ADC_resolution;
		}	
	}
	Convert();
}

void setHMCoffset(void)
{
	for (j=0;j<=2;j++)
	{
		HMC_offset[j]=(HMC_offsetnT[j]/100000.0)*HMC_offset_fac[j];
	}
	for (j=0;j<=2;j++)
	{
		offset_d[j] = 4.6+HMC_offset[j];
		
		offset_d[j] *= DAC_Factor;
		offset_i[j] = (uint16_t)offset_d[j];
		
		MSB[j] = (offset_i[j] & 0xFF00) >> 8;
		LSB[j] = (offset_i[j] & 0x00FF);
		
		if(j==0){PORTA &= ~DAC_1;}
		if(j==1){PORTA &= ~DAC_2;}
		if(j==2){PORTA &= ~DAC_3;}
		
		_delay_us(1000);
		SPIsendByte(MSB[j]);
		_delay_us(1000);
		SPIsendByte(LSB[j]);
		_delay_us(1000);
		
		if(j==0){PORTA |= DAC_1;}
		if(j==1){PORTA |= DAC_2;}
		if(j==2){PORTA |= DAC_3;}
	}
}

