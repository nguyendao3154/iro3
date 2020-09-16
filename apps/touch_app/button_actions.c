/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : button_actions.c
* Version      : 1.0 <- Optional as long as history is shown below
* Description  : This module solves all the world's problems
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 15.01.2007 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_touch_button_if.h"
#include "r_gpio_rx_if.h"
#include "led.h"
#include "UIControl.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/


/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/
 
/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: App_TOUCH_Button_Notification
* Description  : This function is called by the button layer when a button event occurs.
* Arguments    : p_args -
*                    Pointer to arguments which are passed by the R_TOUCH_Button layer. Type-cast p_args to
*                    (touch_button_callback_arg_t const * const).
* Return Value : None.
***********************************************************************************************************************/
void App_TOUCH_Button_Notification(void * p_args)
{
   touch_button_callback_arg_t * p_arg = p_args;
   uint32_t button_id = p_arg->id;
   touch_button_event_t button_event = (touch_button_event_t)p_arg->event;
   
   (void)button_id;
   (void)button_event;

   if(TOUCH_BUTTON_EVENT_REQUEST_DELAY==button_event)
   {
	   R_BSP_SoftwareDelay(1, BSP_DELAY_MILLISECS);
   }

   if(TOUCH_BUTTON_EVENT_PRESSED ==button_event)
   {
	   if(button_id == BUTTON_ID_SET)
	   {

		   TouchBtnPressed_cb(button_id);
	   }else if(!UIControl_stateIsLock())
	   {
		   TouchBtnPressed_cb(button_id);
	   }
	   if(UIControl_stateIsLock())
		   return;
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

   if(TOUCH_BUTTON_EVENT_RELEASED==button_event)
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

}
/***********************************************************************************************************************
End of function App_TOUCH_Button_Notification
***********************************************************************************************************************/
