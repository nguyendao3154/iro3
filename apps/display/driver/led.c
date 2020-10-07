/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        led.c
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


#include "led.h"
#include "gpio.h"

/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/
extern bool g_disableLedKey;

/******************************************************************************
* Constants and macros
******************************************************************************/

GPIO_Type * LED_MACHINE_STATE_PORT[LED_MACHINE_STATE_NUM] = {LED_MACHINE_STATE_FILTER_PORT, LED_MACHINE_STATE_TDS_OUT_PORT};
const uint32_t LED_MACHINE_STATE_PIN[LED_MACHINE_STATE_NUM] = {LED_MACHINE_STATE_FILTER_PIN ,LED_MACHINE_STATE_TDS_OUT_PIN};

/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/


/******************************************************************************
* Local variables
******************************************************************************/
MachineStateLed_t s_currentMachineState;
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


void Led_switchMachineStateLed(MachineStateLed_t machineState)
{
	s_currentMachineState = machineState;
	uint8_t i;
	for ( i = 0;  i < LED_MACHINE_STATE_NUM; ++ i) {
		GPIO_PinWrite(LED_MACHINE_STATE_PORT[i], LED_MACHINE_STATE_PIN[i], 1);
	}
	GPIO_PinWrite(LED_MACHINE_STATE_PORT[machineState], LED_MACHINE_STATE_PIN[machineState], 0);
}

void Led_turnOffMachineStateled()
{
	uint8_t i;
	for ( i = 0;  i < LED_MACHINE_STATE_NUM; ++ i) {
		GPIO_PinWrite(LED_MACHINE_STATE_PORT[i], LED_MACHINE_STATE_PIN[i], 1);
	}
}

void Led_turnOnMachineStateled()
{
	Led_switchMachineStateLed(s_currentMachineState);
}

void Led_turnOnAll()
{

	for (uint8_t i = 0;  i < LED_MACHINE_STATE_NUM; ++ i) {
		GPIO_PinWrite(LED_MACHINE_STATE_PORT[i], LED_MACHINE_STATE_PIN[i], 0);
	}
}

void Led_turnAllLedMachineState(LedState_t state)
{
	uint8_t gpioLevel;
	if(state == LED_STATE_OFF)
		gpioLevel = 1U;
	else
		gpioLevel = 0U;
	for (uint8_t i = 0;  i < LED_MACHINE_STATE_NUM; ++ i) {
		GPIO_PinWrite(LED_MACHINE_STATE_PORT[i],LED_MACHINE_STATE_PIN[i], gpioLevel);
	}
}

