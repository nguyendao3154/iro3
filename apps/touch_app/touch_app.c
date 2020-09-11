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

#include "ctsu_pin_info.h"
#include "r_ctsu_rx_config.h"
#include "r_ctsu_rx_if.h"
#include "r_touch_rx_if.h"
#include "r_touch_button_if.h"

#include "ctsu/r_touch.h"
/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define MAX_CONFIGS	(METHOD_NUM)


/***********************************************************************************************************************
* Local types
***********************************************************************************************************************/
typedef struct
{
	uint32_t touch_id;
	uint32_t ctsu_id;
}cap_touch_system_t;

typedef union
{
	uint32_t			any;
	ctsu_err_t 			ctsu;
	touch_err_t 		touch;
	touch_button_err_t	button;
}error_t;

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

extern ctsu_pin_info_t const g_ctsu_pin_info;
extern touch_button_cfg_t const * const g_all_button_configs[];
extern const uint16_t g_button_config_count;
extern touch_cfg_t const * const g_all_touch_configs[];
extern const uint8_t g_num_touch_configs;
extern ctsu_cfg_t const * const g_all_ctsu_configs[];
extern const uint8_t g_num_ctsu_configs;


/******************************************************************************
* Local function prototypes
******************************************************************************/
static void ctsu_pin_setup(uint64_t ts_enable_mask);


/******************************************************************************
* Local variables
******************************************************************************/
static cap_touch_system_t id[MAX_CONFIGS];



/******************************************************************************
* Local functions
******************************************************************************/

/***********************************************************************************************************************
* Function Name: ctsu_pin_setup
* Description  : Set up the GPIO and MPC for the pins used by all configurations.
* Arguments    : ts_enable_mask -
*                    Mask of all pins being used by all configurations.
* Return Value : None
***********************************************************************************************************************/
static void ctsu_pin_setup(uint64_t ts_enable_mask)
{
	uint64_t bitmask;
	mpc_err_t mpc_err;
	ctsu_err_t ctsu_err;
	uint32_t ctsu_pin_itr;
	mpc_config_t ctsu_pin;

    /* Initialize CTSU Pins */
    ctsu_pin.irq_enable = false;
    ctsu_pin.pin_function = g_ctsu_pin_info.pin_function;
    ctsu_pin.analog_enable = false;

    /* Discharge TSCAP used by the CTSU */
    R_GPIO_PinControl(g_ctsu_pin_info.p_tscap[0], GPIO_CMD_ASSIGN_TO_GPIO);
    R_GPIO_PinControl(g_ctsu_pin_info.p_tscap[0], GPIO_CMD_IN_PULL_UP_DISABLE);
    R_GPIO_PinDirectionSet(g_ctsu_pin_info.p_tscap[0], GPIO_DIRECTION_OUTPUT);
    R_GPIO_PinWrite(g_ctsu_pin_info.p_tscap[0], GPIO_LEVEL_LOW);

    /* Wait for discharge to complete */
    R_BSP_SoftwareDelay(10, BSP_DELAY_MILLISECS);

    /* Initialize CTSU TSCAP */
    R_GPIO_PinDirectionSet(g_ctsu_pin_info.p_tscap[0], GPIO_DIRECTION_INPUT);
    mpc_err = R_MPC_Write(g_ctsu_pin_info.p_tscap[0], &ctsu_pin);
    R_GPIO_PinControl(g_ctsu_pin_info.p_tscap[0], GPIO_CMD_ASSIGN_TO_PERIPHERAL);
    assert(MPC_SUCCESS==mpc_err);

    for(ctsu_pin_itr = 0; ctsu_pin_itr < g_ctsu_pin_info.tspin_count; ctsu_pin_itr++)
    {
    	/* Initialize CTSU TSn */
    	bitmask = ((uint64_t)1U<<ctsu_pin_itr);
    	if((ts_enable_mask & bitmask)==bitmask)
    	{
    		/* TS Pin is enabled */
    		R_GPIO_PinControl(g_ctsu_pin_info.p_tspin[ctsu_pin_itr], GPIO_CMD_ASSIGN_TO_GPIO);
    	    R_GPIO_PinControl(g_ctsu_pin_info.p_tspin[ctsu_pin_itr], GPIO_CMD_IN_PULL_UP_DISABLE);
    		R_GPIO_PinDirectionSet(g_ctsu_pin_info.p_tspin[ctsu_pin_itr], GPIO_DIRECTION_INPUT);
    		mpc_err = R_MPC_Write(g_ctsu_pin_info.p_tspin[ctsu_pin_itr], &ctsu_pin);
    		R_GPIO_PinControl(g_ctsu_pin_info.p_tspin[ctsu_pin_itr], GPIO_CMD_ASSIGN_TO_PERIPHERAL);
    		assert(MPC_SUCCESS==mpc_err);
    	}
    	else
    	{
    		/* Unused CTSU TSn Pins should be set to GND */
    		R_GPIO_PinControl(g_ctsu_pin_info.p_tspin[ctsu_pin_itr], GPIO_CMD_ASSIGN_TO_GPIO);
    		R_GPIO_PinDirectionSet(g_ctsu_pin_info.p_tspin[ctsu_pin_itr], GPIO_DIRECTION_OUTPUT);
    	}
    }
    (void)mpc_err;
    (void)ctsu_err;
}

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
void TOUCH_init()
{
	uint32_t widget_id;
	uint8_t itr = 0;
	uint64_t ts_mask_all_cfgs = 0;
	uint8_t num_buttons = 0;
	error_t err =
	{
		.any = 0,
	};

	for(itr = 0; itr < g_num_ctsu_configs; itr++)
	{
		ctsu_cfg_t const * const p_cfg = g_all_ctsu_configs[itr];
		uint64_t ts_mask_this_cfg = p_cfg->p_ctsu_settings->ctsuchac4.byte;
		ts_mask_this_cfg <<= 8;
		ts_mask_this_cfg |= p_cfg->p_ctsu_settings->ctsuchac3.byte;
		ts_mask_this_cfg <<= 8;
		ts_mask_this_cfg |= p_cfg->p_ctsu_settings->ctsuchac2.byte;
		ts_mask_this_cfg <<= 8;
		ts_mask_this_cfg |= p_cfg->p_ctsu_settings->ctsuchac1.byte;
		ts_mask_this_cfg <<= 8;
		ts_mask_this_cfg |= p_cfg->p_ctsu_settings->ctsuchac0.byte;

		ts_mask_all_cfgs |= ts_mask_this_cfg;
	}

	/* Setup CTSU Pins */
	ctsu_pin_setup(ts_mask_all_cfgs);

	/* Open TOUCH layer */
	for(itr = 0; (itr < g_num_touch_configs); itr++)
	{
		touch_cfg_t const * const p_cfg = g_all_touch_configs[itr];
		err.touch = R_TOUCH_Open(&id[itr].touch_id, p_cfg);
		assert(TOUCH_SUCCESS == err.touch);
	}

	/* Open CTSU layers */
	for(itr = 0; itr < g_num_ctsu_configs; itr++)
	{
		ctsu_cfg_t const * const p_cfg = g_all_ctsu_configs[itr];
		err.ctsu = R_CTSU_Open(&id[itr].ctsu_id, p_cfg);
#if (CTSU_CFG_CALIBRATION_ENABLE==true)
		err.ctsu |= R_CTSU_Calibrate(id[itr].ctsu_id);
#endif
		assert(CTSU_SUCCESS == err.ctsu);
	}

	/* Open Button layer */
	for(itr = 0; itr < g_button_config_count; itr++)
	{
		touch_button_cfg_t const * const p_cfg = g_all_button_configs[itr];
		widget_id = UINT32_MAX;
		err.button = R_TOUCH_ButtonOpen(&widget_id, p_cfg);
		assert(TOUCH_BUTTON_SUCCESS==err.button);
		num_buttons += 1;
	}

	assert(0 == err.any);

	itr = 0;
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
	error_t err =
	{
		.any = 0,
	};
	uint8_t itr = 0;
	err.touch = R_TOUCH_Update(id[itr].touch_id);
	if (TOUCH_SUCCESS == err.touch)
	{
		if (TOUCH_SUCCESS == err.touch)
		{
			itr = ((itr + 1) < MAX_CONFIGS) ? (itr + 1):0;
		}
		err.touch = R_TOUCH_StartScan(id[itr].touch_id);
	}
}


