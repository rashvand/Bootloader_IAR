/* USER CODE BEGIN Header */
/**
******************************************************************************
* @file    user_diskio.c
* @brief   This file includes a diskio driver skeleton to be completed by the user.
******************************************************************************
* @attention
*
* Copyright (c) 2023 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
******************************************************************************
*/
/* USER CODE END Header */

#ifdef USE_OBSOLETE_USER_CODE_SECTION_0
/*
* Warning: the user section 0 is no more in use (starting from CubeMx version 4.16.0)
* To be suppressed in the future.
* Kept to ensure backward compatibility with previous CubeMx versions when
* migrating projects.
* User code previously added there should be copied in the new user sections before
* the section contents can be deleted.
*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */
#endif

/* USER CODE BEGIN DECL */

/* Includes ------------------------------------------------------------------*/
#include "w25qxx.h"
#include "ESP_PSRAM64.h"
#include <string.h>
#include "ff_gen_drv.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

/* USER CODE END DECL */

/* Private function prototypes -----------------------------------------------*/
DSTATUS Memory_initialize (BYTE pdrv);
DSTATUS Memory_status (BYTE pdrv);
DRESULT Memory_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
#if _USE_WRITE == 1
DRESULT Memory_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
DRESULT Memory_ioctl (BYTE pdrv, BYTE cmd, void *buff);
#endif /* _USE_IOCTL == 1 */

Diskio_drvTypeDef  Memory_Driver =
{
  Memory_initialize,
  Memory_status,
  Memory_read,
#if  _USE_WRITE
  Memory_write,
#endif  /* _USE_WRITE == 1 */
#if  _USE_IOCTL == 1
  Memory_ioctl,
#endif /* _USE_IOCTL == 1 */
};



/* Private functions ---------------------------------------------------------*/

/**
* @brief  Initializes a Drive
* @param  pdrv: Physical drive number (0..)
* @retval DSTATUS: Operation status
*/
DSTATUS Memory_initialize (
                         BYTE pdrv           /* Physical drive nmuber to identify the drive */
                           )
{
  Stat = STA_NOINIT;
  if(W25QXX_Init() == W25Q_OK){
    Stat &= ~STA_NOINIT;
  }
  return Stat;
}

/**
* @brief  Gets Disk Status
* @param  pdrv: Physical drive number (0..)
* @retval DSTATUS: Operation status
*/
DSTATUS Memory_status (
                     BYTE pdrv       /* Physical drive number to identify the drive */
                       )
{
  Stat = STA_NOINIT;
  if(W25QXX_Get_State() == W25Q_OK){
    Stat &= ~STA_NOINIT;
  }
  return Stat;
}

/**
* @brief  Reads Sector(s)
* @param  pdrv: Physical drive number (0..)
* @param  *buff: Data buffer to store read data
* @param  sector: Sector address (LBA)
* @param  count: Number of sectors to read (1..128)
* @retval DRESULT: Operation result
*/
DRESULT Memory_read (
                   BYTE pdrv,      /* Physical drive nmuber to identify the drive */
                   BYTE *buff,     /* Data buffer to store read data */
                   DWORD sector,   /* Sector address in LBA */
                   UINT count      /* Number of sectors to read */
                     )
{
  W25QXX_Read((uint8_t*)buff, (uint32_t)sector*_MIN_SS, (uint16_t)count*_MIN_SS);
  return RES_OK;
}

/**
* @brief  Writes Sector(s)
* @param  pdrv: Physical drive number (0..)
* @param  *buff: Data to be written
* @param  sector: Sector address (LBA)
* @param  count: Number of sectors to write (1..128)
* @retval DRESULT: Operation result
*/
#if _USE_WRITE == 1
DRESULT Memory_write (
                    BYTE pdrv,          /* Physical drive nmuber to identify the drive */
                    const BYTE *buff,   /* Data to be written */
                    DWORD sector,       /* Sector address in LBA */
                    UINT count          /* Number of sectors to write */
                      )
{

  W25QXX_Write((uint8_t*)buff, (uint32_t)sector*_MIN_SS, (uint16_t)count*_MIN_SS);
  return RES_OK;
}
#endif /* _USE_WRITE == 1 */

/**
* @brief  I/O control operation
* @param  pdrv: Physical drive number (0..)
* @param  cmd: Control code
* @param  *buff: Buffer to send/receive control data
* @retval DRESULT: Operation result
*/
#if _USE_IOCTL == 1
DRESULT Memory_ioctl (
                    BYTE pdrv,      /* Physical drive nmuber (0..) */
                    BYTE cmd,       /* Control code */
                    void *buff      /* Buffer to send/receive control data */
                      )
{
  DRESULT res = RES_ERROR;
  
  switch (cmd){
    /* Make sure that no pending write process */
  case CTRL_SYNC :
    res = RES_OK;
    break;
    
    /* Get number of sectors on the disk (DWORD) */
  case GET_SECTOR_COUNT :
    res = RES_OK;
    switch(W25QXX_TYPE)
    {
    case W25Q80 :
      {

        break;
      }
    case W25Q16 :
      {

        break;
      }
    case W25Q32 :
      {
        *(DWORD*)buff = 0x400000 / _MIN_SS;
        break;
      }
    case W25Q64 :
      {

        break;
      }
    case W25Q128 :
      {

        break;
      }
    case W25Q256 :
      {


        break;
      }
    default:

      res = RES_ERROR;
      break;
    }
    
    break;
    
    /* Get R/W sector size (WORD) */
  case GET_SECTOR_SIZE :
    *(WORD*)buff = _MIN_SS;
    res = RES_OK;
    break;
    
    /* Get erase block size in unit of sector (DWORD) */
  case GET_BLOCK_SIZE :
    *(DWORD*)buff = 1;
    res = RES_OK;
    break;
    
  default:
    res = RES_PARERR;
  }
  
  return res;
}
#endif /* _USE_IOCTL == 1 */
