
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
#ifndef INTERRUPT
	while(true)
	{
		UART_putChar(uart, UART_getChar(uart));
	}
#else
	while(true)
	{


		if(!RingBuffer_isEmpty(rxRing))
		{
			uint8_t data = RingBuffer_peek(rxRing);
			if(isalnum(data) || ispunct(data))
			{
				count[data]++;
			}
			else if(data == 27)
			{
				UART_queueString(uart, "\n\r");
				UART_queueString(uart, "Begin Report");
				UART_queueString(uart, "\n\r");
				for(int i = 0; i < 128; i++)
				{
					if(count[i] > 0)
					{
						UART_queueChar(uart, '[');
						UART_queueChar(uart, (char)i);
						UART_queueString(uart," - ");
						char num[5];
						sprintf(num, "%d", count[i]);
						UART_queueString(uart, num);
						UART_queueChar(uart, ']');
						if(i != 127)
						{
							UART_queueString(uart, ", ");
						}

						UART_enableTXInterrupt(uart);
					}

				}
				UART_queueString(uart, "\n\r");
				UART_queueString(uart, "End Report");
				UART_queueString(uart, "\n\r");
				UART_enableTXInterrupt(uart);

			}
			RingBuffer_pop(rxRing);
		}
	}
#endif
#endif
}
