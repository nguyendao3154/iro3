/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
****************************************************************************
*
* @file        UIControl.c
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
#include "UIControl.h"
#include "display.h"
#include "user_config.h"
#include "filter_time.h"
#include "timeCheck.h"
#include "errorCheck.h"
#include "adc.h"
#include "tools.h"
#include "display.h"
#include "filterExpire.h"
#include "CurentData.h"

/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define HOLD_TIME1 2000
#define HOLD_TIME2 3000
#define HOLD_TIME3 7000

#define MAX_TIME_WAIT 30000
#define TIME_BUZZER_ON 300 //ms


/******************************************************************************
* Local types
******************************************************************************/
typedef enum UI_State
{
	UI_STATE_TDS_OUT,
	UI_STATE_FILTER_1,  // Fill 1
	UI_STATE_FILTER_2,
	UI_STATE_FILTER_3,
	UI_STATE_FILTER_4,
	UI_STATE_FILTER_5,
	UI_STATE_FILTER_6,
	UI_STATE_FILTER_7,
	UI_STATE_FILTER_8,
	UI_STATE_FILTER_MAX  // Fill 9
}UI_State_t;

// define có hiển thị màn hình tds out hay không
#ifdef ENABLE_TDS_OUT
	#define UI_STATE_MIN UI_STATE_TDS_OUT
#else
	#define UI_STATE_MIN UI_STATE_FILTER_1
#endif
//---------------------------------------------

// define màn hình mặc định
// nếu có tds out mặc định là tds out
#ifdef ENABLE_TDS_OUT
#define UI_STATE_DEFAULD UI_STATE_TDS_OUT
#endif

#ifdef MODEL_888_TN
#define UI_STATE_DEFAULD UI_STATE_FILTER_3
#endif

#ifdef MODEL_888_TC
#define UI_STATE_DEFAULD UI_STATE_FILTER_4
#endif

#ifdef  MODEL_OPTIMUS_I1
#define UI_STATE_DEFAULD UI_STATE_FILTER_1
#endif
//----------------------------------------------

#define UI_STATE_IS_FILTER(state) ((state >= UI_STATE_FILTER_1) && (state <= UI_STATE_FILTER_MAX))
#define UI_STATE_TO_FILTER_INDEX(state) (state - UI_STATE_FILTER_1)
typedef enum
{
	UI_MODE_NOMAL,
	UI_MODE_SETTING,
}UIMode_t ;

typedef enum
{
	TDS_SETTING_MODE_LIMIT,
	TDS_SETTING_MODE_CALIB,
}TdsSettingMode_t ;

typedef enum
{
	BTN_STATE_RELEASE = 0,
	BTN_STATE_PRESS,
	BTN_STATE_HOLD1,
	BTN_STATE_HOLD2,
	BTN_STATE_HOLD3,
}BtnState_t;
typedef struct
{
	BtnState_t state;
	uint32_t timePress;
}BtnControl_t;
typedef enum{
	FAST_CHAGNE_NON,
	FAST_CHAGNE_PLUS,
	FAST_CHAGNE_MINUS
}fashChange_t ;
/******************************************************************************
* Local function prototypes
******************************************************************************/
void UIControl_btnProcess();
void UIControl_btnHold_cb(ButtonId_t btn,uint32_t holdingTime);
void UIControl_switchUiStateTo(UI_State_t newState);
void UIControl_updateUI();
void UIControl_resetSettingNumber();
void UIControl_setLockState(bool isLock);


/******************************************************************************
* Local variables
******************************************************************************/
UI_State_t s_UIState = UI_STATE_DEFAULD;
BtnControl_t btnControls[BUTTON_NUM];
UIMode_t s_uiMode = UI_MODE_NOMAL;
uint32_t s_lastPressTime;
TdsSettingMode_t tdsSettingMode;
fashChange_t s_fastChangeState = FAST_CHAGNE_NON;
bool s_needReset = false;
uint32_t s_timeReset;
uint8_t s_uiStateCnt = UI_STATE_FILTER_1 + FILTER_NUM;
bool s_isKeyBoardLock = true;
/******************************************************************************
* Local functions
******************************************************************************/
void saveFilterLifeTime(uint8_t index)
{
	UserConfig_setFilterLifeTime(Led7seg_getNumberInLed4()*3600,index);
	filter_time_resetTimeAtIndex(index);
	filterExpire_resetTimeAtIndex(index);
}
void saveCurentSetingNumer()
{
	switch (s_UIState) {
		case UI_STATE_TDS_OUT:
		{
			if(tdsSettingMode == TDS_SETTING_MODE_LIMIT)
				UserConfig_setTdsLimitOut(Led7seg_getNumberInLed4());
			else if(tdsSettingMode == TDS_SETTING_MODE_CALIB)
				ADC_CalibTdsValue(Led7seg_getNumberInLed4(),TDS_OUT_VALUE);
			break;
		}
		default:
			break;
	}
	if(UI_STATE_IS_FILTER(s_UIState))
	{
		saveFilterLifeTime(UI_STATE_TO_FILTER_INDEX(s_UIState));
	}
}
void UIControl_btnHold_cb(ButtonId_t btn,uint32_t holdingTime)
{
	s_lastPressTime = g_sysTime;
	if((btn == BUTTON_ID_SET) && (holdingTime == HOLD_TIME2))
	{
		if(s_uiMode == UI_MODE_SETTING)
		{
			s_uiMode = UI_MODE_NOMAL;
			Buzzer_onInMs(TIME_BUZZER_ON);
			saveCurentSetingNumer();
		}
		else
		{
			if(s_isKeyBoardLock)
			{
				Buzzer_onInMs(TIME_BUZZER_ON);
				UIControl_setLockState(false);
			}else if(s_UIState == UI_STATE_DEFAULD)
			{
				Buzzer_onInMs(TIME_BUZZER_ON);
				UIControl_setLockState(true);
			}
		}
	}
	else if((btn == BUTTON_ID_SELECT) && (holdingTime == HOLD_TIME2) && !UIControl_stateIsLock())
	{
		if(s_uiMode == UI_MODE_NOMAL)
		{
			s_uiMode = UI_MODE_SETTING;
			tdsSettingMode = TDS_SETTING_MODE_LIMIT;
			UIControl_resetSettingNumber();
			Buzzer_onInMs(TIME_BUZZER_ON);
		}
	}
	else if((btn == BUTTON_ID_PLUS) && (holdingTime == HOLD_TIME2) && !UIControl_stateIsLock())
	{
		if((s_uiMode == UI_MODE_NOMAL) && ((s_UIState == UI_STATE_TDS_OUT)))
		{
			s_uiMode = UI_MODE_SETTING;
			tdsSettingMode = TDS_SETTING_MODE_CALIB;
			UIControl_resetSettingNumber();
			Buzzer_blink(2);
		}
	}
	else if((btn == BUTTON_ID_SELECT) && (holdingTime == HOLD_TIME3) && (s_UIState == UI_STATE_DEFAULD))
	{
		flash_app_factoryReset();
		s_needReset = true;
		s_timeReset = g_sysTime + 500;
		Buzzer_onInMs(300);
	}
	if((s_uiMode == UI_MODE_SETTING) && (holdingTime == HOLD_TIME1))
	{
		if(btn == BUTTON_ID_PLUS)
		{
			s_fastChangeState = FAST_CHAGNE_PLUS;
		}
		else if(btn == BUTTON_ID_MINUS)
		{
			s_fastChangeState = FAST_CHAGNE_MINUS;
		}
	}
}
void UIControl_updateUI()
{
	if(s_isKeyBoardLock && ErrorCheck_haveError())
	{
		Display_showCurrentError();
	}else
	{
		switch (s_UIState) {
			case UI_STATE_TDS_OUT:
				Display_showTdsOut();
				break;
			default:
				break;
		}
		if(UI_STATE_IS_FILTER(s_UIState))
		{
			Display_showFilterTime(UI_STATE_TO_FILTER_INDEX(s_UIState));
		}
	}

}
void UIControl_resetSettingNumber()
{
	switch (s_UIState) {
		case UI_STATE_TDS_OUT:
			if(tdsSettingMode == TDS_SETTING_MODE_LIMIT)
			{
				Display_showTdsOutLimit();
			}else
			{
				Display_showTdsOut();
			}
			break;
		default:
			break;
	}
	if(UI_STATE_IS_FILTER(s_UIState))
	{
		Display_showFilterLifeTime(UI_STATE_TO_FILTER_INDEX(s_UIState));
	}
}

void UIControl_setLockState(bool isLock)
{
	s_isKeyBoardLock = isLock;
	if(isLock)
	{
		if(!ErrorCheck_haveError()) Led7_ReduceIntensity(true);
		s_UIState = UI_STATE_DEFAULD;
	}else
	{
		Display_turnOffBlinkled();
		Led7_ReduceIntensity(false);
		s_UIState = UI_STATE_FILTER_1;
	}
	UIControl_updateUI();
}

void UIControl_switchUiStateTo(UI_State_t newState)
{
	s_UIState = newState;
	UIControl_updateUI();
}
void UIControl_goNextState()
{
	UI_State_t newState = s_UIState +1;
	if(newState == s_uiStateCnt)
		newState = UI_STATE_MIN;
	UIControl_switchUiStateTo(newState);
}
void UIControl_goPrevState()
{
	UI_State_t newState;
	if(s_UIState == UI_STATE_MIN)
	{
		newState = s_uiStateCnt - 1;
	}else
	{
		newState = s_UIState - 1;
	}
	UIControl_switchUiStateTo(newState);
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
void UIControl_btnProcess()
{
	for(ButtonId_t btnId = 0; btnId< BUTTON_NUM; btnId++)
	{
		if(btnControls[btnId].state != BTN_STATE_RELEASE)
		{
			if(elapsedTime(g_sysTime , btnControls[btnId].timePress) > HOLD_TIME1)
			{
				if(btnControls[btnId].state == BTN_STATE_PRESS)
				{
					btnControls[btnId].state = BTN_STATE_HOLD1;
					UIControl_btnHold_cb(btnId,HOLD_TIME1);
				}
				if(elapsedTime(g_sysTime , btnControls[btnId].timePress) > HOLD_TIME2)
				{
					if(btnControls[btnId].state == BTN_STATE_HOLD1)
					{
						btnControls[btnId].state = BTN_STATE_HOLD2;
						UIControl_btnHold_cb(btnId,HOLD_TIME2);
					}
					if(elapsedTime(g_sysTime , btnControls[btnId].timePress) > HOLD_TIME3)
					{
						if(btnControls[btnId].state == BTN_STATE_HOLD2)
						{
							btnControls[btnId].state = BTN_STATE_HOLD3;
							UIControl_btnHold_cb(btnId,HOLD_TIME3);
						}
					}
				}

			}
		}
	}
}
void fashChangeProcess()
{
	if(s_fastChangeState == FAST_CHAGNE_MINUS)
	{
		Led7seg_reduceNumberInLed4(1);
		s_lastPressTime = g_sysTime;
	}
	else if(s_fastChangeState == FAST_CHAGNE_PLUS)
	{
		s_lastPressTime = g_sysTime;
		Led7seg_increaseNumberInLed4(1);
	}
}
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
bool UIControl_stateIsLock()
{
	return s_isKeyBoardLock;
}


void UIControl_process()
{
	UIControl_btnProcess();
	if(s_uiMode == UI_MODE_NOMAL)
	{
		UIControl_updateUI();
	}
	if(!s_isKeyBoardLock && (elapsedTime(g_sysTime,s_lastPressTime) > MAX_TIME_WAIT))
	{
		UIControl_setLockState(true);
		s_uiMode = UI_MODE_NOMAL;

	}
	fashChangeProcess();
	if(s_needReset && (timeIsAfter(g_sysTime, s_timeReset)))
	{
		mySoftwareReset();
	}
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


void TouchBtnPressed_cb(ButtonId_t btn)
{
	btnControls[btn].state = BTN_STATE_PRESS;
	btnControls[btn].timePress = g_sysTime;
}


void TouchBtnHoldRelease_cb(ButtonId_t btn)
{
	if(!UIControl_stateIsLock() )
	{
		if(btnControls[btn].state == BTN_STATE_PRESS)
		{
			switch (btn) {
				case BUTTON_ID_SELECT:
					if(s_uiMode == UI_MODE_NOMAL)
					{
						UIControl_switchUiStateTo(UI_STATE_DEFAULD);
					}
					else if(s_uiMode == UI_MODE_SETTING && UI_STATE_IS_FILTER(s_UIState))
					{
						Led7seg_SetNumberInLed4(UserConfig_getDefaultLifeTimeHour(UI_STATE_TO_FILTER_INDEX(s_UIState)));
					}
					break;
				case BUTTON_ID_PLUS:
					if(s_uiMode == UI_MODE_NOMAL)
					{
						UIControl_goNextState();
					}
					else if(s_uiMode == UI_MODE_SETTING)
					{
						Led7seg_increaseNumberInLed4(1);
					}
					break;
				case BUTTON_ID_MINUS:
					if(s_uiMode == UI_MODE_NOMAL)
					{
						UIControl_goPrevState();
					}
					else if(s_uiMode == UI_MODE_SETTING)
					{
						Led7seg_reduceNumberInLed4(1);
					}
					break;
				case BUTTON_ID_SET:
					if(s_uiMode == UI_MODE_SETTING)
					{
						UIControl_resetSettingNumber();
					}
					break;
				default:
					break;
			}

		}
		else if((btn == BUTTON_ID_PLUS) || (btn == BUTTON_ID_MINUS))
		{
			s_fastChangeState = FAST_CHAGNE_NON;
		}


	}
	btnControls[btn].state = BTN_STATE_RELEASE;
	s_lastPressTime = g_sysTime;
}

// callback
void ErroCheck_newError_cb(ErrorType_t newError)
{
	Display_showNewError(newError);
	UIControl_setLockState(true);
	Led7_ReduceIntensity(false);
}

void ErrorCheck_allErrorAreRemoved_cb()
{
	Display_turnOffBlinkled();
	if(s_isKeyBoardLock) Led7_ReduceIntensity(true);

}
