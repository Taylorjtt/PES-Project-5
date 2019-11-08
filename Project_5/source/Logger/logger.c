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
 * @file logger.c
 * @brief Cross platform Logger
 *
 *  @author John Taylor
 * 	@date Aug 11, 2019
 *
 */
#include "logger.h"
LoggerHandle Logger_Constructor(void *pmemory, const size_t numbytes)
{
	LoggerHandle handle;
	LOGGERObject *obj;

	if(numbytes < sizeof(LOGGERObject))
	{
		return ((LoggerHandle)NULL);

	}

	handle = (LoggerHandle)pmemory;
	obj = (LOGGERObject *)handle;
	obj->status = DISABLED;
	return handle;
}
void Logger_enable(LoggerHandle handle)
{
	LOGGERObject *obj = (LOGGERObject *)handle;
	obj->status = ENABLED;
}
void Logger_disable(LoggerHandle handle)
{
	LOGGERObject *obj = (LOGGERObject *)handle;
	obj->status = DISABLED;
}
LOGGER_STATUS Logger_status(LoggerHandle handle)
{
	LOGGERObject *obj = (LOGGERObject *)handle;
	return obj->status;
}
void Logger_logData(LoggerHandle handle, uint8_t* loc, size_t length,const char* function, LOG_LEVEL level)
{
	LOGGERObject *obj = (LOGGERObject *)handle;
	if(obj->status == ENABLED)
	{
		for(int i = 0; i < length; i++)
		{
			uintptr_t address = (uintptr_t)(loc + i);
			#ifdef FREEDOM
			PRINTF("In function %s()\n\r",function);
			PRINTF("%s: Address: %02X\tData:%X\n\r",getLevelString(level),address,loc[i]);
			#else
			printf("LOGGER: Address: %02X\tData:%X\n\r",(uint32_t)address,loc[i]);
			#endif
		}
		#ifdef FREEDOM
			PRINTF("\n\r");
		#else
			printf("\n\r");
		#endif
	}
}
void Logger_logString(LoggerHandle handle, const char * string,const char* function, LOG_LEVEL level)
{
	LOGGERObject *obj = (LOGGERObject *)handle;
	if(obj->status == ENABLED)
	{
		PRINTF("------------------------------------------------------------------\n\r");
		PRINTF("In function %s()\n\r",function);
		PRINTF("%s: %s\n\r",getLevelString(level),string);
	}
}
void Logger_logTemps(LoggerHandle handle, float currentTemp,float averageTemp,const char* function, LOG_LEVEL level)
{
	LOGGERObject *obj = (LOGGERObject *)handle;
	if(obj->status == ENABLED)
	{

		PRINTF("------------------------------------------------------------------\n\r");
		PRINTF("In function %s()\n\r",function);
		if(averageTemp > 0 && currentTemp > 0)
		{
			PRINTF("%s: Temperature Reading\n\rCurrent Temp: %f C\n\rAverage Temp %f C\n\r",getLevelString(level),currentTemp,averageTemp);
		}
		if(averageTemp > 0 && currentTemp < 0)
		{
			PRINTF("%s: Temperature Reading\n\rCurrent Temp: -%f C\n\rAverage Temp %f C\n\r",getLevelString(level),currentTemp,averageTemp);
		}
		if(averageTemp < 0 && currentTemp > 0)
		{
			PRINTF("%s: Temperature Reading\n\rCurrent Temp: %f C\n\rAverage Temp -%f C\n\r",getLevelString(level),currentTemp,averageTemp);
		}
		if(averageTemp <= 0 && currentTemp <= 0)
		{
			PRINTF("%s: Temperature Reading\n\rCurrent Temp: -%f C\n\rAverage Temp -%f C\n\r",getLevelString(level),currentTemp,averageTemp);
		}

	}
}


void Logger_logInt(LoggerHandle handle, uint8_t num,const char* function, LOG_LEVEL level)
{
	LOGGERObject *obj = (LOGGERObject *)handle;
	if(obj->status == ENABLED)
	{
		PRINTF("------------------------------------------------------------------\n\r");
		PRINTF("In function %s()\n\r",function);
		PRINTF("%s: %d\n\r",getLevelString(level),num);
	}
}
const char* getLevelString(LOG_LEVEL level)
{
	switch(level)
	{
		case TEST_LEVEL:
		{
			return "TEST\0";
			break;
		}
		case DEBUG_LEVEL:
		{
			return "DEBUG\0";
			break;
		}
		case STATUS_LEVEL:
		{
			return "STATUS\0";
			break;
		}
	}
}
