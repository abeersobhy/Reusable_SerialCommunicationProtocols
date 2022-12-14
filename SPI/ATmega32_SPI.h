/*
 * SPI.h
 *
 * Created: 02/10/2022 05:01:02 م
 *  Author: user
 */ 


#ifndef SPI_H_
#define SPI_H_
#include <avr/interrupt.h>
#include "../../Utilities/bit_math.h"
#define NULL '\0'
typedef enum
{
	SPI_InterruptEnable,
	SPI_InterruptDisable,
}SPI_ControlInterrupt;

typedef enum
{
	SPI_MasterEnable_DoubleSpeed,
	SPI_MasterEnable_NormalSpeed,
	SPI_SlaveEnable,
	SPI_Disable,
}SPI_ControlModes;

typedef enum
{
	SPI_LSB,
	SPI_MSB,	
}SPI_DataOrder;

typedef enum
{
	SPI_Mode0,
	SPI_Mode1,
	SPI_Mode2,
	SPI_Mode3,
}SPI_ClockModes;

typedef enum
{
	SPI_F_CPU_DIV_2,
	SPI_F_CPU_DIV_4,
	SPI_F_CPU_DIV_8,
	SPI_F_CPU_DIV_16,
	SPI_F_CPU_DIV_32,
	SPI_F_CPU_DIV_64,
	SPI_F_CPU_DIV_128,
}SPI_Frequency;

typedef enum
{
	SPI_NoError,
	SPI_DataWritingBeforSending,	
	dataReceived,
}SPI_Errors;
typedef struct
{
	SPI_ControlInterrupt  controlInterrupt;
	SPI_ControlModes      controlModes;
	SPI_DataOrder         dataOrder;
	SPI_ClockModes        clockModes;
    SPI_Frequency         clockRate;
	SPI_Errors            error;
	void (*SPI_TransmittComplete)(void);
}SPI_ConfigStruct;

void SPI_Init(void);
void SPI_sendData_Blocking(uint8_t data);
void SPI_sendData_NonBlocking(uint8_t data);
uint8_t SPI_receiveData_Blocking();
uint8_t SPI_receiveData_NonBlocking();
uint8_t SPI_ExchanngeData_Blocking(uint8_t data);
uint8_t SPI_ExchanngeData_NonBlocking(uint8_t data);

extern SPI_ConfigStruct SPI0;
#endif /* SPI_H_ */