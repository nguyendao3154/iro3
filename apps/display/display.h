/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         display.h
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

#ifndef APPS_DISPLAY_DISPLAY_H_
#define APPS_DISPLAY_DISPLAY_H_


/******************************************************************************
* Includes
******************************************************************************/

#include <config.h>
#include <stdint.h>
#include "led7seg.h"
#include "led.h"
#include "buzzer.h"
#include "errorCheck.h"

/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Macros
******************************************************************************/



/******************************************************************************
* Types
******************************************************************************/
typedef enum
{
	ERROR_CODE_INCOME_WATER_LOST = 4,
	ERROR_CODE_INCOME_WATER_NO_STABILITY = 2,
	ERROR_CODE_PUMP_RUN_OVER_TIME = 1,
	ERROR_CODE_LEAK_WATER = 5,
}ErrorCode_t;


/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/

/**
 * @brief process display module
 *
 * call this funtion in loop.
 * Required cycle is 10ms
 *
 * @return none
 */
void Display_process();
/**
 * @brief show start screen
 *
 * call this funtion before loop
 *
 * @return none
 */
void Display_showStart();
/**
 * @brief show Tds out screen
 *
 *
 * @return none
 */
void Display_showTdsOut();
/**
 * @brief show Tds in screen
 *
 *
 * @return none
 */
void Display_showTdsIn();
/**
 * @brief show filter life time
 *
 *
 * @param filter the index of filter
 *
 * @return none
 */
void Display_showFilterLifeTime(uint8_t filter);
/**
 * @brief show remain life time of filter
 *
 *
 * @param filter the index of filter
 *
 * @return none
 */
void Display_showFilterTime(uint8_t filter);
/**
 * @brief show current error
 *
 * call in the loop when system have error.
 *
 * @return none
 */
void Display_showCurrentError();
/**
 * @brief show tds_in limit threshold
 *
 * call when setup the threshold
 *
 * @return none
 */
void Display_showTdsInLimit();
/**
 * @brief show tds_out limit threshold
 *
 * call when setup the threshold
 *
 * @return none
 */
void Display_showTdsOutLimit();
/**
 * @brief show new error
 *
 * call when find a new error
 * after calling screen will display new error
 * @param newError
 *
 * @return none
 */
void Display_showNewError(ErrorType_t newError);
/**
 * @brief turn off blink led
 *
 * call when UI is change to unlocked state from Show Error state.
 *
 * @return none
 */
void Display_turnOffBlinkled();
/**
 * @brief cancel showing error
 *
 * call when system just found a error for short time but now error is gone.
 *
 * @return none
 */
void Display_cancelError();


#endif /* APPS_DISPLAY_DISPLAY_H_ */



