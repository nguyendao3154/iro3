/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        user_config.c
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


#include "user_config.h"
#include "flash_app.h"
#include <string.h>


/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/
UserConfig_t g_userConfig;
/******************************************************************************
* Constants and macros
******************************************************************************/
static const UserConfig_t USER_CONFIG_DEFAULD = {
#ifdef MODEL_LEAF
		// 300 600 3600 1000 1000 1000 1000 1000 1000
		.filterRunTimeTotal = {1080000,2160000,12960000,3600000,3600000,3600000,3600000,3600000,3600000},
		// tháng: 3 6 36 12 12 12 12 12 12
		.s_lifeTimeTotal = {7776000,15552000,93312000,31104000,31104000,31104000,31104000,31104000,31104000},
#endif
#ifdef MODEL_888_TN
		// 300 600 3600 1000 1000 1000 1000 1000 1000
		.filterRunTimeTotal = {1080000,2160000,12960000,3600000,3600000,3600000,3600000,3600000,3600000},
		// tháng: 3 6 36 12 12 12 12 12 12
		.s_lifeTimeTotal = {7776000,15552000,93312000,31104000,31104000,31104000,31104000,31104000,31104000},
#endif
#ifdef MODEL_888_TC
		// 300 600 600 3600 1000 1000 1000 1000 1000
		.filterRunTimeTotal = {1080000,2160000,2160000,12960000,3600000,3600000,3600000,3600000,3600000},
		// tháng: 3 6 6 36 12 12 12 12 12
		.s_lifeTimeTotal = {7776000,15552000,15552000,93312000,31104000,31104000,31104000,31104000,31104000},
#endif
#ifdef MODEL_IRO3_K92
		// 3600 700 1000 1000 1000 1000 1000 1000 1000
		.filterRunTimeTotal = {12960000,2520000,3600000,3600000,3600000,3600000,3600000,3600000,3600000},
		// tháng: 60 12 24 24 24 24 24 24 24
		.s_lifeTimeTotal = {155520000,31104000,62208000,62208000,62208000,62208000,62208000,62208000,62208000},
#endif
#ifdef MODEL_OPTIMUS_I1
		// 250 300 300 2000 600 600 600 600 600
		.filterRunTimeTotal = {900000,1080000,1080000,7200000,2160000,2160000,2160000,2160000,2160000},
		// tháng: 3 3 3 24 6 6 6 6 6
		.s_lifeTimeTotal = {7776000,7776000,7776000,62208000,15552000,15552000,15552000,15552000,15552000},
#endif
#ifdef MODEL_OPTIMUS_I2
		// 250 300 300 2000 600 600 600 600 600
		.filterRunTimeTotal = {900000,1080000,1080000,7200000,2160000,2160000,2160000,2160000,2160000},
		// tháng: 3 3 3 24 6 6 6 6 6
		.s_lifeTimeTotal = {7776000,7776000,7776000,62208000,15552000,15552000,15552000,15552000,15552000},
#endif
#ifdef MODEL_TOPBOX
		// 200 300 1500 400 400 400 400 400 400
		.filterRunTimeTotal = {720000,1080000,5400000,1440000,1440000,1440000,1440000,1440000,1440000},
		// tháng: 3 3 24 6 6 6 6 6 6
		.s_lifeTimeTotal = {7776000,7776000,62208000,15552000,15552000,15552000,15552000,15552000,15552000},
#endif
		.tdsLimitIn = 1000,
		.tdsLimitOut = 50,
		.tdsToTimeMode = false,
		.showTdsIn = false,
		.waitTimeUpdateTds = WAIT_TIME_UPDATE_TDS
};

/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/


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


void user_config_updateToFlash()
{
	flash_app_writeBlock((uint8_t *)&g_userConfig, USER_CONFIG_BLOCK, sizeof(g_userConfig));
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
void user_config_init()
{
    bool readOk = flash_app_readData((uint8_t*)&g_userConfig,USER_CONFIG_BLOCK,sizeof(g_userConfig));

    if(!readOk)
    {
    	memcpy(&g_userConfig,&USER_CONFIG_DEFAULD,sizeof(g_userConfig));
    	user_config_updateToFlash();
    }

}

void UserConfig_setFilterLifeTime(uint32_t lifeTime,uint8_t filterIndex)
{
	g_userConfig.filterRunTimeTotal[filterIndex] = lifeTime;
	user_config_updateToFlash();
}
void UserConfig_setTdsLimitIn(uint16_t tdsLimit)
{
	g_userConfig.tdsLimitIn = tdsLimit;
	user_config_updateToFlash();
}

void UserConfig_setTdsLimitOut(uint16_t tdsLimit)
{
	g_userConfig.tdsLimitOut = tdsLimit;
	user_config_updateToFlash();
}


uint16_t UserConfig_getDefaultLifeTimeHour(uint8_t filterIndex)
{
	 return (USER_CONFIG_DEFAULD.filterRunTimeTotal[filterIndex] / 3600);
}

uint32_t UserConfig_getWaitTimeUpdateTds()
{
#ifdef KAROFI
	return g_userConfig.waitTimeUpdateTds;
#endif
#ifdef KAROHOME
	return 0;
#endif
}
void UserConfig_setWaitTimeUpdateTds(uint32_t mS)
{
	g_userConfig.waitTimeUpdateTds = mS;
	user_config_updateToFlash();
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
void UserConfig_resetToDefault()
{
	memcpy(&g_userConfig,&USER_CONFIG_DEFAULD,sizeof(g_userConfig));
	user_config_updateToFlash();
}
