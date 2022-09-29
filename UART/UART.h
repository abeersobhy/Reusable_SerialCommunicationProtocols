/*
 * UART.h
 *
 * Created: 29/09/2022 02:23:24 م
 *  Author: user
 */ 
/*
   Preprocessor configurable driver
   Linker configurable driver
   Post build configurable driver "During run time"
*/
#ifndef UART_H_
#define UART_H_

#include <avr/interrupt.h>
#include "../../Utilities/bit_math.h"

#define NULL '\0'
#define F_CPU 1000000UL

typedef enum /*Enum to select single and multi communication .*/
{
	USART_singleProcessor,
	USART_multiProcessor,
}USART_CommunicationTerminal;
typedef enum /*Enum to select the speed and the communication mode.*/
{
	USART_Asynch_normalSpeed,
	USART_Asynch_doubleSpeed,
	USART_Synch,	
}USART_communicationMode;
typedef enum /*Enum to control the USART receiving and transmitting.*/
{
	USART_disAble,
	USART_TX_Only,
	USART_RX_Only,
	USART_RX_TX,	
}USART_EnableMode;
typedef enum /*Enum to control the interrupt source.*/
{
	USART_InterruptDisable,
	USART_RXComp_Enable,
	USART_TXComp_Enable,
	USART_UDREmpty_Enable,
	USRT_RXComp_TXComp_Enable,
	USRT_RXComp_UDREmpty_Enable,
	USRT_TXComp_UDREmpty_Enable,
	USRT_RXComp_TXComp_UDREmpty_Enable,
}USART_InterruptSorce;
typedef enum /*Enum to control the size of data character.*/
{
	USART_5bitData,
	USART_6bitData,
	USART_7bitData,
	USART_8bitData,
	USART_9bitData,
}USART_DataFrame;
typedef enum /*Enum to control parity and number of stop bit.*/
{
	USART_ParityDisable_1stop,
	USART_ParityDisable_2stop,
	USART_ParityEven_1stop,
	USART_ParityEven_2stop,
	USART_ParityOdd_1stop,
	USART_ParityOdd_2stop,
}USART_ControlFrame;
typedef enum /*Synchronous mode sampling with rising or falling edge.*/
{
	USART_SampleOnFallingEdge,
	USART_SampleOnRisingEdge,
}USART_ClockPolarity;
/*PLease refer to the upper Enums for more details on what to select.*/
typedef enum /*Enum to handle the USART receiving errors*/
{
	USART_NoError,
	USART_PartyError,
	USART_FrameError,
	USART_DataOverRunError,
}USART_ErorrState;
typedef struct  
{
	USART_EnableMode            enableMode;
	USART_communicationMode     communicationMode;
	USART_CommunicationTerminal communicationTerminal;
	USART_InterruptSorce        interruptSource;
	USART_DataFrame             dataFrame;	
	USART_ControlFrame          controlFrame;
	USART_ErorrState            errorState;
	/*Assign this option only when working on synchronouse mode.*/
	USART_ClockPolarity         clockPolarity;
	/*Baud rate variable*/
	uint64_t                    baudRate; /*10 bit for baud rate.*/
    /*Interrupt Call back members*/
	void (*RX_InterruptCall)(void); /*Make this pointer points to the function you want to called with RX_Interrupt*/   
	void (*TX_InterruptCall)(void); /*Make this pointer points to the function you want to called with TX_Interrupt*/
	void (*UDR_InterruptCall)(void); /*Make this pointer points to the function you want to called with UDR_Interrupt*/
}USART_ConfigurationStruct;

extern USART_ConfigurationStruct USART0; 
/************************************************
* Function name : USART_Init                     
* Function arguments : void                     
* returned value : void                         
* Functionality  : initialize the USART protocol
**************************************************/
void USART_Init(void);
/************************************************
* Function name : USART_sendData_Blocking       
* Function arguments : uint16_t variable (data) 
* returned value : void                         
* Functionality  : Asynchronous function to send data
*************************************************/
void USART_sendData_Blocking(uint16_t data);
/************************************************
* Function name : USART_sendData_NonBlocking
* Function arguments : uint16_t variable (data)
* returned value : void
* Functionality  : synchronous function to send data (using in ISR function)
*************************************************/
void USART_sendData_NonBlocking(uint16_t data);
/************************************************
* Function name : USART_receiveData_Blocking
* Function arguments : void
* returned value : uint16_t variable (data) - received data
* Functionality  : Asynchronous function to receive data
*************************************************/
uint16_t USART_receiveData_Blocking(void);
/************************************************
* Function name : USART_receiveData_NonBlocking
* Function arguments : void
* returned value : received data (uint16_t variable)
* Functionality  : synchronous function to receive data (using in ISR function)
*************************************************/
uint16_t USART_receiveData_NonBlocking(void);
        
#endif /* UART_H_ */