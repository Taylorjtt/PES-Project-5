/*
 * RingBuffer.c
 *
 *  Created on: Nov 8, 2019
 *      Author: john
 */
#include <stdint.h>
#include <stdbool.h>
typedef enum {BUFFER_FULL_ERROR,BUFFER_EMPTY_ERROR}BUFFER_ERROR;

typedef struct RING_BUFF_OBJ
{
	uint8_t * buffer;
	uint8_t * head;
	uint8_t * tail;
	size_t length;
	size_t count;
}RingBufferObject;

typedef struct RING_BUFF_OBJ *RingBufferHandle;

RingBufferHandle RingBuffer_Constructor(void *pmemory, const size_t numbytes, const size_t bufferSize);

BUFFER_ERROR RingBuffer_push(RingBufferHandle handle, uint8_t data);
BUFFER_ERROR RingBuffer_pop(RingBufferHandle handle);
uint8_t RingBuffer_peek(RingBufferHandle handle);

bool RingBuffer_isFull(RingBufferHandle handle);
bool RingBuffer_isEmpty(RingBufferHandle handle);
bool RingBuffer_isInitialized(RingBufferHandle handle);
bool RingBuffer_isValid(RingBufferHandle handle);
void RingBuffer_destroy(RingBufferHandle handle);
