
/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         Temp.h
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

#ifndef TIMER_H_
#define TIMER_H_


/******************************************************************************
* Includes
******************************************************************************/


#include <config.h>
#include "adc.h"
/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Macros 
******************************************************************************/



/******************************************************************************
* Types
******************************************************************************/


/**
 * @brief Use brief, otherwise the index won't have a brief explanation.
 *
 * Detailed explanation.
 */




/******************************************************************************
* Global variables
******************************************************************************/
   

/******************************************************************************
* Global functions
******************************************************************************/
PUBLIC void delay_clk (uint16_t delay);

PUBLIC void TIMER_SystemTickEvent(void);

PUBLIC void TIMER_CheckTimerEvent();

PUBLIC void TIMER_Init();
/******************************************************************************
* Inline functions
******************************************************************************/



#endif 


