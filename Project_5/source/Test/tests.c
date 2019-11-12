/*****************************************************************************
* Copyright (C) 2019 by John Taylor
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. John Taylor and the University of Colorado are not liable for
* any misuse of this material.
*
*****************************************************************************/
/*
 * @file tests.c
 * @brief
 *
 *  @author John Taylor
 * 	@date Nov 11, 2019
 *
 */

#include "tests.h"

RingBufferHandle ring;

void testRingBuffer(void)
{

	UCUNIT_TestcaseBegin("Testing RingBuffer");
	ring = malloc(sizeof(RingBufferObject));
	RingBuffer_Constructor((void*)ring, sizeof(RingBufferObject), 3);
	UCUNIT_CheckIsEqual(true,RingBuffer_isEmpty(ring));

	UCUNIT_CheckIsEqual(BUFFER_EMPTY_ERROR,RingBuffer_pop(ring));
	UCUNIT_CheckIsEqual(BUFFER_EMPTY_ERROR,RingBuffer_peek(ring));

	for(int i = 0; i < 3; i++)
	{
		UCUNIT_CheckIsEqual(RingBuffer_push(ring, i),NO_ERROR);
	}

	UCUNIT_CheckIsEqual(RingBuffer_push(ring, 3),BUFFER_FULL_ERROR);
	UCUNIT_CheckIsEqual(RingBuffer_push(ring, 4),NO_ERROR);

	for(int i = 0; i < 5; i++)
	{
		UCUNIT_CheckIsEqual(RingBuffer_peek(ring),i);
		UCUNIT_CheckIsEqual(RingBuffer_pop(ring),NO_ERROR);
	}
	UCUNIT_CheckIsEqual(RingBuffer_pop(ring),BUFFER_EMPTY_ERROR);


	UCUNIT_TestcaseEnd();
}
