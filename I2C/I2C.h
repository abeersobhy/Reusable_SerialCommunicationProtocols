#ifndef _I2C_H_
#define _I2C_H_

#include "I2C_Config.h"
typedef enum 
{
	I2C_AcknowledgeEnable, 
	I2C_AcknowledgeDisable,
}I2C_AcknowledgeState;

typedef enum
{
	I2C_MasterEnable,
	I2C_SlaveEnable,
	I2C_Disable,
}I2C_ControlState;

typedef enum
{
	I2C_InterruptEnable,
	I2C_InterruptDisable,	
}I2C_InterruptControl;

typedef enum
{
	I2C_Prescaler1,
	I2C_Prescaler4,
	I2C_Prescaler16,
	I2C_Prescaler64,
}I2C_Prescaler;

typedef struct
{
	I2C_AcknowledgeState acknowledgeState ;
    I2C_ControlState ControlState;
	I2C_InterruptControl interruptControl;
	I2C_Prescaler prescaler;		
	uint32_t SCL_Frequency; /*Only to master*/
    uint8_t ID;
	uint8_t RW;
}I2C_Configuration;

void I2C_Init();
void I2C_MasterStart();
void I2C_MasterAddress(uint8_t ID);
void I2C_MasterWrite(uint8_t data);
uint8_t I2C_MasterRead();
void I2C_MasterStop();
uint8_t I2C_SlaveRead();
uint8_t I2C_slaveAvialable();

extern I2C_Configuration I2C;
#endif