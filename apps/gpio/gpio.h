
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
#include "fsl_gpio.h"

/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Macros 
******************************************************************************/
// Pin define
#define BUZZER_PORT GPIOD
#define BUZZER_PIN 	4U

#define PIN_KEY_NUM 4

#define LED_MACHINE_STATE_NUM 2
#define LED_MACHINE_STATE_TDS_OUT_PORT GPIOD
#define LED_MACHINE_STATE_TDS_OUT_PIN 6U

#define LED_MACHINE_STATE_FILTER_PORT GPIOB
#define LED_MACHINE_STATE_FILTER_PIN  2U

#define PORT_AP_THAP	GPIOA
#define PIN_AP_THAP 	6U
#define PORT_AP_CAO		GPIOE
#define PIN_AP_CAO 		0U

#define PORT_PULSE      GPIOE
#define PIN_PULSE       6U


#define PORT_DK_BOM     GPIOA
#define PIN_DK_BOM 		0U
#define PORT_VAN_XA     GPIOE
#define PIN_VAN_XA 		1U

// action define
#define CHECK_CO_AP_THAP 	(GPIO_PinRead(PORT_AP_THAP, PIN_AP_THAP)== 1U)
#define CHECK_CO_AP_CAO 	(GPIO_PinRead(PORT_AP_CAO, PIN_AP_CAO)== 1U)

#define RISE_PULSE          GPIO_PinWrite(PORT_PULSE, PIN_PULSE, 1)
#define FALL_PULSE          GPIO_PinWrite(PORT_PULSE, PIN_PULSE, 0)

#define TURN_ON_PUMP 		GPIO_PinWrite(PORT_DK_BOM, PIN_DK_BOM, 1)
#define TURN_OFF_PUMP 		GPIO_PinWrite(PORT_DK_BOM, PIN_DK_BOM, 0)
#define TURN_ON_VAN_XA 		GPIO_PinWrite(PORT_VAN_XA, PIN_VAN_XA, 1)
#define TURN_OFF_VAN_XA 	GPIO_PinWrite(PORT_VAN_XA, PIN_VAN_XA, 0)


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


