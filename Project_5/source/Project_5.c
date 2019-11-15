
/**
 * @file    Project_5.c
 * @brief   Application entry point.
 */
#include "Project_5.h"
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "UART/UART.h"
#include <ctype.h>

LoggerHandle logger;
RGBLEDHandle led;
UARTHandle uart;

uint32_t count[128] = {0};

int main(void) {

	/* Init board hardware. */

	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
	//10hz
	SysTick_Config(4800000);

	led = malloc(sizeof(RGBLEDObject));
	led = RGBLED_Constructor((void*) led, sizeof(RGBLEDObject), RED_BASE, RED_PIN, GREEN_BASE, GREEN_PIN, BLUE_BASE, BLUE_PIN);
	RGBLED_set(led, false, false, true);



	uart = malloc(sizeof(UART_OBJ));
	uart = UART_constructor((void *)UART0_BASE, sizeof(UART_OBJ),24000,115200,led);

	logger = malloc(sizeof(LOGGERObject));
	logger = Logger_Constructor((void*)logger, sizeof(LOGGERObject), uart);
	Logger_enable(logger);
	Logger_logString(logger, "Program Started", "main", STATUS_LEVEL);



#ifdef TEST
	testRingBuffer();
	exit(0);
#endif


#ifdef ECHO
#ifndef INTERRUPT
	Logger_logString(logger, "Echo Mode with Polling", "main", STATUS_LEVEL);
	while(true)
	{
		UART_putChar(uart, UART_getChar(uart));
	}
#else
	Logger_logString(logger, "Echo Mode with Interrupts", "main", STATUS_LEVEL);
	while(true)
	{

	}
#endif
#endif

#ifdef APPLICATION
	#ifdef INTERRUPT
	Logger_logString(logger, "Application Mode with Interrupts", "main", STATUS_LEVEL);
	#else
	Logger_logString(logger, "Application Mode with Polling", "main", STATUS_LEVEL);
	#endif
	while(true)
	{
		#ifndef INTERRUPT
		if(RingBuffer_push(rxRing, UART_getChar(uart))== BUFFER_FULL_ERROR)
		{
			Logger_logString(logger, "Receive Buffer has been resized", "main", STATUS_LEVEL);
			UART_printf(uart, "New size is : %d Bytes\n\r",RingBuffer_getSize(rxRing));
		}
		#endif
		//if the receive buffer isn't empty process the data in it
		if(!RingBuffer_isEmpty(rxRing))
		{
			//grab the data
			uint8_t data = RingBuffer_peek(rxRing);

			if(isalnum(data) || ispunct(data))
			{
				//if its alphanumeric or punctuation add it to the count data structure
				//that keeps track of how many times a particular character has been received
				count[data]++;
			}
			else if(data == ESCAPE_CHARACTER)
			{
				Logger_logString(logger, "Report Generation Initiated", "main", STATUS_LEVEL);
				//if the data is the escape character (27) print out the report
				for(int i = 0; i < 128; i++)
				{
					//for each ASCII value in the array print out a report of how many times it has been received if it
					//has be received more than 0 times
					if(count[i] > 0)
					{
						UART_printf(uart, "[%c-%d]",(char)i,count[i]);

					}


				}
				UART_printf(uart, "\n\r");
				Logger_logString(logger, "Report Generation Finished", "main", STATUS_LEVEL);


			}
			RingBuffer_pop(rxRing);
		}

		if(!RingBuffer_isEmpty(txRing))
		{

			RGBLED_set(led, false, true, false);
			#ifdef INTERRUPT
			UART_enableTXInterrupt(uart);
			#else
			UART_putChar(uart, RingBuffer_peek(txRing));
			RingBuffer_pop(txRing);
			#endif

		}
		else
		{
			RGBLED_set(led, false, false, true);
		}
		if(txBufferFull)
		{
			txBufferFull = false;
			Logger_logString(logger, "Transmit Buffer has been resized", "main", STATUS_LEVEL);
			UART_printf(uart, "New size is : %d Bytes\n\r",RingBuffer_getSize(txRing));
		}

	}
#endif
}
