/*
 * UIControl.h
 *
 *  Created on: Jul 28, 2018
 *      Author: Admin
 */
/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         UIControl.h
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

#ifndef APPS_UICONTROL_UICONTROL_H_
#define APPS_UICONTROL_UICONTROL_H_

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

#ifdef HW_VER_214
typedef enum BUTTON_ID
{
	BUTTON_ID_SELECT = 0,
	BUTTON_ID_PLUS = 1,
	BUTTON_ID_MINUS = 3,
	BUTTON_ID_SET = 2,
	BUTTON_NUM = 4,
}ButtonId_t;
#else
typedef enum BUTTON_ID
{
	BUTTON_ID_SELECT = 3,
	BUTTON_ID_PLUS = 1,
	BUTTON_ID_MINUS = 0,
	BUTTON_ID_SET = 2,
	BUTTON_NUM = 4,
}ButtonId_t;
#endif
/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/
bool UIControl_stateIsLock();
void UIControl_process();

// callback
void TouchBtnPressed_cb(ButtonId_t btn);
void TouchBtnHoldRelease_cb(ButtonId_t btn);

/******************************************************************************
* Inline functions
******************************************************************************/




#endif /* APPS_UICONTROL_UICONTROL_H_ */
