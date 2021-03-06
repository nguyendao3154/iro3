#ifndef APPS_FLASH_FILTER_TIME_H_
#define APPS_FLASH_FILTER_TIME_H_

/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         filter_time.h
 *
 * @author    	quanvu
 *
 * @version   1.0
 *
 * @date
 *
 * @brief     Brief description of the file
 *
 * Detailed Description of the file. If not used, remove the separator above.
 *
 */


/******************************************************************************
* Includes
******************************************************************************/

#include <config.h>

/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Macros
******************************************************************************/




/******************************************************************************
* Types
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/
void filter_time_init();
void filter_time_minusTime(uint32_t second);
uint16_t filter_time_getFilterHour(uint8_t filIndex);
uint32_t filter_time_getFilterSecond(uint8_t filIndex);
void filter_time_resetTimeAtIndex(uint8_t filIndex);
void filter_time_resetToDefault();

/******************************************************************************
* Inline functions
******************************************************************************/





#endif /* APPS_FLASH_FILTER_TIME_H_ */
