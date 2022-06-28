/*
 * MEMSIC.c
 *
 * Created: 05/10/2021 16:30:48
 *  Author: Andreas Stren
 */ 

#include "MEMSIC.h"
#include "SPI.h"

#define MMC 0x30
#define MMC_Write 0x00
#define MMC_Read 0x80

#define MMC_Xout0 0x00
#define MMC_Xout1 0x01
#define MMC_Yout0 0x02
#define MMC_Yout1 0x03
#define MMC_Zout0 0x04
#define MMC_Zout1 0x05
#define MMC_Tout 0x07
#define MMC_Status 0x08
#define MMC_control0 0x09
#define MMC_ID 0x2F

uint8_t measureM = 0x01;
uint8_t measureT = 0x02;
	
uint8_t setM = 0x08;
uint8_t resetM = 0x10;

uint8_t M_Data_M[6];
uint8_t M_Data_T;

uint8_t M_Status;
uint8_t M_control0;
uint8_t M_ID;

uint8_t i;
uint8_t j;
uint8_t reg_data;

float factorM = 0.000244140625;
float factorT = 0.78125;

uint8_t M_read_reg(uint8_t MEM, uint8_t M_reg)
{
	PORTC &= ~MEM;
	_delay_us(10);
	SPIsendByte(MMC_Read|M_reg);
	_delay_us(10);
	reg_data = SPIreceiveByte();
	_delay_us(10);
	PORTC |= MEM;
	return reg_data;
}

void M_write_reg(uint8_t MEM, uint8_t M_reg, uint8_t data)
{
	PORTC &= ~MEM;
	_delay_us(10);
	SPIsendByte(MMC_Write|M_reg);
	_delay_us(10);
	SPIsendByte(data);
	_delay_us(10);
	PORTC |= MEM;
}

void M_set()
{
	M_write_reg(MEM_1, MMC_control0, setM);
	_delay_us(5);
	M_write_reg(MEM_2, MMC_control0, setM);
	_delay_us(5);
	M_write_reg(MEM_3, MMC_control0, setM);
}

void M_reset()
{
	M_write_reg(MEM_1, MMC_control0, resetM);
	_delay_us(5);
	M_write_reg(MEM_2, MMC_control0, resetM);
	_delay_us(5);
	M_write_reg(MEM_3, MMC_control0, resetM);
}

void M_Init()
{
	M_reset();
	_delay_us(5);
	M_set();
}


void M_measure_M(uint8_t MEM)
{
	M_write_reg(MEM, MMC_control0, measureM);
	_delay_us(5);
	while ((M_read_reg(MEM, MMC_Status) & 0x01) == 0);
}

void M_measure_T(uint8_t MEM)
{
	M_write_reg(MEM, MMC_control0, measureT);
	_delay_us(5);
	while ((M_read_reg(MEM, MMC_Status) & 0x02 )== 0);
}

void M_read_M(uint8_t MEM)
{
	for (i=0;i<=5;i++)
	{
		M_Data_M[i] = M_read_reg(MEM, MMC_Xout0+i);
		_delay_ms(1);	
	}
	for (i=0;i<=2;i++)
	{
		M_Raw[i] = (M_Data_M[i*2]<<8)|M_Data_M[(i*2)+1];
		M_Field_G[i]=((double)M_Raw[i]*factorM)-8.0;
		M_Field_nT[i]=M_Field_G[i]*100000.0;
		if (MEM == MEM_1){j=0;}
		if (MEM == MEM_2){j=1;}
		if (MEM == MEM_3){j=2;}
		
		M_Field_G[i] = M_Field_G[i]+(M_OffsetnT[j][i]/100000.0);
		M_Field_nT[i] = M_Field_nT[i]+M_OffsetnT[j][i];
	}
}

void M_read_T(uint8_t MEM)
{
	M_Data_T = M_read_reg(MEM, MMC_Tout);
	_delay_ms(1);
	M_Temperature = ((float)M_Data_T * factorT) - 75;
}