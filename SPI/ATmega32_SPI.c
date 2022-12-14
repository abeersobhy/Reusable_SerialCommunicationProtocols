/*
 * SPI.c
 *
 * Created: 02/10/2022 05:01:32 م
 *  Author: user
 */ 
#include "ATmega32_SPI.h"

SPI_ConfigStruct SPI0 = {0};
	
void SPI_Init(void)
{
	switch(SPI0.controlModes)
	{
		case SPI_Disable : /*Nothing to do.*/
		break;
		case SPI_MasterEnable_DoubleSpeed : /*Enable MC as Master with double speed.*/
		{
			 SET_BIT(DDRB , PIN4); /*SS OUT*/
			 SET_BIT(DDRB , PIN5); /*MOSI OUT*/
			 SET_BIT(DDRB , PIN7); /*SCK OUT*/
			 SPSR |= (1 << SPI2X);
			 SPCR |= (1 << MSTR);
		}
		break;
		case SPI_MasterEnable_NormalSpeed : /*Enable MC as Master with Normal speed.*/
		{
			SET_BIT(DDRB , PIN4); /*SS OUT*/
			SET_BIT(DDRB , PIN5); /*MOSI OUT*/
			SET_BIT(DDRB , PIN7); /*SCK OUT*/
		    SPCR |= (1 << MSTR);
		}
		break;
		case SPI_SlaveEnable: /*Enable MC as Master with Normal speed.*/
			 SET_BIT(DDRB , PIN6); /*MISO OUT.*/	 
		break;
		default: /*Nothing to do.*/
		break;
	}
	switch(SPI0.clockRate)
	{
		case SPI_F_CPU_DIV_2   : /*F_CPU Frequency divided by 2*/
		     SPSR |= (1 << SPI2X);
		break;
		case SPI_F_CPU_DIV_4   : /*F_CPU Frequency divided by 4*/
		break; /*Nothing to do.*/
		case SPI_F_CPU_DIV_8   : /*F_CPU Frequency divided by 8*/
   			 SPCR |= (1 << SPR0);
			 SPSR |= (1 << SPI2X);
		break;
		case SPI_F_CPU_DIV_16  : /*F_CPU Frequency divided by 16*/
			 SPCR |= (1 << SPR0);
		break;
		case SPI_F_CPU_DIV_32  : /*F_CPU Frequency divided by 32*/
			 SPCR |= (1 << SPR1);
			 SPSR |= (1 << SPI2X);
		break;
		case SPI_F_CPU_DIV_64  : /*F_CPU Frequency divided by 64*/
			 SPCR |= (1 << SPR0) | (1 << SPR1);
			 SPSR |= (1 << SPI2X);
		break;
		case SPI_F_CPU_DIV_128 : /*F_CPU Frequency divided by 128*/
		     SPCR |= (1 << SPR0) | (1 << SPR1);
		break;
		default:
		break;
	}
	switch(SPI0.dataOrder)
	{
		case SPI_LSB : /*Send the LSB bits first.*/
		     SPCR |= (1 << DORD);
		break;
		case SPI_MSB : /*Send the MSB bits first.*/
		     /*Nothing to do.*/
		break; 
		default:
		break;
	}
	switch(SPI0.controlInterrupt)
	{
		case SPI_InterruptDisable : /*Disable interrupt transmitted flag.*/
		     /*Nothing to do*/
		break;
		case SPI_InterruptEnable : /*Enable interrupt transmitted flag.*/
		     SPCR |= (1 << SPIE);
		break;
		default:
		break;
	}
	switch(SPI0.clockModes)
	{
		/*
		* Note that if you use mode3 or mode2 the idle state of the clock must be HIGH
		* In some controller we must make the default value of the clock (SCK - PIN7) is HIGH
		* by setting it to output with initial value HIGH
		*/
		case SPI_Mode0 : /*Sample on rising edge & Setup on falling edge.*/
		     /*Nothing to do.*/
		break;
		case SPI_Mode1 : /*Set up on rising edge & Sample on falling edge.*/
		     SPCR |= (1 << CPHA);
		break;
		case SPI_Mode2 : /*Set up on rising edge & Sample on falling edge.*/
		     SPCR |= (1 << CPOL);
		     SET_BIT(PORTB , PIN7); /**/
		break;
		case SPI_Mode3 : /*Sample on rising edge & Setup on falling edge.*/
		     SPCR |= (1 << CPOL) | (1 << CPHA);
			 SET_BIT(PORTB , PIN7);
		break;
		default:
		break; 
	}
	SPCR |= (1 << SPE); /*Enable SPI.*/
}
void SPI_sendData_Blocking(uint8_t data)
{
	SPDR = data;
	if(READ_BIT(SPSR , WCOL) == 1)
 	{
		/*Error handling*/
		SPI0.error = SPI_DataWritingBeforSending;
 	} 
 	while(READ_BIT(SPSR , SPIF) == 0); /*There is a non transmitted data on the shift register.*/
}
void SPI_sendData_NonBlocking(uint8_t data)
{
	SPDR = data;
	if(READ_BIT(SPSR , WCOL) == 1)
	{
		/*Error handling*/
		SPI0.error = SPI_DataWritingBeforSending;
	}
}
uint8_t SPI_receiveData_Blocking()
{
	while(READ_BIT(SPSR , SPIF) == 0); /*Data is not received yet.*/
	return SPDR;
}
uint8_t SPI_receiveData_NonBlocking()
{
	if(READ_BIT(SPSR , SPIF) != 0) /*Data is not received yet.*/
	{	
		SPI0.error = dataReceived;
	}
	return SPDR;
}
uint8_t SPI_ExchanngeData_Blocking(uint8_t data)
{
	SPDR = data;
	if(READ_BIT(SPSR , WCOL) == 1)
	{
		/*Error handling*/
		SPI0.error = SPI_DataWritingBeforSending;
	}
	else
	{
		SPI0.error = SPI_NoError;
		while(READ_BIT(SPSR , SPIF) == 0); /*There is a non transmitted data on the shift register.*/	
	}
	return SPDR;
}
uint8_t SPI_ExchanngeData_NonBlocking(uint8_t data)
{
	SPDR = data;
	if(READ_BIT(SPSR , WCOL) == 1)
	{
		/*Error handling*/
		SPI0.error = SPI_DataWritingBeforSending;
	}
	return SPDR;
}
ISR(SPI_STC_vect)
{
	if (SPI0.SPI_TransmittComplete != NULL)
	{
		SPI0.SPI_TransmittComplete();
	}
}
