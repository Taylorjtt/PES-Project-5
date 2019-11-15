/*
 * UART.c
 *
 *  Created on: Nov 14, 2019
 *      Author: john
 */


#include "UART.h"

volatile uint8_t nextByte;
volatile bool newData = false;

UARTHandle UART_constructor(void *pmemory, const size_t numBytes,uint32_t systemClock, uint32_t baud)
{
	UARTHandle uartHandle;


	if(numBytes < sizeof(UART_OBJ))
	{
		return((UARTHandle)NULL);
	}

	// assign the handle
	uartHandle = (UARTHandle)pmemory;

	UART_OBJ *uart = (UART_OBJ *)uartHandle;

	//disable both the tx and rx
	uart->CONTROL_2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);

	//configure for 8 bit no parity
	uart->CONTROL_1 = 0;

	uint16_t sysBaud = (uint16_t)((systemClock*1000)/(baud * 16));

	uint8_t buffer = uart->BAUD_HIGH & ~(UART_BDH_SBR(0x1F));

	uart->BAUD_HIGH = buffer |  UART_BDH_SBR(((sysBaud & 0x1F00) >> 8));
	uart->BAUD_LOW = (uint8_t)(sysBaud & UART_BDL_SBR_MASK);

   //Enable rx and tx
   uart->CONTROL_2|= (UART_C2_TE_MASK| UART_C2_RE_MASK );

#ifdef INTERRUPT
   	   //enable interrupt
   	   EnableInterrupts;
   	   NVIC_EnableIRQ(UART0_IRQn);
   	   uart->CONTROL_2 |= UART_C2_RIE_MASK | UART_C2_TIE_MASK;
#endif

	return(uartHandle);
}

uint8_t UART_getChar(UARTHandle handle)
{
	UART_OBJ *uart = (UART_OBJ *)handle;
	#ifndef INTERRUPT
		while (!UART_rxAvailable(uart));
	#endif
    return uart->DATA;
}

void UART_putChar(UARTHandle handle, uint8_t c)
{
	UART_OBJ *uart = (UART_OBJ *)handle;
	#ifndef INTERRUPT
		while(!UART_txAvailable(uart));
	#endif
	uart->DATA = (uint8_t)c;

 }
bool UART_txAvailable(UARTHandle handle)
{
	UART_OBJ *uart = (UART_OBJ *)handle;
	if(uart->STATUS_1 & UART_S1_TDRE_MASK)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool UART_rxAvailable(UARTHandle handle)
{
	UART_OBJ *uart = (UART_OBJ *)handle;
	if(uart->STATUS_1 & UART_S1_TDRE_MASK)
	{
		return true;
	}
	else
	{
		return false;
	}
}

#ifdef INTERRUPT
void UART0_IRQHandler (void)
{
	UARTHandle uartHandle = (UARTHandle)UART0_BASE;
	UART_OBJ *uart = (UART_OBJ *)uartHandle;

	if(uart->STATUS_1 & UART_S1_RDRF_MASK)
	{
		//the recieve data buffer is full (there is a byte to grab)
		nextByte = UART_getChar(uart);
		newData = true;

	}

	if(uart->STATUS_1 & UART_S1_TDRE_MASK)
	{
		//the transmit data register is empty (we are free to send another byte)
		if(newData)
		{
			UART_putChar(uart, nextByte);
			newData = false;
		}


	}


}
#endif
