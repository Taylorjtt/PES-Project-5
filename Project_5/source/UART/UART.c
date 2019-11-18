/*
 * UART.c
 *
 *  Created on: Nov 14, 2019
 *      Author: john
 */


#include "UART.h"

volatile uint8_t nextByte;
volatile bool newData = false;
volatile RingBufferHandle rxRing;
volatile RingBufferHandle txRing;
volatile bool txBufferFull = false;
RGBLEDHandle ledHandle;

UARTHandle UART_constructor(void *pmemory, const size_t numBytes,uint32_t systemClock, uint32_t baud, RGBLEDHandle led)
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
	uart->CONTROL_2 |= UART_C2_RE_MASK | UART_C2_TE_MASK;
	ledHandle = led;

	rxRing = malloc(sizeof(RingBufferObject));
	RingBuffer_Constructor((void*)rxRing, sizeof(RingBufferObject), 4);

	txRing = malloc(sizeof(RingBufferObject));
	RingBuffer_Constructor((void*)txRing, sizeof(RingBufferObject), 4);


#ifdef INTERRUPT
	//enable interrupt
	EnableInterrupts;
	NVIC_EnableIRQ(UART0_IRQn);
	uart->CONTROL_2 |= UART_C2_RIE_MASK;
#endif

	return(uartHandle);
}
void UART_printf(UARTHandle handle, const char* format, ...)
{
	//http://www.firmcodes.com/write-printf-function-c/
	UART_OBJ *uart = (UART_OBJ *)handle;
	char *traverse;
	unsigned int i;
	char *s;

	//Module 1: Initializing Myprintf's arguments
	va_list arg;
	va_start(arg, format);

	for(traverse = format; *traverse != '\0'; traverse++)
	{
		while( *traverse != '%' && *traverse != '\0')
		{
			UART_queueChar(handle, (*traverse));
			traverse++;
		}
		if(*traverse == '\0')
		{
			va_end(arg);
			break;
		}


		traverse++;

		//Module 2: Fetching and executing arguments
		switch(*traverse)
		{
			case 'c' : i = va_arg(arg,int);			//Fetch char argument
				UART_queueChar(handle, i);
				break;

			case 'd' : i = va_arg(arg,int); 		//Fetch Decimal/Integer argument
				if(i<0)
				{
					i = -i;
					UART_queueChar(handle, i);
				}
				UART_queueString(uart, (convert(i,10)));
				break;

			case 'o': i = va_arg(arg,unsigned int); //Fetch Octal representation
				UART_queueString(uart, (convert(i,8)));
				break;

			case 's':
				s = va_arg(arg,char *); 		//Fetch string
				UART_queueString(uart, s);
				break;

			case 'x': i = va_arg(arg,unsigned int); //Fetch Hexadecimal representation
				UART_queueString(uart, (convert(i,16)));
				break;
		}
	}

	//Module 3: Closing argument list to necessary clean-up
	va_end(arg);
}
uint8_t UART_getChar(UARTHandle handle)
{
	UART_OBJ *uart = (UART_OBJ *)handle;
	RGBLED_set(ledHandle, false, false, true);
#ifndef INTERRUPT
	while (!UART_rxAvailable(uart));
#endif
	return uart->DATA;
}

char *convert(unsigned int num, int base)
{
	//http://www.firmcodes.com/write-printf-function-c/
	static char Representation[]= "0123456789ABCDEF";
	static char buffer[50];
	char *ptr;

	ptr = &buffer[49];
	*ptr = '\0';

	do
	{
		*--ptr = Representation[num%base];
		num /= base;
	}while(num != 0);

	return(ptr);
}
void UART_putChar(UARTHandle handle, uint8_t c)
{
	UART_OBJ *uart = (UART_OBJ *)handle;
	RGBLED_set(ledHandle, false, true, false);
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
void UART_queueString(UARTHandle handle,char* string)
{
	int i = 0;
	while(string[i] != '\0')
	{
		UART_queueChar(handle, string[i++]);
	}
}
void UART_queueChar(UARTHandle handle,char c)
{
	if(RingBuffer_push(txRing, c) == BUFFER_FULL_ERROR)
	{
		txBufferFull = true;
	}
#ifdef INTERRUPT
	UART_enableTXInterrupt(handle);
#endif
}
bool UART_transmitComplete(UARTHandle handle)
{
	UART_OBJ *uart = (UART_OBJ *)handle;
	return uart->STATUS_1 & UART0_S1_TC_MASK;
}
void UART_enableTXInterrupt(UARTHandle handle)
{
	UART_OBJ *uart = (UART_OBJ *)handle;
	uart->CONTROL_2 |=  UART_C2_TIE_MASK;
}
void UART_disableTXInterrupt(UARTHandle handle)
{
	UART_OBJ *uart = (UART_OBJ *)handle;
	uart->CONTROL_2 &= ~UART_C2_TIE_MASK;
}

#ifdef INTERRUPT
void UART0_IRQHandler (void)
{
	DisableInterrupts;
	UARTHandle uartHandle = (UARTHandle)UART0_BASE;
	UART_OBJ *uart = (UART_OBJ *)uartHandle;

	if(uart->STATUS_1 & UART_S1_RDRF_MASK && uart->CONTROL_2 & UART_C2_RIE_MASK)
	{
		//the recieve data buffer is full (there is a byte to grab)
#ifdef ECHO
		nextByte = UART_getChar(uart);
		newData = true;
#endif
#ifdef APPLICATION
		RingBuffer_push(rxRing, UART_getChar(uart));
		newData = true;
#endif

	}

	if(uart->STATUS_1 & UART_S1_TDRE_MASK && uart->CONTROL_2 & UART_C2_TIE_MASK)
	{
		//the transmit data register is empty (we are free to send another byte)
#ifdef ECHO
		if(newData)
		{
			UART_putChar(uart, nextByte);
			newData = false;
		}
#endif
#ifdef APPLICATION
		//put the latest character on the bus
		if(!RingBuffer_isEmpty(txRing))
		{
			UART_putChar(uart, RingBuffer_peek(txRing));
			//wait for the transmission to complete
			while(!(UART_transmitComplete(uart)));
			//pop the sent value off of the tx buffer
			RingBuffer_pop(txRing);
		}

		//disable the tx ready interrupt
		uart->CONTROL_2 &= ~UART_C2_TIE_MASK;
#endif



	}
	EnableInterrupts;

}
#endif
