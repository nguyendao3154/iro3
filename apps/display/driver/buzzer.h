
/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         buzzer.h
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

#ifndef APPS_DISPLAY_BUZZER_H_
#define APPS_DISPLAY_BUZZER_H_


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
/**
 * @brief process buzzer module
 *
 * call this funtion in loop.
 * Required cycle is 10ms
 *
 * @return none
 */
void Buzzer_process();
/**
 * @brief on buzzer in time ms
 *
 * @param msTime
 *
 * @return none
 */
void Buzzer_onInMs(uint16_t msTime);
/**
 * @brief blink buzzer
 *
 *	on buzzer many time.
 *	cycle is defined as BUZZER_INTERVAL
 *	time on in cycle is defined as BUZZER_ON_TIME
 *
 * @param time
 *
 * @return none
 */
void Buzzer_blink(uint8_t time);
/**
 * @brief blink buzzer when a error found
 *
 * call when a error is found.
 *	time buzzer ring is defined as BUZZER_TIME_IN_ERROR
 *
 * @return none
 */
void Buzzer_blinkError();
/**
 * @brief on or off buzzer
 *
 * @return none
 */
void Buzzer_turnOn(bool isOn);
/**
 * @brief cancel blink buzzer
 *
 * call when you want buzzer to turn off after call Buzzer_blink
 *
 * @return none
 */
void Buzzer_cancelBlink();


/******************************************************************************
* Inline functions
******************************************************************************/






#endif /* APPS_DISPLAY_BUZZER_H_ */
