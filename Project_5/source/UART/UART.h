/*
 * UART.h
 *
 *  Created on: Nov 14, 2019
 *      Author: john
 */

#ifndef UART_UART_H_
#define UART_UART_H_
#define INTERRUPT
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "CMSIS/MKL25Z4.h"

typedef struct _UART_OBJ_
{
	__IO uint8_t BAUD_HIGH;
	__IO uint8_t BAUD_LOW;
	__IO uint8_t CONTROL_1;
	__IO uint8_t CONTROL_2;
	__IO uint8_t STATUS_1;
	__IO uint8_t STATUS_2;
	__IO uint8_t CONTROL_3;
	__IO uint8_t DATA;
	__IO uint8_t MATCH_ADDRESS_1;
	__IO uint8_t MATCH_ADDRESS_2;
	__IO uint8_t CONTROL_4;
	__IO uint8_t CONTROL_5;


}UART_OBJ;

typedef struct _UART_OBJ_ *UARTHandle;
UARTHandle UART_constructor(void *pmemory, const size_t numBytes,uint32_t systemClock, uint32_t baud);
uint8_t UART_getChar(UARTHandle handle);
void UART_putChar(UARTHandle handle, uint8_t c);
bool UART_txAvailable(UARTHandle handle);
bool UART_rxAvailable(UARTHandle handle);
#endif /* UART_UART_H_ */
