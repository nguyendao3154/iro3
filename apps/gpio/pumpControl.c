/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        pumpControl.c
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

#include "pumpControl.h"
#include "gpio.h"
#include "timeCheck.h"
#include "filter_time.h"
#include "adc.h"
#include "user_config.h"
/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/
extern bool g_ioTestEn;
/******************************************************************************
* Constants and macros
******************************************************************************/


/******************************************************************************
* Local types
******************************************************************************/
typedef enum ouput{
	OUT_PUMP = 0,
	OUT_VAN
}Output_t ;
/******************************************************************************
* Local function prototypes
******************************************************************************/
void processVanXa();
void processPump();

/******************************************************************************
* Local variables
******************************************************************************/
bool s_pumpIsOn = false;
uint32_t s_pumpStartTime = 0;
uint32_t timeOffVanXa;
bool vanXaIsOn = false;
bool turnOffAll = false;
uint32_t s_lastUpdateFilterTime = 0;


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
void setOutput(Output_t out, bool isON)
{
	if(g_ioTestEn)
		return;
	switch (out) {
		case OUT_PUMP:
			if(isON)
				TURN_ON_PUMP;
			else
				TURN_OFF_PUMP;
			break;
		case OUT_VAN:
			if(isON)
				TURN_ON_VAN_XA;
			else
				TURN_OFF_VAN_XA;
			break;
		default:
			break;
	}
}
void processVanXa()
{
	if(vanXaIsOn && timeIsAfter(g_sysTime , timeOffVanXa) )
	{
		setOutput(OUT_VAN, false);
		vanXaIsOn = false;
	}
}

void processPump()
{
	if(CHECK_CO_AP_THAP && CHECK_CO_AP_CAO && (!ADC_GetH2oDet(H2O_2)))
	{
		if(!s_pumpIsOn)
		{
			s_pumpIsOn = true;
			setOutput(OUT_PUMP, true);
			s_pumpStartTime = g_sysTime;
			s_lastUpdateFilterTime = g_sysTime;
			pumpControl_onVanXaInMs(15000);
		}
		if(elapsedTime(g_sysTime, s_lastUpdateFilterTime) > 600000)
		{
			uint32_t runTimeToUpdate = elapsedTime(g_sysTime , s_lastUpdateFilterTime);
			s_lastUpdateFilterTime = g_sysTime;
			filter_time_minusTime(runTimeToUpdate / 1000);
		}
		if(pumpControl_getTimePumpRun() > UserConfig_getWaitTimeUpdateTds())
		{
			if( ADC_SetDisableUpdateTds(false))
			{
				ADC_UpdateTdsDisplay();
			}
		}
	}
	else
	{
		if(s_pumpIsOn)
		{
			s_pumpIsOn = false;
			setOutput(OUT_PUMP, false);
			uint32_t runTimeToUpdate = elapsedTime(g_sysTime , s_lastUpdateFilterTime);
			filter_time_minusTime(runTimeToUpdate / 1000);
			pumpControl_onVanXaInMs(5000);
			// disable update TDS
			ADC_SetDisableUpdateTds(true);
		}
	}
}
/******************************************************************************
* Global functions
******************************************************************************/

void pumpControl_onVanXaInMs(uint32_t msTime)
{
	if(turnOffAll)
		return;
	timeOffVanXa = g_sysTime + msTime;
	setOutput(OUT_VAN, true);
	vanXaIsOn = true;
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
void pumpControl_process()
{
	if(turnOffAll)
		return;
	processPump();
	processVanXa();
}
uint32_t pumpControl_getTimePumpRun()
{
	if(s_pumpIsOn)
	{
		return (elapsedTime(g_sysTime, s_pumpStartTime));
	}else{
		return 0;
	}
}

void pumpControl_turnOffAll()
{
	setOutput(OUT_VAN, false);
	setOutput(OUT_PUMP, false);
	s_pumpIsOn = false;
	vanXaIsOn = false;
	turnOffAll = true;
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


