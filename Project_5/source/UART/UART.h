/*
 * UART.h
 *
 *  Created on: Nov 14, 2019
 *      Author: john
 */

#ifndef UART_UART_H_
#define UART_UART_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "CMSIS/MKL25Z4.h"
#include "CMSIS/core_cm0plus.h"
#include "util.h"
#include "RingBuffer/RingBuffer.h"
#include "LED/RGB.h"
#include "mode.h"
#include <stdarg.h>


extern volatile RingBufferHandle rxRing;
extern volatile RingBufferHandle txRing;

extern volatile bool txBufferFull;
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
/**
* @brief Constructor for UART object
*
* Given a chunk of memory, the size of an UART object, initialize a UART
*
* @param pmemory		The pointer to the memory that this object will use
* @param numbytes		The expected size of the object
* @param systemClock	System clock speed for baud calculation
* @param baud			desired baud
* @param led			led handle
* @return UARTHandle 	a pointer to a UART
*/
UARTHandle UART_constructor(void *pmemory, const size_t numBytes,uint32_t systemClock, uint32_t baud, RGBLEDHandle led);
/**
* @brief get the latest char in the readBuffer
*
*
* @param handle	pointer to a UARTHandle
* @return the latest data
*/
uint8_t UART_getChar(UARTHandle handle);
/**
* @brief put a char in the tx register
*
*
* @param handle	pointer to a UARTHandle
* @param data to send
*/
void UART_putChar(UARTHandle handle, uint8_t c);
/**
* @brief is the transmitter ready to be used
*
*
* @param handle	pointer to a UARTHandle
* @return whether or not tx is ready to send
*/
bool UART_txAvailable(UARTHandle handle);
/**
* @brief is the rx ready to be used
*
*
* @param handle	pointer to a UARTHandle
* @return whether or not rx is ready to recieve
*/
bool UART_rxAvailable(UARTHandle handle);
/**
* @brief is the latest tx done
*
*
* @param handle	pointer to a UARTHandle
* @return whether or not tx is done sending
*/
bool UART_transmitComplete(UARTHandle handle);
/**
* @brief add a char to the tx ringBuffer
*
*
* @param handle	pointer to a UARTHandle
* @param char to add
*/
void UART_queueChar(UARTHandle handle,char c);
/**
* @brief enable tx interrupt
*
*
* @param handle	pointer to a UARTHandle
*/
void UART_enableTXInterrupt(UARTHandle handle);
/**
* @brief disable tx interrupt
*
*
* @param handle	pointer to a UARTHandle
*/
void UART_disableTXInterrupt(UARTHandle handle);
/**
* @brief add a string to the tx ringbuffer
*
*
* @param handle	pointer to a UARTHandle
* @param string to add
*/
void UART_queueString(UARTHandle handle,char* string);
/**
* @brief a printf-like function for this project
*
*
* @param handle	pointer to a UARTHandle
* @param variable amount of arguments (like printf)
*/
void UART_printf(UARTHandle handle, const char*, ...);
/**
* @brief convert a number to a particular base
*
*
* @param num	number to convert
* @param base	base to convert to
*/
char *convert(unsigned int num, int base);
#endif /* UART_UART_H_ */
