/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        filter_time.c
*
* @author    quanvu
*
* @version   1.0
*
* @date
*
* @brief
*
*******************************************************************************
*
* Detailed Description of the file. If not used, remove the separator above.
*
******************************************************************************/


#include "filter_time.h"
#include "flash_app.h"
#include "user_config.h"
#include <string.h>

/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/

/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/
static uint32_t s_filterRunTimRemain[FILTER_NUM];



/******************************************************************************
* Local functions
******************************************************************************/
/**
 * @brief One line documentation
 *
 * A more detailed documentation
 *
 * @param arg1 the first function argument
 * @param arg2 the second function argument
 *
 * @return descrition for the function return value
 */


void filter_time_updateToFlash()
{
	flash_app_writeBlock((uint8_t *)s_filterRunTimRemain, FILLTER_TIME_BLOCK, sizeof(s_filterRunTimRemain));
}

/******************************************************************************
* Global functions
******************************************************************************/

/**
 * @brief One line documentation
 *
 * A more detailed documentation
 *
 * @param arg1 the first function argument
 * @param arg2 the second function argument
 *
 * @return descrition for the function return value
 */
void filter_time_init()
{
    bool readOk = flash_app_readData((uint8_t*)s_filterRunTimRemain,FILLTER_TIME_BLOCK,sizeof(s_filterRunTimRemain));

    if(!readOk)
    {
    	memcpy(s_filterRunTimRemain,g_userConfig.filterRunTimeTotal,sizeof(g_userConfig.filterRunTimeTotal));
    	filter_time_updateToFlash();
    }
}
void filter_time_resetToDefault()
{
	memcpy(s_filterRunTimRemain,g_userConfig.filterRunTimeTotal,sizeof(g_userConfig.filterRunTimeTotal));
	filter_time_updateToFlash();
}

void filter_time_minusTime(uint32_t second)
{
	for(uint8_t i = 0; i< FILTER_NUM; i++)
	{
		if(s_filterRunTimRemain[i] > second)
		{
			s_filterRunTimRemain[i] -= second;
		}
		else // need new filter
		{
			s_filterRunTimRemain[i] = 0;
		}
	}
	filter_time_updateToFlash();
}

uint16_t filter_time_getFilterHour(uint8_t filIndex)
{
	if(s_filterRunTimRemain[filIndex]%3600 >0)
	{
		return (uint16_t)(s_filterRunTimRemain[filIndex]/3600 +1);
	}
	else
	{
		return (uint16_t)(s_filterRunTimRemain[filIndex]/3600);
	}
}

uint32_t filter_time_getFilterSecond(uint8_t filIndex)
{
	return (s_filterRunTimRemain[filIndex]);
}

void filter_time_resetTimeAtIndex(uint8_t filIndex)
{
	s_filterRunTimRemain[filIndex] = g_userConfig.filterRunTimeTotal[filIndex];
	filter_time_updateToFlash();
}
/**
 * @brief One line documentation
 *
 * A more detailed documentation
 *
 * @param arg1 the first function argument
 * @param arg2 the second function argument
 *
 * @return descrition for the function return value
 */
