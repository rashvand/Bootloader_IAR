/*
* @Author: LiuBT
* @Date: 2022-02-23 09:42:01
* @LastEditors: LiuBT
* @LastEditTime: 2022-03-18 20:42:59
* @FilePath: \MDK-ARMd:\desk\usb32\stm32_usb\Middlewares\Hardwere\inc\w25qxx.h
* @Description: 
* 
*/
#ifndef __W25QXX_H
#define __W25QXX_H
#include "stdint.h"

//////////////////////////////////////////////////////////////////////////////////	 
// This program is only for learning use, without the author's permission, it cannot be used for any other purpose
// ALIENTEK STM32F103 development board
// W25QXX driver code	   
// Punctual atom @ALIENTEK
// Technical forum: www.openedv.com
// Creation date: 2017/5/30
// Version: V1.0
//All rights reserved, piracy must be investigated.
// Copyright(C) Guangzhou Xingyi Electronic Technology Co., Ltd. 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

// W25X series/Q series chip list	   
//W25Q80  ID  0XEF13
//W25Q16  ID  0XEF14
//W25Q32  ID  0XEF15
//W25Q64  ID  0XEF16	
//W25Q128 ID  0XEF17	
//W25Q256 ID  0XEF18
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256 0XEF18



#define   W25Q_OK                        ((uint8_t)0x00)
#define   W25Q_ERROR                     ((uint8_t)0x01)

#define W25QXX_CS_LOW                    GPIOE->BSRR = (uint32_t)GPIO_PIN_4 << 16U
#define W25QXX_CS_HIGH                   GPIOE->BSRR = GPIO_PIN_4


////////////////////////////////////////////////////////////////////////////////// 
// instruction table
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg1		    0x05 
#define W25X_ReadStatusReg2		    0x35 
#define W25X_ReadStatusReg3		    0x15 
#define W25X_WriteStatusReg1      0x01 
#define W25X_WriteStatusReg2      0x31 
#define W25X_WriteStatusReg3      0x11 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F 
#define W25X_Enable4ByteAddr      0xB7
#define W25X_Exit4ByteAddr        0xE9

extern  uint16_t W25QXX_TYPE;					 // Define W25QXX chip model	

extern uint8_t W25QXX_Init(void);
uint16_t  W25QXX_ReadID ( void );  	    		 // Read FLASH ID
extern uint8_t W25QXX_Get_State(void);
uint8_t  W25QXX_Read_SR ( uint8_t regno);              // read status register
void  W25QXX_4ByteAddr_Enable ( void );      // Enable 4-byte address mode
void  W25QXX_Write_SR ( uint8_t regno, uint8_t sr);    // write status register
void  W25QXX_Write_Enable ( void );  		 // write enable
void  W25QXX_Write_Disable ( void );		 // write protection
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
extern void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);   //∂¡»°flash
extern void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);//–¥»Îflash
extern void  W25QXX_Erase_Chip ( void );    	  	 // whole chip erase
void  W25QXX_Erase_Sector ( uint32_t Dst_Addr);	 // Sector erase
void  W25QXX_Wait_Busy ( void );           	 // wait for idle
void  W25QXX_PowerDown ( void );        	 // Enter power down mode
void  W25QXX_WAKEUP ( void );				 // wake up


#endif
