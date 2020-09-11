/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         led7seg.h
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

#ifndef APPS_DISPLAY_LED7SEG_H_
#define APPS_DISPLAY_LED7SEG_H_


/******************************************************************************
* Includes
******************************************************************************/

#include <config.h>
#include <stdint.h>
/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Macros
******************************************************************************/
#define LED_7SEG_OFF 10
#define LED_7SEG_ERROR 11



/******************************************************************************
* Types
******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/
void Led7seg_SetNumberInLed4(uint16_t number); // set number for 4 digit led
uint16_t Led7seg_getNumberInLed4();
void Led7seg_reduceNumberInLed4(uint16_t reduce);
void Led7seg_increaseNumberInLed4(uint16_t reduce);
void Led7seg_turnOnLed();
void Led7seg_turnOffLed();

void Led7seg_SetNumberInLed1(int8_t number); // set number for 1 digit led
/**
 * @brief scan 7seg led
 *
 * call it with cycle less than 1ms.
 *
 * @return none
 */
void Led7seg_scanLed(void);

void Led7seg_setAllLedCode(uint8_t ledCode);
void Led7seg_OnAllSegInLed(uint8_t ledIndex);
void Led7_ReduceIntensity(bool isOn);


/******************************************************************************
* Inline functions
******************************************************************************/






#endif /* APPS_DISPLAY_LED7SEG_H_ */
