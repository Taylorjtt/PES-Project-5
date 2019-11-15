
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
	SysTick_Config(480);

	led = malloc(sizeof(RGBLEDObject));
	led = RGBLED_Constructor((void*) led, sizeof(RGBLEDObject), RED_BASE, RED_PIN, GREEN_BASE, GREEN_PIN, BLUE_BASE, BLUE_PIN);
	RGBLED_set(led, false, false, false);

	logger = malloc(sizeof(LOGGERObject));
	logger = Logger_Constructor((void*)logger, sizeof(LOGGERObject));
	Logger_enable(logger);

	uart = malloc(sizeof(UART_OBJ));
	uart = UART_constructor((void *)UART0_BASE, sizeof(UART_OBJ),24000,115200);



#ifdef TEST
	testRingBuffer();
	exit(0);
#endif


#ifdef ECHO
#ifndef INTERRUPT
	while(true)
	{
		UART_putChar(uart, UART_getChar(uart));
	}
#else
	while(true)
	{

	}
#endif
#endif

#ifdef APPLICATION
	while(true)
	{
		#ifndef INTERRUPT
		RingBuffer_push(rxRing, UART_getChar(uart));
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
				//if the data is the escape character (27) print out the report
				UART_queueString(uart, "\n\r");
				UART_queueString(uart, "Begin Report");
				UART_queueString(uart, "\n\r");
				for(int i = 0; i < 128; i++)
				{
					//for each ASCII value in the array print out a report of how many times it has been received if it
					//has be received more than 0 times
					if(count[i] > 0)
					{
						UART_queueChar(uart, '[');
						UART_queueChar(uart, (char)i);
						UART_queueString(uart,"-");
						char num[5];
						sprintf(num, "%d", count[i]);
						UART_queueString(uart, num);
						UART_queueChar(uart, ']');

					}

				}
				UART_queueString(uart, "\n\r");
				UART_queueString(uart, "End Report");
				UART_queueString(uart, "\n\r");


			}
			RingBuffer_pop(rxRing);
		}

		if(!RingBuffer_isEmpty(txRing))
		{
			#ifdef INTERRUPT
			UART_enableTXInterrupt(uart);
			#else
			UART_putChar(uart, RingBuffer_peek(txRing));
			RingBuffer_pop(txRing);
			#endif

		}

	}
#endif
}
