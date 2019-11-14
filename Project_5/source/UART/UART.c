/*
 * UART.c
 *
 *  Created on: Nov 14, 2019
 *      Author: john
 */


#include "UART.h"
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


	return(uartHandle);
}

uint8_t UART_getChar(UARTHandle handle)
{
	UART_OBJ *uart = (UART_OBJ *)handle;

    while (!(uart->STATUS_1 & UART_S1_RDRF_MASK));

    return uart->DATA;
}

void UART_putChar(UARTHandle handle, uint8_t c)
{
	UART_OBJ *uart = (UART_OBJ *)handle;

      while(!(uart->STATUS_1 & UART_S1_TDRE_MASK));


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
