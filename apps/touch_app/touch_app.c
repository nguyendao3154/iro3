/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        touch_app.c
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


#include <config.h>
#include "touch_app.h"

#define NDEBUG
#include <assert.h>

#include "TSI_config.h"
#include "TSI_key.h"
#include "UIControl.h"
#include "led.h"
#include "uart.h"
/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/
uint8_t button_event;
uint8_t button_id = 0xFFU;
/******************************************************************************
* Constants and macros
******************************************************************************/
#define MAX_CONFIGS	(METHOD_NUM)


/***********************************************************************************************************************
* Local types
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/


/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/

/******************************************************************************
* Local functions
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/

/**
 * @brief init touch module
 *
 * call before main loop
 *
 * @return none
 */
void  TOUCH_init()
{
	/* Init board hardware. */
    
	TSI_KeyInit();
}

/**
 * @brief process touch
 *
 * call it cyclically.
 *
 * @param arg1 the first function argument
 * @param arg2 the second function argument
 *
 * @return descrition for the function return value
 */

void TOUCH_process()
{
	button_event = TSI_KeyDetect(&button_id);
//	UART_UartPuts("0");
	if(kKey_Event_Touch == button_event)
   {
		UART_UartPuts("0");
	   if(button_id == BUTTON_ID_SET)
	   {

		   TouchBtnPressed_cb(button_id);
	   }else if(!UIControl_stateIsLock())
	   {
		   TouchBtnPressed_cb(button_id);
	   }
	   if(UIControl_stateIsLock()){
		   return;
	   }
	   switch(button_id)
	   {
	   case BUTTON_ID_SET:
	   {
		   UART_UartPuts("1");
		   break;
	   }
	   case BUTTON_ID_PLUS:
	   {
		   UART_UartPuts("2");
		   break;
	   }
	   case BUTTON_ID_MINUS:
	   {
		   UART_UartPuts("3");
		   break;
	   }
	   case BUTTON_ID_SELECT:
	   {
		   UART_UartPuts("4");
		   break;
	   }
	   default:
		   break;
	   }
   }

	else if(kKey_Event_Release == button_event)
   {
	   TouchBtnHoldRelease_cb(button_id);
	   if(button_id == BUTTON_ID_SET)
	   {

	   }
	   if(UIControl_stateIsLock())
	   {

		   return;
	   }
	   switch(button_id)
	   {
	   case BUTTON_ID_SET:
	   {

		   break;
	   }
	   case BUTTON_ID_PLUS:
	   {

		   break;
	   }
	   case BUTTON_ID_MINUS:
	   {

		   break;
	   }
	   case BUTTON_ID_SELECT:
	   {

		   break;
	   }
	   default:
		   break;
	   }
   }
   else{
	   button_event = kKey_Event_Idle;
   }
}


