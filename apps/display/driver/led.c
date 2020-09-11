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
bool g_disableLedKey = false;

/******************************************************************************
* Constants and macros
******************************************************************************/



const gpio_port_pin_t LED_KEY_RED_PIN[] = {GPIO_PORT_B_PIN_7,GPIO_PORT_C_PIN_3,GPIO_PORT_C_PIN_6,GPIO_PORT_5_PIN_4};
const gpio_port_pin_t LED_KEY_GREEN_PIN[] = {GPIO_PORT_C_PIN_2,GPIO_PORT_C_PIN_5,GPIO_PORT_C_PIN_7,GPIO_PORT_5_PIN_5};
const gpio_port_pin_t LED_MACHINE_STATE_PIN[LED_MACHINE_STATE_NUM] = {LED_MACHINE_STATE_FILTER ,LED_MACHINE_STATE_TDS_IN,LED_MACHINE_STATE_TDS_OUT};

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

void Led_SetLedKeyState(LedKeyName_t ledName,LedKeyColor_t color, LedState_t state)
{
	if(g_disableLedKey) return;
	if(color == LED_KEY_COLLOR_GREEN)
	{
		R_GPIO_PinWrite(LED_KEY_GREEN_PIN[ledName],state == LED_STATE_ON? GPIO_LEVEL_LOW:GPIO_LEVEL_HIGH);
	}else
	{
		R_GPIO_PinWrite(LED_KEY_RED_PIN[ledName],state == LED_STATE_ON? GPIO_LEVEL_LOW:GPIO_LEVEL_HIGH);
	}
}

void Led_turnOnLedKey()
{
	if(g_disableLedKey) return;
	for(uint8_t i = 0; i< PIN_KEY_NUM; i++)
	{
		R_GPIO_PinWrite(LED_KEY_GREEN_PIN[i],GPIO_LEVEL_LOW);
	}
}
void Led_turnOffLedKey()
{
	if(g_disableLedKey) return;
	for(uint8_t i = 0; i< PIN_KEY_NUM; i++)
	{
		R_GPIO_PinWrite(LED_KEY_GREEN_PIN[i],GPIO_LEVEL_HIGH);
	}
}

void Led_turnOffAllLedKey()
{
	if(g_disableLedKey) return;
	for(uint8_t i = 0; i< PIN_KEY_NUM; i++)
	{
		R_GPIO_PinWrite(LED_KEY_GREEN_PIN[i],GPIO_LEVEL_HIGH);
	}
	for(uint8_t i = 0; i< PIN_KEY_NUM; i++)
	{
		R_GPIO_PinWrite(LED_KEY_RED_PIN[i],GPIO_LEVEL_HIGH);
	}
}

void Led_switchMachineStateLed(MachineStateLed_t machineState)
{
	s_currentMachineState = machineState;
	uint8_t i;
	for ( i = 0;  i < LED_MACHINE_STATE_NUM; ++ i) {
		R_GPIO_PinWrite(LED_MACHINE_STATE_PIN[i], GPIO_LEVEL_HIGH);
	}
	R_GPIO_PinWrite(LED_MACHINE_STATE_PIN[machineState], GPIO_LEVEL_LOW);
}

void Led_turnOffMachineStateled()
{
	uint8_t i;
	for ( i = 0;  i < LED_MACHINE_STATE_NUM; ++ i) {
		R_GPIO_PinWrite(LED_MACHINE_STATE_PIN[i], GPIO_LEVEL_HIGH);
	}
}

void Led_turnOnMachineStateled()
{
	Led_switchMachineStateLed(s_currentMachineState);
}

void Led_turnOnAll()
{
	Led_turnOnLedKey();
	for (uint8_t i = 0;  i < LED_MACHINE_STATE_NUM; ++ i) {
		R_GPIO_PinWrite(LED_MACHINE_STATE_PIN[i], GPIO_LEVEL_LOW);
	}
}

void Led_turnAllLedMachineState(LedState_t state)
{
	gpio_level_t gpioLevel;
	if(state == LED_STATE_OFF)
		gpioLevel = GPIO_LEVEL_HIGH;
	else
		gpioLevel = GPIO_LEVEL_LOW;
	for (uint8_t i = 0;  i < LED_MACHINE_STATE_NUM; ++ i) {
		R_GPIO_PinWrite(LED_MACHINE_STATE_PIN[i], gpioLevel);
	}
}
void Led_turnAllLedKey(LedKeyColor_t color)
{
	if(g_disableLedKey) return;
	if(color == LED_KEY_COLLOR_RED)
		for (uint8_t i = 0;  i < 4; ++ i) {
			R_GPIO_PinWrite(LED_KEY_RED_PIN[i], GPIO_LEVEL_LOW);
			R_GPIO_PinWrite(LED_KEY_GREEN_PIN[i], GPIO_LEVEL_HIGH);
		}
	else if(color == LED_KEY_COLLOR_GREEN)
		for (uint8_t i = 0;  i < 4; ++ i) {
			R_GPIO_PinWrite(LED_KEY_GREEN_PIN[i], GPIO_LEVEL_LOW);
			R_GPIO_PinWrite(LED_KEY_RED_PIN[i], GPIO_LEVEL_HIGH);
		}
}
