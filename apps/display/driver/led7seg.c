/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        display.c
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


#include "led7seg.h"
#include "adc.h"
#include "filter_time.h"
#include "timeCheck.h"
#include "fsl_gpio.h"

/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define NUM_OF_LED 5
#define REDUCE_INTENSITY_CNT 10
const uint8_t LED7_CODE_START[] = {0xFE,};

const uint8_t LED7_CODE[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0x86};
GPIO_Type * LED7_PORT[] = {GPIOD_BASE,GPIOC_BASE,GPIOB_BASE,GPIOB_BASE,GPIOE_BASE,GPIOC_BASE,GPIOB_BASE,GPIOE_BASE};
const uint32_t LED7_PIN[] = {7U,3U,5U,6U,8U,2U,4U,3U};
const uint32_t LED7_DIGITS[] = {3U,0U,1U,5U,7U};
GPIO_Type * LED7_PORT_DIGITS[] = {GPIOB_BASE,GPIOC_BASE,GPIOC_BASE,GPIOD_BASE,GPIOB_BASE};


/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/
void showDigitAtIndex(uint8_t digit,uint8_t index);
void processBuzzer();
void encodeLed4(uint16_t number);
void encodeCurrentLed4();


/******************************************************************************
* Local variables
******************************************************************************/
uint8_t s_led4Digits[4] = {0xff,0xff,0xff,0xff};
uint16_t s_led4Number;
uint16_t s_led1Number;
uint8_t s_led1Code = 0xff;
uint8_t s_ledIndex;
bool s_led7IsOn = true;
uint8_t s_indexCnt = NUM_OF_LED;
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

void encodeLed4(uint16_t number)
{
	for(int i = 0; i<4; i++)
	{
		s_led4Digits[i] = LED7_CODE[number%10];
		number /= 10;
	}
	if(s_led4Digits[3] == LED7_CODE[0])
	{
		s_led4Digits[3] = LED7_CODE[LED_7SEG_OFF];
		if(s_led4Digits[2] == LED7_CODE[0])
		{
			s_led4Digits[2] = LED7_CODE[LED_7SEG_OFF];
			if(s_led4Digits[1] == LED7_CODE[0])
			{
				s_led4Digits[1] = LED7_CODE[LED_7SEG_OFF];
			}
		}
	}
}

void encodeCurrentLed4()
{
	if(s_led7IsOn)
		encodeLed4(s_led4Number);
}
void showDigitAtIndex(uint8_t digit,uint8_t index)
{
	for(int i = 0; i<5; i++)
	{
		GPIO_PinWrite(LED7_PORT_DIGITS[i], LED7_DIGITS[i], 1);
	}
	for(int i = 0; i<8;i++)
	{
		GPIO_PinWrite(LED7_PORT[i], LED7_PIN[i], (digit & (1<<i)));
	}
	GPIO_PinWrite(LED7_PORT_DIGITS[index], LED7_DIGITS[index], 0);
}
void turnOffAllLed()
{
	for(int i = 0; i<5; i++)
	{
		GPIO_PinWrite(LED7_PORT_DIGITS[i], LED7_DIGITS[i], 1);
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
void Led7seg_scanLed(void)
{
	uint8_t digit = 0;
	if(s_ledIndex < 4)
	{
		showDigitAtIndex(s_led4Digits[s_ledIndex],s_ledIndex);
	}else if(s_ledIndex == 4){
		digit = s_led1Code;
		showDigitAtIndex(digit,s_ledIndex);
	}else{
		turnOffAllLed();
	}
	s_ledIndex++;
	if(s_ledIndex >= s_indexCnt)
	{
		s_ledIndex = 0;
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
void Led7seg_SetNumberInLed1(int8_t number)
{
	s_led1Number = number;
	if(s_led7IsOn)
		s_led1Code = LED7_CODE[number];
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
void Led7seg_SetNumberInLed4(uint16_t number)
{
	s_led4Number = number;
	encodeCurrentLed4();
}

uint16_t Led7seg_getNumberInLed4()
{
	return s_led4Number;
}

void Led7seg_reduceNumberInLed4(uint16_t reduce)
{
	if(s_led4Number >= reduce)
		s_led4Number -= reduce;
	else
		s_led4Number = 0;
	encodeCurrentLed4();
}

void Led7seg_increaseNumberInLed4(uint16_t reduce)
{
	s_led4Number += reduce;
	if(s_led4Number > 9999)
		s_led4Number = 9999;
	encodeCurrentLed4();
}

void Led7seg_turnOnLed()
{
	encodeCurrentLed4();
	s_led1Code = LED7_CODE[s_led1Number];
	s_led7IsOn = true;
}

void Led7seg_turnOffLed()
{
	s_led7IsOn = false;
	for(uint8_t i = 0; i<4; i++)
	{
		s_led4Digits[i] = LED7_CODE[LED_7SEG_OFF];
	}
	s_led1Code = LED7_CODE[LED_7SEG_OFF];
}

// for start up
void Led7seg_setAllLedCode(uint8_t ledCode)
{
	for(int i = 0; i<4; i++)
	{
		s_led4Digits[i] = ledCode;
	}
	s_led1Code = ledCode;
}

void Led7seg_OnAllSegInLed(uint8_t ledIndex)
{
	for(uint8_t i = 0; i<4; i++)
	{
		s_led4Digits[i] = LED7_CODE[LED_7SEG_OFF];
	}
	s_led1Code = LED7_CODE[LED_7SEG_OFF];
	if(ledIndex < 4)
	{
		s_led4Digits[ledIndex] = 0x00;
	}else{
		s_led1Code = 0x00;
	}
}
void Led7_ReduceIntensity(bool isOn)
{
	if(isOn)
		s_indexCnt = REDUCE_INTENSITY_CNT;
	else
		s_indexCnt = NUM_OF_LED;
}
// end code for start
