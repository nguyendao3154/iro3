/*
 * flash_app.h
 *
 *  Created on: Jul 27, 2018
 *      Author: Admin
 */

#ifndef APPS_FLASH_FLASH_APP_H_
#define APPS_FLASH_FLASH_APP_H_

/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         flash_app.h
 *
 * @author    	quanvu
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


/******************************************************************************
* Includes
******************************************************************************/

#include <config.h>
#include "fsl_lpi2c.h"
/******************************************************************************
* Constants
******************************************************************************/
typedef enum _flash_block_address{
	FLASH_DF_BLOCK_0       = 0x00100000,    /*   1KB: 0x00100000 - 0x001003ff */
	FLASH_DF_BLOCK_1       = 0x00100400,    /*   1KB: 0x00100400 - 0x001007ff */
	FLASH_DF_BLOCK_2       = 0x00100800,    /*   1KB: 0x00100800 - 0x00100bff */
	FLASH_DF_BLOCK_3       = 0x00100C00,    /*   1KB: 0x00100C00 - 0x00100fff */
	FLASH_DF_BLOCK_4       = 0x00101000,    /*   1KB: 0x00101000 - 0x001013ff */
}flash_block_address_t;


/******************************************************************************
* Macros
******************************************************************************/
#define FILLTER_TIME_BLOCK 		FLASH_DF_BLOCK_0
#define USER_CONFIG_BLOCK 		FLASH_DF_BLOCK_1
#define TDS_PARAM_BLOCK     	FLASH_DF_BLOCK_2
#define FILLTER_EXPIRE_BLOCK 	FLASH_DF_BLOCK_3
#define CURENT_DATA_BLOCK 		FLASH_DF_BLOCK_4

#define MAX_LEN_FLASH       (1024) //1Kb
#define BYTE_SAVE_LEN      (2)
#define NUMBER_BYTE_VALID_MAX  (MAX_LEN_FLASH - BYTE_SAVE_LEN)
/******************************************************************************
* Types
******************************************************************************/
union data_len
{
	uint16_t len;
	uint8_t  len_byte[BYTE_SAVE_LEN];
};

/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/
void flash_app_init();
//void flash_app_eraseBlock(flash_block_address_t blockAdress);
bool flash_app_writeBlock(uint8_t * data, uint32_t blockAdress,size_t dataSize);
bool flash_app_readData(uint8_t* dataRead, uint32_t blockAdress, size_t dataSize);
void flash_app_factoryReset();

/******************************************************************************
* Inline functions
******************************************************************************/






#endif /* APPS_FLASH_FLASH_APP_H_ */
