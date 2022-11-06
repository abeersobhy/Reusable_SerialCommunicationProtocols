/*
 * I2C.h
 *
 * Created: 11/26/2014 6:45:32 PM
 *  Author: safifi
 */
#ifndef I2C_MASTER_H
#define I2C_MASTER_H
#define READ 1
#define WRITE 0
typedef enum
{
	NONE,
	START_TRANSMITED,
	REPEATED_START_TRANSMITED,
	SLA_W_TRANSMITTED_ACK_RECEIVED,
	SLA_W_TRANSMITTED_NOT_ACK_RECEIVED,
	DATA_TRANSMITTED_ACK_RECEIVED,
	DATA_TRANSMITTED_NOT_ACK_RECEIVED,
	ARIBITRATION_LOST,
	SLA_R_TRANSMITTED_ACK_RECEIVED,
	SLA_R_TRANSMITTED_NOT_ACK_RECEIVED,
	DATA_RECEIVED_ACK_RETURNED,
	DATA_RECEIVED_NOT_ACK_RETURNED,
	SLA_W_RECEIVED_ACK_RETURNED,
	SLA_R_RECEIVED_ACK_RETURNED,
	DATA_TWDR_TRANSMITTED_ACK_RETURNED,
}I2C_Status;
/*****************************************************************************
* Function Name: I2C_master_init
* Purpose      : init I2C to be master - generate SCL = 400K
* Parameters   : void
* Return value : void
*****************************************************************************/
void I2C_init(void);

/*****************************************************************************
* Function Name: I2C_master_start
* Purpose      : Send start condition to be a master and control the bus
* Parameters   : void
* Return value : void
*****************************************************************************/
void I2C_start(void);


/*****************************************************************************
* Function Name: I2C_master_addr
* Purpose      : Send first byte which have ID of the slave that master need to
*                talk with and set if master need to read or write.
* Parameters   : ID: id of the slave (1 to 112)
*                R_W: 0 for write and 1 for read
* Return value : void
*****************************************************************************/
void I2C_SLA_RW(uint8_t ID, uint8_t R_W);



/*****************************************************************************
* Function Name: I2C_master_write
* Purpose      : Send 1 byte data to slave
* Parameters   : data: 8 bit data should be sent to slave at the end of the function
* Return value : void
*****************************************************************************/
void I2C_Write(uint8_t data);


/*****************************************************************************
* Function Name: I2C_master_stop
* Purpose      : Send stop condition to release the bus 
* Parameters   : void
* Return value : void
*****************************************************************************/
void I2C_Stop(void);


/*****************************************************************************
* Function Name: I2C_slave_Init
* Purpose      : configure I2C to be a slave and set ID
* Parameters   : My_Add: id of the slave
* Return value : void
*****************************************************************************/
void I2C_SLA(uint8_t My_Add);


/*****************************************************************************
* Function Name: I2C_slave_avialable
* Purpose      : check if data will be come from master 
* Parameters   : void
* Return value : 1 if data will be came, 0 if not
*****************************************************************************/
/*****************************************************************************
* Function Name: I2C_slave_read
* Purpose      : Read data which is comming from master 
* Parameters   : void
* Return value : data 8 bit which came from master
*****************************************************************************/
void I2C_Read(uint8_t *data);

I2C_Status I2C_GitStatus(void);
#endif // I2C_MASTER_H