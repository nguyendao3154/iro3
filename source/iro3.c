/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        main.c
*
* @author    trongkn
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

#include "config.h"
#include "gpio.h"
#include "adc.h"
#include "display.h"
//#include "touch_app.h"
//#include "flash_app.h"
//#include "filter_time.h"
#include "UIControl.h"
#include "pumpControl.h"
#include "timer.h"
#include "errorCheck.h"
#include "uart.h"
//#include "filterExpire.h"
#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKE16Z4.h"
#include "config_timer_user.h"


/******************************************************************************
* External objects
******************************************************************************/
volatile uint8_t g_run200usFlag;
volatile uint8_t g_run1msFlag;
uint8_t g_adc_flag;
bool g_disableLedKey;
uint8_t abc;
/******************************************************************************
* Global variables
******************************************************************************/

uint32_t g_sysTimeS = 0;
uint16_t g_adc_result;

/******************************************************************************
* Constants and macros
******************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define TIME_TO_DISPLAY_TDS  (2000) //2 secs
/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/


/******************************************************************************
* Local variables
******************************************************************************/

uint16_t value ;
uint32_t time_out = 0;
uint32_t i = 0;
uint8_t s_timeOut100ms;
uint8_t s_timeOut10ms;
LOCAL uint8_t s_pwm_cnt = 0;
//LOCAL BOOLEAN s_is_timeout = TRUE;



/******************************************************************************
* Local functions
******************************************************************************/
void run200usTask();
void run1msTask();
void run10msTask();
void run100msTask();
void run_DisplayTds();

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
void main(void);
void main(void)
{

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    GPIO_Init();
	//TOUCH_init();
	//UART_Init();
	//flash_app_init();
	//ADC_Init();
	//TIMER_Init();
	Display_showStart();
	g_disableLedKey = true;
	Led7_ReduceIntensity(true);
	//pumpControl_onVanXaInMs(5000);
	/* Main loop */
	while(1)
	{
		//////////////////////
    	// if(g_run200usFlag == 1)
    	// {
    	// 	run200usTask();
    	// 	g_run200usFlag= 0;
    	// }
    	// if(g_run1msFlag == 1)
    	// {
    	// 	run1msTask();
    	// 	g_run1msFlag= 0;
    	// }
    	// if(s_timeOut10ms >= 10)
    	// {
    	// 	run10msTask();
    	// 	s_timeOut10ms = 0;
    	// }
    	// if(s_timeOut100ms >= 100)
    	// {
    	// 	run100msTask();
    	// 	s_timeOut100ms = 0;
    	// }
    	// if(g_adc_flag)
    	// {
    	// 	ADC_UpdateTds (s_pwm_cnt);

    	// }
    	// UART_Process();
    	// TIMER_CheckTimerEvent();
	}
}

void run200usTask()
{

}
void run1msTask()
{
	if((g_sysTime %1000) == 0)
		g_sysTimeS++;
	TOUCH_process();
	if(s_timeOut100ms <100)
	{
		s_timeOut100ms++;
	}
	if(s_timeOut10ms <10)
	{
		s_timeOut10ms++;
	}
}

void run10msTask()
{
	Display_process();
}

void run100msTask()
{
	UIControl_process();
	pumpControl_process();
	ErrorCheck_process();
	filterExpire_process();
}

void abort(void)
{
	while(1)
	{
		;
	}
}
