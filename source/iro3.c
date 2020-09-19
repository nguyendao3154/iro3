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
#include "fsl_lptmr.h"
#include "fsl_ftm.h"
#include "gpio.h"
#include "config.h"
#include "adc.h"
#include "display.h"
#include "touch_app.h"
//#include "flash_app.h"
// #include "filter_time.h"
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
#include "peripherals.h"
#include "queue.h"


/******************************************************************************
* External objects
******************************************************************************/
volatile uint8_t g_run200usFlag;
uint8_t g_adc_flag;
bool g_disableLedKey = false;
volatile uint32_t g_sysTime = 0;
volatile uint8_t g_run1msFlag = 0;
volatile uint8_t g_run200usFlag = 0;
volatile uint8_t s_200usTick = 0;

volatile uint8_t g_pwm_timer;
volatile uint8_t g_pwm_value = 0;
volatile uint8_t cnt = 0;
/******************************************************************************
* Global variables
******************************************************************************/
volatile uint8_t g_rx_char;
/* Flag used to control transmission to PC terminal */
volatile uint8_t g_tx_flag = 0U;
/* Flag used to detect whether data is received */
volatile uint8_t g_rx_flag;
/* Flag used to detect completion of transmission */
uint16_t result_tsi[4];
static QUEUE_NODE_T* s_uart_data_queue ;

LOCAL volatile uint16_t  s_rev_index = 0;

LOCAL  uint16_t  s_rev_index_pre = 0;

LOCAL volatile bool  s_rev_done = false;

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
void PWT_LPTMR0_IRQHandler(void)
{

	LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
    /* Start user code for r_Config_CMT1_cmi1_interrupt. Do not edit comment generated here */
	if(++s_200usTick == 5)
	{
		s_200usTick = 0;
		g_sysTime ++;
		g_run1msFlag = 1;
	}
	g_run200usFlag = 1;
	Led7seg_scanLed();

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
void main(void);
void main(void)
{
	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitBootPeripherals();
	LPTMR_StartTimer(LPTMR0);
    GPIO_Init();
	TOUCH_init();
	UART_Init();
	UART_UartPuts("1");
//	flash_app_init();
//	ADC_Init();
	UART_UartPuts("2");
	TIMER_Init();
	UART_UartPuts("3");
	Display_showStart();
	UART_UartPuts("4");
	g_disableLedKey = true;
	Led7_ReduceIntensity(true);
//	pumpControl_onVanXaInMs(5000);
	UART_UartPuts("5");
	/* Main loop */

	while(1)
	{

    	 if(g_run200usFlag == 1)
    	 {
    	 	run200usTask();
//    	 	UART_UartPuts("6");
    	 	g_run200usFlag= 0;

    	 }
    	 if(g_run1msFlag == 1)
    	 {
    	 	run1msTask();

    	 	g_run1msFlag= 0;
    	 }
    	 if(s_timeOut10ms >= 10)
    	 {
    	 	run10msTask();
    	 	s_timeOut10ms = 0;

    	 }
    	 if(s_timeOut100ms >= 100)
    	 {
    	 	run100msTask();
    	 	s_timeOut100ms = 0;

    	 }
//    	 if(g_adc_flag)
//    	 {
//    	 	ADC_UpdateTds (s_pwm_cnt);
//
//    	 }
//    	 UART_Process();
//    	 TIMER_CheckTimerEvent();
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


void FTM_1_IRQHANDLER(void)
{
	FTM_ClearStatusFlags(FTM0, kFTM_TimeOverflowFlag);
    if(0U == g_adc_flag)
	{
		uint8_t check = (cnt++) % 4;
		switch (check) {
			case 0:
				RISE_PULSE;
				g_pwm_value = 1;
				break;
			case 1:
				 ADC12_SetChannelConfig(ADC12_1_PERIPHERAL, 0U, &ADC12_1_channelsConfig[0]);
				break;
			case 2:
				FALL_PULSE;
				g_pwm_value = 0;
				break;
			case 3:
				ADC12_SetChannelConfig(ADC12_1_PERIPHERAL, 0U, &ADC12_1_channelsConfig[0]);
				break;
			default:
				break;
		}
	}
    TIMER_SystemTickEvent();
}
PUBLIC uint16_t  UART_ReadData(uint8_t * data,uint16_t maxlen)
{
	uint16_t ret = 0;
	if(data  == NULL)
	{
	 return ret;
	}

	while(!QUEUE_QueueIsEmpty(s_uart_data_queue))
	{
	    QUEUE_DeQueue(s_uart_data_queue,data+ret);
		ret ++;
		if(ret > maxlen) return ret;
	}
	s_rev_index = 0;
	s_rev_index_pre = 0;
	s_rev_done = false;
	g_rx_flag = 0U;
	return ret;
}
void LPUART_1_SERIAL_RX_TX_IRQHANDLER(void)
{
    /* If new data arrived. */
    if ((kLPUART_RxDataRegFullFlag)&LPUART_GetStatusFlags(LPUART0))
    {
    	g_rx_flag = 1U;
    		/* Set SCI1 receive buffer address and restart reception */
    		g_rx_char = LPUART_ReadByte(LPUART0);
    		QUEUE_EnQueue(s_uart_data_queue,&g_rx_char);
    	//	s_rev_index_pre = s_rev_index;
    		s_rev_index = s_rev_index+1;
    }

    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}
void ADC0_IRQHandler(void)
{
    g_adc_flag = 1U;
}
PUBLIC void UART_CheckDataReadDonePacket (void )
{
	if((s_rev_index == s_rev_index_pre) && (g_rx_flag))
	{
		s_rev_done = true;
	}
	s_rev_index_pre = s_rev_index;
}

