#include <stdio.h>
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKE16Z4.h"
#include "fsl_lptmr.h"
#include "display.h"
#include "timer.h"
#include "gpio.h"

volatile uint32_t g_sysTime = 0;
volatile uint8_t g_run1msFlag = 0;
volatile uint8_t g_run200usFlag = 0;
volatile uint8_t s_200usTick = 0;

volatile uint8_t g_pwm_timer;
extern volatile uint8_t g_adc_flag;
volatile uint8_t g_pwm_value = 0;
volatile uint8_t cnt = 0;

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
    __DSB();
    __ISB();
}

void FTM_1_IRQHANDLER(void)
{
    if(0U == g_adc_flag)
	{
		uint8_t check = (cnt++) % 4;
		switch (check) {
			case 0:
				RISE_PULSE;
				g_pwm_value = 1;
				break;
			case 1:
				R_Config_S12AD0_Start();
				break;
			case 2:
				FALL_PULSE;
				g_pwm_value = 0;
				break;
			case 3:
				R_Config_S12AD0_Start();
				break;
			default:
				break;
		}
	}
    TIMER_SystemTickEvent();
}
