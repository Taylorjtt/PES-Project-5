/*
 * RingBuffer.c
 *
 *  Created on: Nov 8, 2019
 *      Author: john
 */
#include "RingBuffer.h"

RingBufferHandle RingBuffer_Constructor(void *pmemory, const size_t numbytes, const size_t bufferSize)
{
	RingBufferHandle handle;
	RingBufferObject *obj;

	if(numbytes < sizeof(RingBufferObject))
	{
		return ((RingBufferHandle)NULL);
	}

	handle = (RingBufferHandle)pmemory;
	obj = (RingBufferObject *)handle;

	obj->buffer = (uint8_t*)malloc(bufferSize);
	obj->count = 0;
	obj->head = obj->buffer;
	obj->tail = obj->buffer;
	obj->length = bufferSize;
	return handle;

}

BUFFER_ERROR RingBuffer_push(RingBufferHandle handle, uint8_t data)
{
	RingBufferObject *obj = (RingBufferObject *)handle;
	if(!RingBuffer_isFull(handle))
	{
		START_CRITICAL;
		*(obj->head) = data;
		obj->head++;
		obj->count++;
		END_CRITICAL;
		return NO_ERROR;
	}
	else
	{

		START_CRITICAL;
		//calculate offsets from the buffer
		size_t headOffset = obj->head - obj->buffer;
		size_t tailOffset = obj->tail - obj->buffer;
		//reallocate buffer
		obj->buffer = realloc(obj->buffer,sizeof(obj->buffer)*2);
		//set the head and tail based on offsets
		obj->head = obj->buffer + headOffset;
		obj->tail = obj->buffer + tailOffset;
		obj->length = obj->length * 2;
		*(obj->head) = data;
		obj->head++;
		obj->count++;
		END_CRITICAL;
		return BUFFER_FULL_ERROR;
	}

}
BUFFER_ERROR RingBuffer_pop(RingBufferHandle handle)
{
	RingBufferObject *obj = (RingBufferObject *)handle;
	if(!RingBuffer_isEmpty(handle))
	{
		START_CRITICAL;
		obj->tail++;
		obj->count--;
		END_CRITICAL;
		return NO_ERROR;

	}
	else
	{
		return BUFFER_EMPTY_ERROR;
	}
}
uint8_t RingBuffer_peek(RingBufferHandle handle)
{
	RingBufferObject *obj = (RingBufferObject *)handle;
	if(!RingBuffer_isEmpty(handle))
	{
		return *(obj->tail);
		return NO_ERROR;
	}
	else
	{
		return BUFFER_EMPTY_ERROR;
	}
}

bool RingBuffer_isFull(RingBufferHandle handle)
{
	RingBufferObject *obj = (RingBufferObject *)handle;
	if(obj->count >= obj->length)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool RingBuffer_isEmpty(RingBufferHandle handle)
{
	RingBufferObject *obj = (RingBufferObject *)handle;
	if(obj->count != 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool RingBuffer_isInitialized(RingBufferHandle handle)
{
	RingBufferObject *obj = (RingBufferObject *)handle;
}
bool RingBuffer_isValid(RingBufferHandle handle)
{
	RingBufferObject *obj = (RingBufferObject *)handle;
}
void RingBuffer_destroy(RingBufferHandle handle)
{
	RingBufferObject *obj = (RingBufferObject *)handle;
}



