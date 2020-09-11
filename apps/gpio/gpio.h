
/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         gpio.h
 *
 * @author    	trongkn
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

#ifndef GPIO_H_
#define GPIO_H_


/******************************************************************************
* Includes
******************************************************************************/

#include <config.h>
#include "r_gpio_rx_if.h"

/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Macros 
******************************************************************************/
// Pin define
#define BUZZER_PIN GPIO_PORT_1_PIN_7

#define PIN_KEY_NUM 4

#define LED_MACHINE_STATE_NUM 3
#define LED_MACHINE_STATE_TDS_OUT GPIO_PORT_4_PIN_6
#define LED_MACHINE_STATE_TDS_IN  GPIO_PORT_4_PIN_5
#define LED_MACHINE_STATE_FILTER GPIO_PORT_4_PIN_7

#define PIN_AP_THAP 	GPIO_PORT_0_PIN_3
#define PIN_AP_CAO 		GPIO_PORT_2_PIN_7

#define PIN_DK_BOM 		GPIO_PORT_3_PIN_1
#define PIN_VAN_XA 		GPIO_PORT_3_PIN_6

// action define
#define CHECK_CO_AP_THAP 	(R_GPIO_PinRead(PIN_AP_THAP)== GPIO_LEVEL_HIGH)
#define CHECK_CO_AP_CAO 	(R_GPIO_PinRead(PIN_AP_CAO)== GPIO_LEVEL_HIGH)

#define TURN_ON_PUMP 		R_GPIO_PinWrite(PIN_DK_BOM,GPIO_LEVEL_HIGH)
#define TURN_OFF_PUMP 		R_GPIO_PinWrite(PIN_DK_BOM,GPIO_LEVEL_LOW)
#define TURN_ON_VAN_XA 		R_GPIO_PinWrite(PIN_VAN_XA,GPIO_LEVEL_HIGH)
#define TURN_OFF_VAN_XA 	R_GPIO_PinWrite(PIN_VAN_XA,GPIO_LEVEL_LOW)


/******************************************************************************
* Types
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/
   

/******************************************************************************
* Global functions
******************************************************************************/
PUBLIC void GPIO_Init(void);

/******************************************************************************
* Inline functions
******************************************************************************/



#endif 


