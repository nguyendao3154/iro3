///******************************************************************************
//*
//* Embedded software team.
//* (c) Copyright 2018.
//* ALL RIGHTS RESERVED.
//*
//***************************************************************************//*!
//*
//* @file        flash_app.c
//*
//* @author    quanvu
//*
//* @version   1.0
//*
//* @date
//*
//* @brief
//*
//*******************************************************************************
//*
//* Detailed Description of the file. If not used, remove the separator above.
//*
//******************************************************************************/
//
//
#include "flash_app.h"
#include "user_config.h"
#include "filter_time.h"
#include "filterExpire.h"
#include "CurentData.h"
//
///******************************************************************************
//* External objects
//******************************************************************************/
//
//
///******************************************************************************
//* Global variables
//******************************************************************************/
//
///******************************************************************************
//* Constants and macros
//******************************************************************************/
//
//
///******************************************************************************
//* Local types
//******************************************************************************/
//
///******************************************************************************
//* Local function prototypes
//******************************************************************************/
//
///******************************************************************************
//* Local variables
//******************************************************************************/
//
//
//
///******************************************************************************
//* Local functions
//******************************************************************************/
///**
// * @brief One line documentation
// *
// * A more detailed documentation
// *
// * @param arg1 the first function argument
// * @param arg2 the second function argument
// *
// * @return descrition for the function return value
// */
//
//
///******************************************************************************
//* Global functions
//******************************************************************************/
//
//void flash_app_eraseAll()
//{
//	flash_err_t err;
//	flash_res_t result;
//    /* Erase all of data flash */
//    err = R_FLASH_Erase(FLASH_DF_BLOCK_0, FLASH_NUM_BLOCKS_DF);
//    if (err != FLASH_SUCCESS)
//    {
//        while(1) ;
//    }
//
//    /* Verify erased */
//    err = R_FLASH_BlankCheck(FLASH_DF_BLOCK_0, FLASH_DF_FULL_SIZE, &result);
//    if ((err != FLASH_SUCCESS) || (result != FLASH_RES_BLANK))
//    {
//        while(1) ;
//    }
//}
///**
// * @brief One line documentation
// *
// * A more detailed documentation
// *
// * @param arg1 the first function argument
// * @param arg2 the second function argument
// *
// * @return descrition for the function return value
// */
void flash_app_init()
{
   
//    flash_app_eraseAll();
   user_config_init();
   filter_time_init();
   filterExpire_init();
   curentData_init();
   //    filter_time_eraseBlock(DEFAULD_BLOCK);

}
//
void flash_app_factoryReset()
{
	curentData_resetData();
////	UserConfig_resetToDefault();
////	filter_time_resetToDefault();
////	filterExpire_resetToDefault();
//
}
//void flash_app_eraseBlock(flash_block_address_t blockAdress)
//{
//	flash_err_t err;
//	flash_res_t result;
//    err = R_FLASH_Erase(blockAdress, 1);
//    if (err != FLASH_SUCCESS)
//    {
//        while(1) ;
//    }
//
//    /* Verify erased */
//    err = R_FLASH_BlankCheck(blockAdress, FLASH_DF_BLOCK_SIZE, &result);
//    if ((err != FLASH_SUCCESS) || (result != FLASH_RES_BLANK))
//    {
//        while(1) ;
//    }
//}
//
bool flash_app_writeBlock(uint8_t * data, uint32_t blockAdress,size_t dataSize)
{
	if(dataSize>= NUMBER_BYTE_VALID_MAX) return false;
	lpi2c_master_transfer_t masterXfer;
	memset(&masterXfer, 0, sizeof(masterXfer));
	masterXfer.slaveAddress   = 0x50;
	masterXfer.direction      = kLPI2C_Write;
	masterXfer.subaddress     = blockAdress;
	masterXfer.subaddressSize = 1;
	masterXfer.data           = data;
	masterXfer.dataSize       = dataSize;
	masterXfer.flags          = kLPI2C_TransferDefaultFlag;
	LPI2C_MasterTransferBlocking(LPI2C0_BASE, &masterXfer);
   return true;
}
//
bool flash_app_readData(uint8_t* dataRead, uint32_t blockAdress, size_t dataSize)
{
	if(dataSize>= NUMBER_BYTE_VALID_MAX) return false;
	lpi2c_master_transfer_t masterXfer;
	memset(&masterXfer, 0, sizeof(masterXfer));
	masterXfer.slaveAddress   = 0x50;
	masterXfer.direction      = kLPI2C_Read;
	masterXfer.subaddress     = blockAdress;
	masterXfer.subaddressSize = 1;
	masterXfer.data           = dataRead;
	masterXfer.dataSize       = dataSize;
	masterXfer.flags          = kLPI2C_TransferDefaultFlag;
	LPI2C_MasterTransferBlocking(LPI2C0_BASE, &masterXfer);
   return true;
}
///**
// * @brief One line documentation
// *
// * A more detailed documentation
// *
// * @param arg1 the first function argument
// * @param arg2 the second function argument
// *
// * @return descrition for the function return value
// */
//
