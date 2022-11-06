/*
 * I2C.c
 *
 * Created: 10/10/2022 07:26:36 م
 *  Author: user
 */ 
#define F_CPU 16000000UL
#include "../../Utilities/std_types.h"
#include "../../Utilities/bit_math.h"
#include "../../Utilities/register.h"
#include <math.h>
#include "I2C.h"

I2C_Configuration I2C;
void I2C_Init()
{	
	/*TWBR = ((F_CPU / SCL_frequency) - 16) / (2*(4^prescalerValue)))*/
	switch(I2C.ControlState)
	{
		case I2C_MasterEnable :
		     switch(I2C.prescaler)
		     {
			     case I2C_Prescaler1 :
			          TWBR = ((F_CPU / I2C.SCL_Frequency) - 16) / (2*1);
			     break;
			     case I2C_Prescaler4:
			          TWBR = ((F_CPU / I2C.SCL_Frequency) - 16) / (2*pow(4 , 4));
			     break;
			     case I2C_Prescaler16:
			          TWBR = ((F_CPU / I2C.SCL_Frequency) - 16) / (2*pow(4 , 16));
			     break;
			     case I2C_Prescaler64:
			          TWBR = ((F_CPU / I2C.SCL_Frequency) - 16) / (2*pow(4 , 64));
			     break;
			     default:
			     break;
		     }
		break;
		case I2C_SlaveEnable :
		     TWAR |= (I2C.ID << 1);
			 switch(I2C.acknowledgeState)
			 {
				 case I2C_AcknowledgeEnable :
				 TWCR |= (1 << TWEA);	/*Enable Acknowledge bit.*/
				 break;
				 case I2C_AcknowledgeDisable: /*Nothing to do.*/
				 break;
				 default:
				 break;
			 }
		break;
		default:
		break;	 		 	
	}
}

void I2C_MasterStart()
{
	TWCR |= (1 << TWINT);	/*Start the operation of the I2C.*/
	TWCR |= (1 << TWSTA);	/*Start bit.*/
	switch(I2C.ControlState)
	{
		case I2C_MasterEnable:
             TWCR |= (1 << TWEN);	/*Enable I2C.*/	
		break;
		case I2C_Disable : /*Nothing to do.*/
		break;
		default:
		break;
	}
	switch(I2C.interruptControl)
	{
		case I2C_InterruptEnable :
		     TWCR |= (1 << TWIE);	/*Enable Interrupt.*/	 
		break;
		case I2C_InterruptDisable : /*Nothing to do.*/
		break;
		default:
		break;
	}
	while(READ_BIT(TWCR , TWINT) == 0); /*Wait until the hardware set the TWINT Pin*/ 		
}
void I2C_MasterAddress(uint8_t ID)
{
	TWDR = (ID << 1);
	(I2C.RW == 1) ? SET_BIT(TWCR , TWWC) : CLR_BIT(TWCR , TWWC); 
	TWCR |= (1 << TWINT) | (1 << TWEN);	/*Start the operation of the I2C.*/ 
	while(READ_BIT(TWCR , TWINT) == 0); /*Wait until the hardware set the TWINT Pin*/
}
void I2C_MasterWrite(uint8_t data)
{
	TWDR = data; /*Data to send.*/
	TWCR |= (1 << TWINT) | (1 << TWEN);	/*Start the operation of the I2C.*/
	while(READ_BIT(TWCR , TWINT) == 0); /*Wait until the hardware set the TWINT Pin*/
}
uint8_t I2C_MasterRead()
{
	TWCR |= (1 << TWEA) | (1 << TWEN) | (1 << TWINT); /*Start the operation of the I2C.*/
	while(READ_BIT(TWCR , TWINT) == 0);
	return TWDR; /*Data is received.*/ 
}
void I2C_MasterStop()
{
	TWCR |= (1 << TWINT) | (1 << TWSTO)| (1 << TWEN);	/*Stop Condition.*/
}
uint8_t I2C_SlaveRead()
{
	TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWEA); /*Start the operation of the I2C.*/
	while(READ_BIT(TWCR , TWINT) == 0); /*Current event is not complete.*/
	return TWDR; /*Data is received.*/
}
uint8_t I2C_slaveAvialable()
{	
	TWCR  = (1<<TWEN) | (1<<TWINT) |(1<<TWEA); /*Start the operation of the I2C.*/
	while(READ_BIT ( TWCR , TWINT) == 0);
	if(TWSR == 0x60) 
	   return 1;
	else 
	   return 0;
}