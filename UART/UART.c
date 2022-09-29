#include "UART.h"

USART_ConfigurationStruct USART0 = {0,
	                                .RX_InterruptCall = NULL,
					.TX_InterruptCall = NULL,
					.UDR_InterruptCall = NULL
									};
void USART_Init(void)
{
	/*Variables used to assign the data of USART registers to it with zeros as initial value.*/
	uint8_t UCSRA_Temp = 0;
	uint8_t UCSRB_Temp = 0;
	uint8_t UCSRC_Temp = 0;	
	volatile float UBRR_Temp = 0; 
	switch(USART0.enableMode) /*check the selected mode in the enable mode enum.*/
	{
		case USART_disAble :  /*Nothing to do.*/
		break;
		case USART_RX_Only : /*Enable receiver.*/
		     UCSRB_Temp |= (1 << RXEN);
		break;
		case USART_TX_Only : /*Enable Transmitter.*/
		     UCSRB_Temp |= (1 << TXEN);
		break;
		case USART_RX_TX :   /*Enable both of receiver and transmitter.*/
		     UCSRB_Temp |= (1 << RXEN) | (1 << TXEN);
	    break;
		default:
		break;
	}
	switch(USART0.communicationMode) /*Check on the selected communication mode enum.*/
	{
		case USART_Asynch_normalSpeed : /*Only set the baud rate value - Asynchronous normal speed mode is active by default.*/
		     UBRR_Temp = (((float)F_CPU) / (16.0 * USART0.baudRate)) - 0.5; /*By Adding (0.5) to the fixed function to convert the number to the closest decimal number.*/
		break;
		case USART_Asynch_doubleSpeed : /*Enable double speed with asynchronous then set the baud rate value.*/
		     UCSRA_Temp |= (1 << U2X);
			 UBRR_Temp = (((float)F_CPU) / (8.0 * USART0.baudRate)) - 0.5; /*By Adding (0.5) to the fixed function to convert the number to the closest decimal number.*/
		break;
		case USART_Synch :              
		     UCSRC_Temp |= (1 << UMSEL);
			 UBRR_Temp = (((float)F_CPU) / (2.0 * USART0.baudRate)) - 0.5; /*By Adding (0.5) to the fixed function to convert the number to the closest decimal number.*/
	         switch(USART0.clockPolarity) /*Check the selected clock polarity*/
	         {
		         case USART_SampleOnFallingEdge : /*Use Sample with falling edge.*/
		         UCSRC_Temp |= (1 << UCPOL);
		         break;
		         case USART_SampleOnRisingEdge : /*Sampling with rising edge is already set by default.*/
		         break;
		         default:
		         break;
	         }	
		break;
		default:
		break;         
	}
	switch(USART0.communicationTerminal) /*Check the communication mode selected*/
	{
		case USART_singleProcessor : /*Single processor mode is selected.*/
		break; 
		case USART_multiProcessor  : /*Multi processor mode is selected.*/
		     UCSRA_Temp |= (1 << MPCM);
		break;
		default:
		break;
	}
	switch(USART0.interruptSource) /*Check the source of the interrupt.*/
	{
		case USART_InterruptDisable :  /*Disable the USART interrupt.*/         
		break;
		case USART_RXComp_Enable :  /*Enable USART interrupt with complete received data*/              
		     UCSRB_Temp |= (1 << RXCIE);
		break;
		case USART_TXComp_Enable : /*Enable USART interrupt with complete transmitted data*/               
	    	 UCSRB_Temp |= (1 << TXCIE);
		break;
		case USART_UDREmpty_Enable :  /*Enable USART when the UDR register is empty.*/            
	    	 UCSRB_Temp |= (1 << UDRIE);
		break;	
		case USRT_RXComp_TXComp_Enable :   /*Enable USART interrupt with complete received data and complete transmitted data*/  
    		 UCSRB_Temp |= (1 << RXCIE) | (1 << TXCIE);
		break;
		case USRT_RXComp_UDREmpty_Enable :  /*Enable USART interrupt with complete received data and when the UDR register is empty */       
    		 UCSRB_Temp |= (1 << RXCIE) | (1 << UDRIE);
		break;
		case USRT_TXComp_UDREmpty_Enable :  /*Enable USART interrupt with complete transmitted data and when the UDR register is empty*/      
        	 UCSRB_Temp |= (1 << TXCIE) | (1 << UDRIE);
		break;
		case USRT_RXComp_TXComp_UDREmpty_Enable :  /*Enable USART interrupt with complete received data and complete transmitted data and when the UDR register is empty*/  
	       	 UCSRB_Temp |= (1 << TXCIE) | (1 << RXCIE) | (1 << UDRIE);
		break;
		default:
		break;
		
	}
	switch(USART0.dataFrame) /*Check the selected size of the character */
	{
		case USART_5bitData : /*5 bit character*/
		break;
		case USART_6bitData :  /*6 bit character*/
		     UCSRC_Temp |= (1 << UCSZ0);
		break;	 
		case USART_7bitData : /*7 bit character*/
		     UCSRC_Temp |= (1 << UCSZ1);
		break;	  
		case USART_8bitData : /*8 bit character*/
		     UCSRC_Temp |= (1 << UCSZ0) | (1 << UCSZ1);
		break;	 
		case USART_9bitData : /*9 bit character*/
		     UCSRC_Temp |= (1 << UCSZ0) | (1 << UCSZ1);
			 UCSRB_Temp |= (1 << UCSZ2);
	}
	switch(USART0.controlFrame) /*Check the selected frame of control.*/
	{
		case USART_ParityDisable_1stop :  /*Disable parity with 1 stop bit*/
		break; /*Nothing to do as it is already set by default.*/
		case USART_ParityDisable_2stop : /*Disable parity with 2 stop bit*/
		     UCSRC_Temp |= (1 << USBS);
		break;	 
		case USART_ParityEven_1stop : /*Enable even parity with 1 stop bit.*/
		     UCSRC_Temp |= (1 << UPM1);
		case USART_ParityEven_2stop : /*Enable even parity with 2 stop bit.*/
		     UCSRC_Temp |= (1 << UPM1) | (1 << USBS);
		break;	 
		case USART_ParityOdd_1stop : /*Enable odd parity with 1 stop bit.*/
		     UCSRC_Temp |= (1 << UPM0) | (1 << UPM1);
		break;
		case USART_ParityOdd_2stop : /*Enable odd parity with 2 stop bit.*/
		     UCSRC_Temp |= (1 << UPM0) | (1 << UPM1) | (1 << USBS);
		break;
		default:
		break;	 
	}
	/*Set the selected baud rate to the UBRR register*/
	
	UBRRH = (((uint16_t)UBRR_Temp) >> 8); /*Cast the USART0.baudRate to 16 bit character then get only the first two bits (8 , 9).*/
	UBRRL = ((uint8_t)UBRR_Temp); /*Cast the USART0.baudRate to 8 bit character*/
	
	UCSRA = UCSRA_Temp; /*Assign the new value of UCSRA_Temp to the UCSRA Register*/
	UCSRC = UCSRC_Temp; /*Assign the new value of UCSRC_Temp to the UCSRC Register*/
	UCSRB = UCSRB_Temp; /*Assign the new value of UCSRB_Temp to the UCSRB Register*/
}
void USART_sendData_Blocking(uint16_t data) /*Synchronous function to send data*/
{
	while(READ_BIT(UCSRA , UDRE) == 0); /*iterate as long as the UDR register is not empty.*/
	if(USART0.dataFrame  == USART_9bitData) /*9 bit data  frame is used.*/
	{
		CLR_BIT(UCSRB , TXB8); /*Make sure that the 9th bit is cleared before writing*/
		UCSRB |= (data >> 8); /*Set the 9th bit to the first bit of UCSRB.*/
	}
	UDR = (uint8_t)data; /*Set the first 8 bit data to the UDR.*/
}
void USART_sendData_NonBlocking(uint16_t data) /*Asynchronous send data function for ISR using*/
{
	if(READ_BIT(UCSRA , UDRE) != 0); /*As the interrupt is already done check the flag for one time.*/
	{
		if(USART0.dataFrame  == USART_9bitData) /*9 bit data  frame is used.*/
		{
			CLR_BIT(UCSRB , TXB8); /*Make sure that the 9th bit is cleared before writing*/
			UCSRB |= (data >> 8); /*Set the 9th bit to the first bit of UCSRB.*/
		}
		UDR = (uint8_t)data; /*Set the first 8 bit data to the UDR.*/	
	}
}
uint16_t USART_receiveData_Blocking(void) /*Synchronous function to receive data*/
{
	uint16_t data = 0;
	while(READ_BIT(UCSRA , RXC) == 0); /*iterate as long as the UDR register is empty.*/
	if(READ_BIT(UCSRA , PE)) /* The parity error flag is raised.*/
	{
		USART0.errorState = USART_PartyError;
	}
	else if(READ_BIT(UCSRA , FE)) /* The data frame flag is raised.*/
	{
		USART0.errorState = USART_FrameError;
	}
	else if(READ_BIT(UCSRA , DOR)) /* The data over run flag is raised.*/
	{
		USART0.errorState = USART_DataOverRunError;
	}
	if(USART0.dataFrame  == USART_9bitData) /*9 bit data frame is used.*/
	{
		data |= (UCSRB >> RXB8); /*Read the RXB8 bit then assign it to the data variable*/
		data <<= 8; /*Shift the data variable to the left with 8 bit.*/
	}
	data |= UDR; /*Assign the UDR data register to the data variable.*/
	return data; /*return the received data.*/
}
uint16_t USART_receiveData_NonBlocking(void) /*Asynchronous receive data function for ISR using*/
{
	uint16_t data = 0;
	if(READ_BIT(UCSRA , RXC) != 0) /*As the interrupt is already done check the flag for one time.*/
	{
		if(READ_BIT(UCSRA , PE)) /* The parity error flag is raised.*/
		{
			USART0.errorState = USART_PartyError;
		}
		else if(READ_BIT(UCSRA , FE)) /* The data frame flag is raised.*/
		{
			USART0.errorState = USART_FrameError;
		}
		else if(READ_BIT(UCSRA , DOR)) /* The data over run flag is raised.*/
		{
			USART0.errorState = USART_DataOverRunError;
		}
		else /*None of the previous flags is raised.*/
		{
			USART0.errorState = USART_NoError; /*No error*/
			if(USART0.dataFrame  == USART_9bitData) /*9 bit data frame is used.*/
			{
				data |= (UCSRB >> RXB8); /*Read the RXB8 bit then assign it to the data variable*/
				data <<= 8; /*Shift the data variable to the left with 8 bit.*/
			}
			data |= UDR; /*Assign the UDR data register to the data variable.*/
		}	
	}
	return data; /*return the received data.*/
}
/*ISR(_VECTORS(13))*/
ISR(USART_RXC_vect)
{
	if(USART0.RX_InterruptCall != NULL)
	{
		USART0.RX_InterruptCall();
	}
	else
	{
		/*Error handling function.*/
	}
}
ISR(USART_TXC_vect)
{
	if(USART0.TX_InterruptCall != NULL)
	{
		USART0.TX_InterruptCall();
	}
	else
	{
		/*Error handling function.*/
	}
}
ISR(USART_UDRE_vect)
{
	if(USART0.UDR_InterruptCall != NULL)
	{
		USART0.UDR_InterruptCall();
	}
	else
	{
		/*Error handling function.*/
	}
}
