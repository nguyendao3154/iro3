/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        adc.c
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


#include <adc.h>
#include <Config_S12AD0.h>
#include <rx130/r_flash_rx130.h>

#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "user_config.h"
#include "CurentData.h"

//#include "Config_S12AD0.h"
/******************************************************************************
* External objects
******************************************************************************/
extern uint8_t g_adc_flag;
extern volatile uint8_t g_pwm_value;

/******************************************************************************
* Global variables
******************************************************************************/
int16_t  current_adc_tds_in = 0;
int16_t  current_adc_tds_out = 0;
PUBLIC bool g_disableTdsOut = false;
PUBLIC bool g_disableTdsIn = true;

/* Global flag to indicate A/D conversion operation is completed */

/******************************************************************************
* Constants and macros
******************************************************************************/
#define TIME_OUT_MAX  (10)

#define ADC_LOW_VALUE  (0)

#define ADC_MAX_SAMPLE  (200)

#define ADC_HIGH_VALUE  (500)

/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/


/******************************************************************************
* Local variables
******************************************************************************/

LOCAL const TDS_CALIB_PARAM_T TDS_IN_CONFIG_DEFAULD =
{
		.adc_value = {2591,2485,2360,1801,1700,1626,1580,1533,1476,1442,1403,1346,1088,898,762,610,510,404,327,237,(-24)},
		.tds_value = {0,4,11,20,30,40,50,60,71,80,90,105,201,303,402,502,602,706,807,1010,2010}

};


LOCAL const TDS_CALIB_PARAM_T TDS_OUT_CONFIG_DEFAULD =
{
		.adc_value = {3004,2885,2300,2137,2069,2006,1957,1926,1886,1844,1814,1145,908,733,591,462,434,430,430,425,-28},
		.tds_value = {0,9,23,31,43,51,61,69,79,91,100,214,307,400,493,600,629,629,629,629,1390}

};


LOCAL TDS_CONFIG_T s_tds_calib_param;

LOCAL TDS_T  s_tds_in;

LOCAL TDS_T  s_tds_out;

LOCAL uint16_t s_200ms_cnt ;

LOCAL uint16_t s_adc_h2o_det1 = 0;

LOCAL uint16_t s_cnt_h2o_det1 = 0;

LOCAL bool s_is_h2O_det1 = FALSE;

LOCAL uint16_t s_adc_h2o_det2 = 0;

LOCAL uint16_t s_cnt_h2o_det2 = 0;

LOCAL bool s_is_h2O_det2 = FALSE;

LOCAL bool s_disableUpdateTds = true;

//LOCAL uint16_t s_adc_low_value = 0;

/******************************************************************************
* Local functions
******************************************************************************/
uint16_t  ADC_GetTdsValue(TDS_E channel);

LOCAL void ADC_PushDataToQueue (int16_t data ,TDS_T* tds)
{
	     int16_t sample = 0;
		if(!QUEUE_QueueIsFull(tds->adc_sample))
		{
			tds->sum_tds_adc +=  data;
			QUEUE_EnQueue(tds->adc_sample,&data);
			tds->sma_tds_adc  = data;
		}
		else
		{

			QUEUE_DeQueue(tds->adc_sample,&sample);
			tds->sum_tds_adc = (tds->sum_tds_adc) + data - sample;
		//	tds->sign   = (tds->sum_tds_adc >=0 )?0:1;
			tds->sma_tds_adc = (int16_t) tds->sum_tds_adc/((tds->adc_sample)->max_count);
			QUEUE_EnQueue(tds->adc_sample,&data);

		}
}


LOCAL void ADC_InitConfigFlash()
{
    bool readOk = flash_app_readData((uint8_t*)&s_tds_calib_param,TDS_PARAM_BLOCK,sizeof(s_tds_calib_param));

    if(!readOk)
    {
    	memcpy(&(s_tds_calib_param.tds_in),&TDS_IN_CONFIG_DEFAULD,sizeof(s_tds_calib_param.tds_in));
    	memcpy(&(s_tds_calib_param.tds_out),&TDS_OUT_CONFIG_DEFAULD,sizeof(s_tds_calib_param.tds_out));
    	s_tds_calib_param.tds_out_max    = TDS_OUT_MAX_DEFAULT;
    	s_tds_calib_param.adc_h2o_det = ADC_H2O_DET_DEFAULT;
    	flash_app_writeBlock((uint8_t *)&s_tds_calib_param, TDS_PARAM_BLOCK, sizeof(s_tds_calib_param));
    }


}

LOCAL bool ADC_GetIndexCalibFromTds(TDS_E channel,uint16_t tds_value,uint8_t *index_ret)
{

	uint8_t				ret_index = 0;
	uint8_t				index_level = 0;
	TDS_CALIB_PARAM_T   *calib_param = (channel  == TDS_IN_VALUE)?&(s_tds_calib_param.tds_in): &(s_tds_calib_param.tds_out);

	if(tds_value > calib_param->tds_value[CALIB_POINT_MAX -1]) return false;
	for(index_level = 0; index_level < (CALIB_POINT_MAX -1) ; index_level++)
	{
		if((tds_value >= calib_param->tds_value[index_level]) && (tds_value < calib_param->tds_value[index_level+1]) )
		{
			if(((2*tds_value) > (calib_param->tds_value[index_level] + calib_param->tds_value[index_level+1]))
					|| (calib_param->tds_value[index_level] == 0))
				ret_index = index_level+1;
			else ret_index = index_level;
			break;
		}
	}
	*index_ret = ret_index;
	return true;
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
PUBLIC void ADC_Init()
{   
	R_Config_S12AD0_Create();
	s_tds_in.adc_sample = QUEUE_InitQueue(ADC_SAMPLE_QUEUE_SIZE,sizeof(int16_t));
	s_tds_out.adc_sample = QUEUE_InitQueue(ADC_SAMPLE_QUEUE_SIZE,sizeof(int16_t));
	ADC_InitConfigFlash();
	s_200ms_cnt = 0;
	PWM = 0;
	g_adc_flag = 0U;
	R_Config_S12AD0_Start();
	while((QUEUE_QueueIsEmpty(s_tds_out.adc_sample))||((QUEUE_QueueIsEmpty(s_tds_in.adc_sample))))
	{
    	if(g_adc_flag)
    	{
    		ADC_UpdateTds (0);
    	}

	}
	s_tds_out.tds_display = curentData_getLastTdsOut();

}






PUBLIC uint16_t  ADC_GetTdsValue(TDS_E channel)
{


	float				calculate_value = 0;
	float				slope;
	uint16_t            tds_return = 0;
	uint32_t				index_level = 0;
	TDS_CALIB_PARAM_T   *calib_param = (channel  == TDS_IN_VALUE)?&(s_tds_calib_param.tds_in): &(s_tds_calib_param.tds_out);
	int16_t adc0_value;
	if(channel == TDS_IN_VALUE)
	{
		if(g_disableTdsIn)
		{
			return 0;
		}
		adc0_value = s_tds_in.sma_tds_adc;
	}
	else if(channel == TDS_OUT_VALUE)
	{
		if(g_disableTdsOut)
		{
			return 0;
		}
		adc0_value = s_tds_out.sma_tds_adc;
	}

	if(adc0_value > calib_param->adc_value[0] )
	{
		tds_return = 0;
		return tds_return;
	}

	if(adc0_value < calib_param->adc_value[CALIB_POINT_MAX-1])
	{
		tds_return = calib_param->tds_value[CALIB_POINT_MAX-1];
		return tds_return;
	}

	for(index_level = 0; index_level < (CALIB_POINT_MAX -1) ; index_level++)
	{

		if((adc0_value <= calib_param->adc_value[index_level]) && (adc0_value >= calib_param->adc_value[index_level+1]) )
		{
			slope = ((float)(calib_param->tds_value[index_level+1] - calib_param->tds_value[index_level]))/((float)(calib_param->adc_value[index_level] - calib_param->adc_value[index_level+1]));
			calculate_value = calib_param->tds_value[index_level] + slope * ( calib_param->adc_value[index_level] - adc0_value);
			tds_return= (calculate_value <= 0)?0:(uint16_t) (calculate_value+0.5);
			break;
		}
	}

	return tds_return;

}

PUBLIC uint16_t  ADC_GetTdsValueDisplay(TDS_E channel)
{

	if(channel == TDS_IN_VALUE)
	{
		return s_tds_in.tds_display;
	}
	else
	{
		return s_tds_out.tds_display;
	}

}

void debugADC(int16_t adcIn,int16_t adcOut)
{
	char dbg[UART_SEND_MAX_LEN];
	sprintf(dbg,"[%d,%d]",adcIn,adcOut);
//	sprintf(dbg,"%d,%d\r\n",adcIn,adcOut);
	UART_Debug (dbg);
}

PUBLIC void   ADC_UpdateTds (uint8_t state)
{
	uint16_t adc_result_tds_in = 0;
	uint16_t adc_result_tds_out = 0;
	int16_t  current_adc_tds_in = 0;
	int16_t  current_adc_tds_out = 0;

	s_200ms_cnt = s_200ms_cnt +1;
	if(s_200ms_cnt >= ADC_SAMPLE_CAL_MAX)
	{
		s_200ms_cnt = 0;
		//update tds in
		current_adc_tds_in = ((s_tds_in.high_cnt == 0)| (s_tds_in.low_cnt==0))?0: \
				((s_tds_in.sum_adc_high/s_tds_in.high_cnt) - (s_tds_in.sum_adc_low/s_tds_in.low_cnt));
		ADC_PushDataToQueue(current_adc_tds_in,&s_tds_in);

	//	update tds out
		current_adc_tds_out = ((s_tds_out.high_cnt == 0)| (s_tds_out.low_cnt==0))?0: \
				((s_tds_out.sum_adc_high/s_tds_out.high_cnt) - (s_tds_out.sum_adc_low/s_tds_out.low_cnt));
		ADC_PushDataToQueue(current_adc_tds_out,&s_tds_out);

//		debugADC(s_tds_out.sma_tds_adc,adc_tds_out);
		debugADC(current_adc_tds_in,current_adc_tds_out);
		s_tds_in.high_cnt = 0;
		s_tds_in.low_cnt  = 0;
		s_tds_in.sum_adc_high = 0;
		s_tds_in.sum_adc_low  = 0;
		s_tds_out.high_cnt = 0;
		s_tds_out.low_cnt  = 0;
		s_tds_out.sum_adc_high = 0;
		s_tds_out.sum_adc_low  = 0;
	    //check h2o det
		R_Config_S12AD0_Get_ValueResult(H20_CHANNEL_DETECT_1,&s_adc_h2o_det1);
		if(s_adc_h2o_det1 < s_tds_calib_param.adc_h2o_det)
		{
			s_cnt_h2o_det1 ++;
			if(s_cnt_h2o_det1> H2O_DET_CNT_MAX)
			{
				s_is_h2O_det1 =TRUE;
				s_cnt_h2o_det1 = 0;
			}
		}
		else
		{
			s_is_h2O_det1 =FALSE;
			s_cnt_h2o_det1 = 0;
		}
		//check h2o det 2
		R_Config_S12AD0_Get_ValueResult(H20_CHANNEL_DETECT_2,&s_adc_h2o_det2);
		if(s_adc_h2o_det2 < s_tds_calib_param.adc_h2o_det)
		{
			s_cnt_h2o_det2 ++;
			if(s_cnt_h2o_det2> H2O_DET_CNT_MAX)
			{
				s_is_h2O_det2 =TRUE;
				s_cnt_h2o_det2 = 0;
			}
		}
		else
		{
			s_is_h2O_det2 =FALSE;
			s_cnt_h2o_det2 = 0;
		}
		goto end_function;
	}
//	R_Config_S12AD0_Get_ValueResult(TDS_IN_CHANNEL, &adc_result_tds_in);
	R_Config_S12AD0_Get_ValueResult(TDS_OUT_CHANNEL,&adc_result_tds_out);

// 0 với mạch test 1 với mạch cũ
#ifdef HW_VER_214
	if(g_pwm_value ==  0)
#else
	if(g_pwm_value ==  1)
#endif
	{

		s_tds_in.low_cnt ++;
		s_tds_in.sum_adc_low  += adc_result_tds_in;

		s_tds_out.low_cnt ++;
		s_tds_out.sum_adc_low  += adc_result_tds_out;

	}
	else
	{

		s_tds_in.high_cnt ++;
		s_tds_in.sum_adc_high  += adc_result_tds_in;

		s_tds_out.high_cnt ++;
		s_tds_out.sum_adc_high  += adc_result_tds_out;

	}


    end_function:


	g_adc_flag = 0U;
}


PUBLIC ERR_E ADC_CalibTdsValue(uint16_t tdsvalue,TDS_E channel)
{
	uint8_t index = 0;
	ERR_E f_ret = OK;
	bool ret = false;
	if(ADC_GetIndexCalibFromTds(channel,tdsvalue,&index) == true)
	{
		if(channel  == TDS_IN_VALUE)
		{
			s_tds_calib_param.tds_in.tds_value[index] = tdsvalue;
//			s_tds_calib_param.tds_in.adc_value[index] = current_adc_tds_in;
			s_tds_calib_param.tds_in.adc_value[index] = s_tds_in.sma_tds_adc;
		}
		else if(channel  == TDS_OUT_VALUE)
		{
			s_tds_calib_param.tds_out.tds_value[index] = tdsvalue;
//			s_tds_calib_param.tds_out.adc_value[index] = current_adc_tds_out;
			s_tds_calib_param.tds_in.adc_value[index] = s_tds_out.sma_tds_adc;
		}
		ret = flash_app_writeBlock((uint8_t *)&s_tds_calib_param, TDS_PARAM_BLOCK, sizeof(s_tds_calib_param));
		f_ret = (ret == true)?OK:ERR;
		return f_ret;
	}
	else return ERR;

}


PUBLIC void ADC_ClearH2oDet()
{
	s_is_h2O_det1 = 0;
	s_is_h2O_det2 = 0;
}

PUBLIC bool ADC_GetH2oDet(H2O_T index)
{
	if(index == H2O_1) return s_is_h2O_det1 ;
	if(index == H2O_2) return s_is_h2O_det2 ;
	return true;
}

PUBLIC void ADC_UpdateTdsDisplay()
{
	if(s_disableUpdateTds)
	{
		return;
	}
// nếu tds vượt ngưỡng, 2 lần mới hiển thị
	static uint8_t tdsLimitCnt = 0;
	if(ADC_GetTdsValue(TDS_OUT_VALUE) > g_userConfig.tdsLimitOut)
	{
		if(tdsLimitCnt < 1)
		{
			tdsLimitCnt ++;
			return;
		}
	}else{
		tdsLimitCnt = 0;
	}
// ----------------------------------------
	s_tds_in.tds_display = ADC_GetTdsValue(TDS_IN_VALUE);
	s_tds_out.tds_display = ADC_GetTdsValue(TDS_OUT_VALUE);
	curentData_updateTdsMin(s_tds_out.tds_display);

}


PUBLIC ERR_E ADC_GetCalibTdsParam(TDS_E channel,uint8_t* out)
{
	TDS_CALIB_PARAM_T   *calib_param = (channel  == TDS_IN_VALUE)?&(s_tds_calib_param.tds_in): &(s_tds_calib_param.tds_out);
	for(uint8_t i = 0; i< CALIB_POINT_MAX; i++)
	{
		char calibStr[20]= "";
		sprintf(calibStr,"(%d,%d)",calib_param->tds_value[i],calib_param->adc_value[i]);
		strcat((char*)out,calibStr);
	}
	return OK;
}

PUBLIC ERR_E ADC_GetAdcTable(TDS_E channel,uint8_t* out)
{
	TDS_CALIB_PARAM_T   *calib_param = (channel  == TDS_IN_VALUE)?&(s_tds_calib_param.tds_in): &(s_tds_calib_param.tds_out);
	for(uint8_t i = 0; i< CALIB_POINT_MAX; i++)
	{
		char calibStr[20]= "";
		if(i == (CALIB_POINT_MAX - 1))
			sprintf(calibStr,"%d",calib_param->adc_value[i]);
		else
			sprintf(calibStr,"%d,",calib_param->adc_value[i]);
		strcat((char*)out,calibStr);
	}
	return OK;
}

PUBLIC ERR_E ADC_GetTdsTable(TDS_E channel,uint8_t* out)
{
	TDS_CALIB_PARAM_T   *calib_param = (channel  == TDS_IN_VALUE)?&(s_tds_calib_param.tds_in): &(s_tds_calib_param.tds_out);
	for(uint8_t i = 0; i< CALIB_POINT_MAX; i++)
	{
		char calibStr[20]= "";
		if(i == (CALIB_POINT_MAX - 1))
			sprintf(calibStr,"%d",calib_param->tds_value[i]);
		else
			sprintf(calibStr,"%d,",calib_param->tds_value[i]);
		strcat((char*)out,calibStr);
	}
	return OK;
}

PUBLIC ERR_E ADC_CalibTdsValueFromUart(uint16_t tdsvalue,TDS_E channel,uint8_t index)
{
	ERR_E f_ret = OK;
	bool ret = false;
	if(index > (CALIB_POINT_MAX -1))
	{
		return ERR;
	}
	else
	{
		if(channel  == TDS_IN_VALUE)
		{
			s_tds_calib_param.tds_in.tds_value[index] = tdsvalue;
			s_tds_calib_param.tds_in.adc_value[index] = (index == 0)?(s_tds_in.sma_tds_adc - 10) :s_tds_in.sma_tds_adc;
		}
		else if(channel  == TDS_OUT_VALUE)
		{
			s_tds_calib_param.tds_out.tds_value[index] = tdsvalue;
			s_tds_calib_param.tds_out.adc_value[index] = (index == 0)?(s_tds_out.sma_tds_adc -10):s_tds_out.sma_tds_adc;
		}
		ret = flash_app_writeBlock((uint8_t *)&s_tds_calib_param, TDS_PARAM_BLOCK, sizeof(s_tds_calib_param));
		f_ret = (ret == true)?OK:ERR;
		return f_ret;
	}
}

PUBLIC ERR_E ADC_WriteAdcFromUart(int* adcTable,TDS_E channel)
{
	ERR_E f_ret = OK;
	bool ret = false;
	if(channel  == TDS_OUT_VALUE)
	{
		for(uint8_t i=0; i < CALIB_POINT_MAX; i++)
		{
			s_tds_calib_param.tds_out.adc_value[i] = (int16_t)adcTable[i];
		}
	}
	flash_block_address_t block = TDS_PARAM_BLOCK;
	ret = flash_app_writeBlock((uint8_t *)&s_tds_calib_param, block, sizeof(s_tds_calib_param));
	f_ret = (ret == true)?OK:ERR;
	return f_ret;
}

PUBLIC ERR_E ADC_WriteTdsFromUart(int* tdsTable,TDS_E channel)
{
	ERR_E f_ret = OK;
	bool ret = false;
	if(channel  == TDS_OUT_VALUE)
	{
		for(uint8_t i=0; i < CALIB_POINT_MAX; i++)
		{
			s_tds_calib_param.tds_out.tds_value[i] = (int16_t)tdsTable[i];
		}
	}
	flash_block_address_t block = TDS_PARAM_BLOCK;
	ret = flash_app_writeBlock((uint8_t *)&s_tds_calib_param, block, sizeof(s_tds_calib_param));
	f_ret = (ret == true)?OK:ERR;
	return f_ret;
}

PUBLIC bool ADC_SetDisableUpdateTds(bool disable)
{
	if(s_disableUpdateTds == disable)
		return false;
	else
	{
		s_disableUpdateTds = disable;
		return true;
	}
}

PUBLIC void ADC_process()
{
//	if((g_sysTime >= 60000) && (g_sysTime < 65000) && (s_tds_out.tds_display == 0))
//	{
//		s_tds_out.tds_display = ADC_GetTdsValue(TDS_OUT_VALUE);
//	}
}
