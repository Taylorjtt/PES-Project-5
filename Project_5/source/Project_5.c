
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

LoggerHandle logger;
RGBLEDHandle led;
UARTHandle uart;
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
	while(true)
	{
		if(UART_rxAvailable(uart))
		{
			UART_putChar(uart, UART_getChar(uart));
		}
	}
#endif


}
