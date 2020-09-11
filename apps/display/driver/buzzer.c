/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        buzzer.c
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


#include "buzzer.h"
#include "timeCheck.h"
#include "gpio.h"
/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define BUZZER_ON_TIME 300
#define BUZZER_INTERVAL 700
#define BUZZER_TIME_IN_ERROR 10



/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/


/******************************************************************************
* Local variables
******************************************************************************/
uint32_t timeOffBuzzer;
bool buzzerIsOn = false;
uint8_t timeBuzzerLeft = 0 ;
uint32_t timeStartBuzzer;
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
void Buzzer_process()
{
	if(buzzerIsOn && timeIsAfter(g_sysTime , timeOffBuzzer) )
	{
		R_GPIO_PinWrite(BUZZER_PIN, GPIO_LEVEL_LOW);
		buzzerIsOn = false;
	}
	if(timeBuzzerLeft >0)
	{
		if(timeIsAfter(g_sysTime, timeStartBuzzer + (BUZZER_TIME_IN_ERROR - timeBuzzerLeft) * BUZZER_INTERVAL))
		{
			Buzzer_onInMs(BUZZER_ON_TIME);
			timeBuzzerLeft -- ;
		}
	}
}

/******************************************************************************
* Global functions
******************************************************************************/

void Buzzer_onInMs(uint16_t msTime)
{
	timeOffBuzzer = g_sysTime + msTime;
	R_GPIO_PinWrite(BUZZER_PIN, GPIO_LEVEL_HIGH);
	buzzerIsOn = true;
}
void Buzzer_turnOn(bool isOn)
{
	if(isOn)
	{
		R_GPIO_PinWrite(BUZZER_PIN, GPIO_LEVEL_HIGH);
	}
	else{
		R_GPIO_PinWrite(BUZZER_PIN, GPIO_LEVEL_LOW);
	}
}
void Buzzer_blink(uint8_t time)
{
	timeStartBuzzer = g_sysTime;
	timeBuzzerLeft = time;
}

void Buzzer_blinkError()
{
	Buzzer_blink(BUZZER_TIME_IN_ERROR);
}
void Buzzer_cancelBlink()
{
	timeBuzzerLeft = 0;
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


