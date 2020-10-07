/******************************************************************************
*
* M1 Communication Inc.
* (c) Copyright 2016 M1 Communication, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         Config.h
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

#ifndef CONFIG_H_
#define CONFIG_H_

/******************************************************************************
* Includes
******************************************************************************/
#include "MKE16Z4.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Macros 
******************************************************************************/

#define PUBLIC 
#define LOCAL static


#define uint8  uint8_t
#define uint16 uint16_t
#define uint32 uint32_t
#define TRUE   1
#define FALSE  0
#define PNULL  0
//#define DEBUG_ENABLE
#define MAX_TRACE_LEN				(120)
#define  ARR_SIZE( _a )  ( sizeof( (_a) ) / sizeof( (_a[0]) ) )

#define VERSION_SOFTWARE ("V1.1")
#define HW_VER_214
#define FILTER_NUM_MAX 9
// define model **********************************************************************
// MODEL_LEAF , MODEL_888_TN , MODEL_888_TC , MODEL_IRO3_K92
// MODEL_OPTIMUS_I1, MODEL_OPTIMUS_I2 , MODEL_TOPBOX
#define MODEL_OPTIMUS_I2

// số lượng lõi
#define FILTER_NUM 9
// ***********************************************************************************

// TỰ ĐỘNG DEFINE THEO MODEL MÁY
// nhà sản xuất tương ứng với model máy
// KAROFI : MODEL_OPTIMUS_I1, MODEL_OPTIMUS_I2 , MODEL_TOPBOX
// KAROHOME : MODEL_LEAF , MODEL_888_TN , MODEL_888_TC , MODEL_IRO3_K92
#if defined(MODEL_OPTIMUS_I1) || defined(MODEL_OPTIMUS_I2) || defined(MODEL_TOPBOX)
#define KAROFI
#endif
#if defined(MODEL_LEAF) || defined(MODEL_888_TN) || defined(MODEL_888_TC) || defined(MODEL_IRO3_K92)
#define KAROHOME
#endif
// define có bật tds out hay không tương ứng với từng model máy
#if defined(MODEL_LEAF) || defined(MODEL_IRO3_K92) || defined(MODEL_OPTIMUS_I2) || defined(MODEL_TOPBOX)
#define ENABLE_TDS_OUT
#endif
// ***********************************************************************************

#define WAIT_TIME_UPDATE_TDS 30000 //ms

/******************************************************************************
* Types
******************************************************************************/

typedef uint8_t		BOOLEAN;

typedef void*		DPARAM;				/*!< param data pointer type */

typedef enum
{
	OK = 0,
	ERR = 1,
	ERR_PARAM =2,
	ERR_TIMEOUT = 3,
	ERR_UNKNOWN = 4,
	NOT_SUPPORT = 5,
	INPUT_FALSE = 6,

}ERR_E;

/**
 * @brief Use brief, otherwise the index won't have a brief explanation.
 *    PE43712ds_attenuation 
 * Detailed explanation.
 */




/******************************************************************************
* Global variables
******************************************************************************/
extern volatile uint32_t g_sysTime;
extern uint32_t g_sysTimeS;
   

/******************************************************************************
* Global functions
******************************************************************************/


/******************************************************************************
* Inline functions
******************************************************************************/



#endif


