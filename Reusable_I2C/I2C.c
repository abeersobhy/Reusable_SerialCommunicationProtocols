#include "../../Utilities/bit_math.h"
#include "../../Utilities/std_types.h"
#include "../../Utilities/register.h"
#include "I2C.h"
#define F_CPU 16000000UL
#define F_SCL 400000UL // SCL frequency
#define Prescaler 1
#define TWBR_val ((((F_CPU / F_SCL) / Prescaler) - 16 ) / 2)

void I2C_init(void)
{
	TWBR = TWBR_val; // for 400K scl and 16Mhz freq uc
}
void I2C_start()
{
	// transmit START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	// wait for end of transmission
	while( READ_BIT(TWCR,TWINT) == 0);
}
void I2C_SLA_RW(uint8_t addr , uint8_t RW)
{ // for write op you should put 0 on LSB
	// load slave address into data register
	TWDR = addr<<1; // write operation
	if(RW == READ) /**/
	{
		TWDR |= 1;
	}
	// start transmission of address with acknowledge
	TWCR = (1<<TWINT) | (1<<TWEN) | (1 << TWEA);
	// wait for end of transmission
	while( READ_BIT(TWCR,TWINT) == 0);
}
void I2C_Write(uint8_t data)
{
	// load data into data register
	TWDR = data;
	// start transmission of data with acknowledge
	TWCR = (1<<TWINT) | (1<<TWEN) | (1 << TWEA);
	// wait for end of transmission
	while( READ_BIT(TWCR,TWINT) == 0);
}
void I2C_Stop(void)
{
	// transmit STOP condition
	TWCR = (1<<TWINT) | (1<<TWSTO) | 1<<TWEN;
}
void I2C_SLA(uint8_t My_Add)
{
	TWAR = My_Add<<1;
}
void I2C_Read(uint8_t *data)
{
	// start TWI module and acknowledge data after reception
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	// wait for end of transmission
	while( READ_BIT(TWCR,TWINT) == 0);
	// return received data from TWDR
	*data = TWDR;
}
I2C_Status I2C_GitStatus()
{
	TWCR  = (1<<TWEN) | (1<<TWINT) |(1<<TWEA);
	while(READ_BIT(TWCR,TWINT) == 0);
	switch(TWSR)
	{
		case 0x08: return START_TRANSMITED;                   break;
		case 0x18: return SLA_W_TRANSMITTED_ACK_RECEIVED;     break;
		case 0x20: return SLA_W_TRANSMITTED_NOT_ACK_RECEIVED; break;
		case 0x28: return DATA_TRANSMITTED_ACK_RECEIVED;      break;
		case 0x30: return DATA_TRANSMITTED_NOT_ACK_RECEIVED;  break;
		case 0x38: return ARIBITRATION_LOST;                  break;
		case 0x40: return SLA_R_TRANSMITTED_ACK_RECEIVED;     break;
		case 0x48: return SLA_R_TRANSMITTED_NOT_ACK_RECEIVED; break;
		case 0x50: return DATA_RECEIVED_ACK_RETURNED;         break;
		case 0x58: return DATA_RECEIVED_NOT_ACK_RETURNED;     break;
		case 0x60: return SLA_W_RECEIVED_ACK_RETURNED;        break;
		case 0xA8: return SLA_R_RECEIVED_ACK_RETURNED;        break;
		default:   return NONE;                               break;
	}
}
uint8_t I2C_slave_avialable()
{
	TWCR  = (1<<TWEN) | (1<<TWINT) |(1<<TWEA);
	while( READ_BIT(TWCR,TWINT) == 0);
	if(TWSR == 0x60) return 1;
	else return 0;
}