/**
  ******************************************************************************
  * @file    bsp_sdio_sd.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   SDIO sd卡测试驱动（不含文件系统）
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 H743 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
	
#include "main.h"	
#include "./drivers/fatfs_sd_sdio.h"
#include <stdio.h>
#include <string.h>
#include "ff_gen_drv.h"

#define SD_TIMEOUT            			((uint32_t)0x00100000U)
#define BLOCK_SIZE            			512 
#define NUMBER_OF_BLOCKS      			10
#define MULTI_BUFFER_SIZE    				(BLOCK_SIZE * NUMBER_OF_BLOCKS)


SD_HandleTypeDef uSdHandle;
HAL_SD_CardInfoTypeDef CardInfo;
uint8_t TX_Flag=0, RX_Flag=0;

/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

const Diskio_drvTypeDef  SD_Driver =
{
  SD_initialize,
  SD_status,
  SD_read, 
#if  _USE_WRITE == 1
  SD_write,
#endif /* _USE_WRITE == 1 */
  
#if  _USE_IOCTL == 1
  SD_ioctl,
#endif /* _USE_IOCTL == 1 */
};



void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd){
    TX_Flag=1;
}

void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd){   
//    SCB_InvalidateDCache_by_Addr((uint32_t*)Buffer_Block_Rx, MULTI_BUFFER_SIZE/4);
    RX_Flag=1;
}

static HAL_StatusTypeDef Wait_SDCARD_Ready(void){
	
    uint32_t loop = SD_TIMEOUT;
    
    /* Wait for the Erasing process is completed */
    /* Verify that SD card is ready to use after the Erase */
    while(loop > 0)
    {
      loop--;
      if(HAL_SD_GetCardState(&uSdHandle) == HAL_SD_CARD_TRANSFER)
      {
          return HAL_OK;
      }
    }
    return HAL_ERROR;
}

static HAL_StatusTypeDef BSP_SD_Init(void){

	HAL_StatusTypeDef sd_state = HAL_OK;
	
  uSdHandle.Instance = SDMMC2;
  uSdHandle.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  uSdHandle.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  uSdHandle.Init.BusWide = SDMMC_BUS_WIDE_4B;
  uSdHandle.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  uSdHandle.Init.ClockDiv = 4;
  uSdHandle.Init.TranceiverPresent = SDMMC_TRANSCEIVER_NOT_PRESENT;
	
	SDIO_SWITCH____uSD;
	
  if(HAL_SD_Init(&uSdHandle) != HAL_OK){
    sd_state = HAL_ERROR;
  }
	
	HAL_NVIC_SetPriority(SDMMC2_IRQn,0,0);
  HAL_NVIC_EnableIRQ(SDMMC2_IRQn);
	
	if(sd_state == HAL_OK){
		HAL_SD_GetCardInfo(&uSdHandle, &CardInfo);
		if(HAL_SD_ConfigWideBusOperation(&uSdHandle, uSdHandle.Init.BusWide) != HAL_OK){
			sd_state = HAL_ERROR;
		}
		else{
			sd_state = HAL_OK;
		}
	}
	
	return  sd_state;
}

DSTATUS SD_initialize(BYTE lun)
{
    Stat = STA_NOINIT;
    if(BSP_SD_Init() == HAL_OK)
    {    
        Stat &= ~STA_NOINIT;
    }
    return Stat;
}

DSTATUS SD_status(BYTE lun){

    Stat = STA_NOINIT;
    if(HAL_SD_GetCardState(&uSdHandle) == HAL_SD_CARD_TRANSFER)
    {
        Stat &= ~STA_NOINIT;
    }
    return Stat;
}
DRESULT SD_read(BYTE lun,//物理扇区，多个设备时用到(0...)
                BYTE *buff,//数据缓存区 
                DWORD sector, //扇区首地址
                UINT count)//扇区个数(1..128)
{
  DRESULT res = RES_ERROR;
  uint32_t timeout;
  uint32_t alignedAddr;

  RX_Flag = 0;
  
  if((DWORD)buff&3)
  {
    DRESULT res = RES_OK;
    DWORD scratch[BLOCKSIZE / 4];

    while (count--) 
    {
      res = disk_read(0,(void *)scratch, sector++, 1);

      if (res != RES_OK) 
      {
        break;
      }
      memcpy(buff, scratch, BLOCKSIZE);
      buff += BLOCKSIZE;
    }
    return res;
  }
  
  alignedAddr = (uint32_t)buff & ~0x1F;
  //更新相应的DCache
//  SCB_CleanDCache_by_Addr((uint32_t*)alignedAddr, count*BLOCKSIZE + ((uint32_t)buff - alignedAddr));
  if(HAL_SD_ReadBlocks_DMA(&uSdHandle, (uint8_t*)buff,
                           (uint32_t) (sector),
                           count) == HAL_OK)
  {
    /* Wait that the reading process is completed or a timeout occurs */
    timeout = HAL_GetTick();
    while((RX_Flag == 0) && ((HAL_GetTick() - timeout) < SD_TIMEOUT))
    {
    }
    /* incase of a timeout return error */
    if (RX_Flag == 0)
    {
      res = RES_ERROR;
    }
    else
    {
      RX_Flag = 0;
      timeout = HAL_GetTick();

      while((HAL_GetTick() - timeout) < SD_TIMEOUT)
      {
        if (HAL_SD_GetCardState(&uSdHandle) == HAL_SD_CARD_TRANSFER)
        {
          res = RES_OK;

          /*
             the SCB_InvalidateDCache_by_Addr() requires a 32-Byte aligned address,
             adjust the address and the D-Cache size to invalidate accordingly.
           */
          alignedAddr = (uint32_t)buff & ~0x1F;
          //使相应的DCache无效
          SCB_InvalidateDCache_by_Addr((uint32_t*)alignedAddr, count*BLOCKSIZE + ((uint32_t)buff - alignedAddr));

           break;
        }
      }
    }
  }

  return res;
}
  
DRESULT SD_write(BYTE lun,//物理扇区，多个设备时用到(0...)
                 const BYTE *buff,//数据缓存区 
                 DWORD sector, //扇区首地址
                 UINT count)//扇区个数(1..128)
{
    DRESULT res = RES_ERROR;
    uint32_t timeout;
  
    TX_Flag = 0;
  
    if((DWORD)buff&3)
    {
      DRESULT res = RES_OK;
      DWORD scratch[BLOCKSIZE / 4];

      while (count--) 
      {
        memcpy( scratch,buff,BLOCKSIZE);
        res = disk_write(0,(void *)scratch, sector++, 1);
        if (res != RES_OK) 
        {
          break;
        }					
        buff += BLOCKSIZE;
      }
      return res;
    }	
  
    //更新相应的DCache
//    SCB_CleanDCache_by_Addr((uint32_t*)alignedAddr, count*BLOCKSIZE + ((uint32_t)buff - alignedAddr));
    if(HAL_SD_WriteBlocks_DMA(&uSdHandle, (uint8_t*)buff,
                             (uint32_t) (sector),
                             count) == HAL_OK)
    {
      /* Wait that the reading process is completed or a timeout occurs */
      timeout = HAL_GetTick();
      while((TX_Flag == 0) && ((HAL_GetTick() - timeout) < SD_TIMEOUT))
      {
      }
      /* incase of a timeout return error */
      if (TX_Flag == 0)
      {
        res = RES_ERROR;
      }
      else
      {
        TX_Flag = 0;
        timeout = HAL_GetTick();

        while((HAL_GetTick() - timeout) < SD_TIMEOUT)
        {
          if (HAL_SD_GetCardState(&uSdHandle) == HAL_SD_CARD_TRANSFER)
          {
            res = RES_OK;
            //使相应的DCache无效
//            SCB_InvalidateDCache_by_Addr((uint32_t*)alignedAddr, count*BLOCKSIZE + ((uint32_t)buff - alignedAddr));

             break;
          }
        }
      }
    }
    return res;
}
DRESULT SD_ioctl(BYTE lun,BYTE cmd, void *buff){
    DRESULT res = RES_ERROR;
    HAL_SD_CardInfoTypeDef CardInfo;

    if (Stat & STA_NOINIT) return RES_NOTRDY;

    switch (cmd)
    {
    /* Make sure that no pending write process */
    case CTRL_SYNC :
      res = RES_OK;
      break;

    /* Get number of sectors on the disk (DWORD) */
    case GET_SECTOR_COUNT :
      HAL_SD_GetCardInfo(&uSdHandle, &CardInfo);
      *(DWORD*)buff = CardInfo.LogBlockNbr;
      res = RES_OK;
      break;

    /* Get R/W sector size (WORD) */
    case GET_SECTOR_SIZE :
      HAL_SD_GetCardInfo(&uSdHandle, &CardInfo);
      *(WORD*)buff = CardInfo.LogBlockSize;
      res = RES_OK;
      break;

    /* Get erase block size in unit of sector (DWORD) */
    case GET_BLOCK_SIZE :
      HAL_SD_GetCardInfo(&uSdHandle, &CardInfo);
      *(DWORD*)buff = CardInfo.LogBlockSize / BLOCK_SIZE;
      res = RES_OK;
      break;

    default:
      res = RES_PARERR;
    }
    return res;
}
/*****************************END OF FILE****************************/

