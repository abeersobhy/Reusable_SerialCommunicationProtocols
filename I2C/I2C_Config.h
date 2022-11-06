/*
 * I2C_Config.h
 *
 * Created: 24/10/2022 09:15:50 م
 *  Author: user
 */ 


#ifndef I2C_CONFIG_H_
#define I2C_CONFIG_H_

/*Status Codes for Master Transmitter Mode*/
#define START_TRANSMITED                        0x08
#define REPEATED_START_TRANSMITED               0x10
#define SLA_W_TRANSMITTED_ACK_RECEIVED          0x18 
#define SLA_W_TRANSMITTED_NOT_ACK_RECEIVED      0x20
#define DATA_TRANSMITTED_ACK_RECEIVED           0x28
#define DATA_TRANSMITTED_NOT_ACK_RECEIVED       0x30
#define ARIBITRATION_LOST                       0x38   
/*Status Codes for Master Receiver Mode*/
#define START_TRANSMITED                        0x08
#define ARIBITRATION_LOST                       0x38
#define SLA_R_TRANSMITTED_ACK_RECEIVED          0x40
#define SLA_R_TRANSMITTED_NOT_ACK_RECEIVED      0x48
#define DATA_RECEIVED_ACK_RETURNED              0x50
#define DATA_RECEIVED_NOT_ACK_RETURNED          0x58

/*Status Codes for Slave Receiver Mode */

/*Status Codes for Slave Transmitter Mode*/
#endif /* I2C_CONFIG_H_ */