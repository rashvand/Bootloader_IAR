/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */

#include "w25qxx.h"
#include "ESP_PSRAM64.h"

/* Definitions of physical drive number for each drive */
#define DEV_FLASH		0
#define DEV_PSRAM		1


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
                     BYTE pdrv		/* Physical drive nmuber to identify the drive */
                       )
{
  DSTATUS Stat = STA_NOINIT;
  
  switch (pdrv) {
    case DEV_FLASH:
      if(W25QXX_Get_State() == W25Q_OK){
        Stat &= ~STA_NOINIT;
      }
    break;
      
    case DEV_PSRAM: {
      if(PSRAM64_EID[0]==0x0D && PSRAM64_EID[1]==0x5D){
        Stat &= ~STA_NOINIT;
      }
    }
    break;
  }
  return Stat;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
                         BYTE pdrv				/* Physical drive nmuber to identify the drive */
                           )
{
  DSTATUS Stat = STA_NOINIT;
  
  switch (pdrv) {
  case DEV_FLASH:
    if(W25QXX_Init() == W25Q_OK){
      Stat &= ~STA_NOINIT;
    }
    break;;
    
  case DEV_PSRAM: 
    PSRAM_Init();
    Stat &= ~STA_NOINIT;
  break;
  }
  return Stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
                   BYTE pdrv,		/* Physical drive nmuber to identify the drive */
                   BYTE *buff,		/* Data buffer to store read data */
                   LBA_t sector,	/* Start sector in LBA */
                   UINT count		/* Number of sectors to read */
                     )
{
  DRESULT res = RES_PARERR;
  
  switch (pdrv) {
  case DEV_FLASH:
    W25QXX_Read((uint8_t*)buff, (uint32_t)sector*FF_MAX_SS, (uint16_t)count*FF_MAX_SS);
    res = RES_OK;
    break;
    
  case DEV_PSRAM: 
    PSRAM_FastRead_bytes((uint32_t)sector*FF_MIN_SS, (uint8_t*)buff, (uint16_t)count*FF_MIN_SS);
    res = RES_OK;
    break;
  }
  
  return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
                    BYTE pdrv,			/* Physical drive nmuber to identify the drive */
                    const BYTE *buff,	/* Data to be written */
                    LBA_t sector,		/* Start sector in LBA */
                    UINT count			/* Number of sectors to write */
                      )
{
  DRESULT res = RES_PARERR;
  
  switch (pdrv) {
  case DEV_FLASH:
    W25QXX_Write((uint8_t*)buff, (uint32_t)sector*FF_MAX_SS, (uint16_t)count*FF_MAX_SS);
    res = RES_OK;
    break;
    
  case DEV_PSRAM: 
    PSRAM_Write_bytes((uint32_t)sector*FF_MIN_SS, (uint8_t*)buff, (uint16_t)count*FF_MIN_SS);
    res = RES_OK;
    break;
  }
  
  return res;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
                    BYTE pdrv,		/* Physical drive nmuber (0..) */
                    BYTE cmd,		/* Control code */
                    void *buff		/* Buffer to send/receive control data */
                      )
{
  DRESULT res = RES_PARERR;
  
  switch (pdrv) {
  case DEV_FLASH :{
    
    switch (cmd){
      /* Make sure that no pending write process */
      case CTRL_SYNC: res = RES_OK; break;
        
      /* Get number of sectors on the disk (DWORD) */
      case GET_SECTOR_COUNT:
        res = RES_ERROR;
        switch(W25QXX_TYPE){
          case W25Q80 : break;
          case W25Q16 : break;
          case W25Q32 : *(DWORD*)buff = 0x400000 / FF_MAX_SS; res = RES_OK; break;
          case W25Q64 : break;
          case W25Q128 : break;
          case W25Q256 : break;
          default: res = RES_ERROR; break;
        }
      break;
      
      /* Get R/W sector size (WORD) */
      case GET_SECTOR_SIZE: *(WORD*)buff = FF_MAX_SS; res = RES_OK; break;
        
      /* Get erase block size in unit of sector (DWORD) */
      case GET_BLOCK_SIZE: *(DWORD*)buff = 1; res = RES_OK;  break;
      
      default: res = RES_PARERR; break;
    }
  }
  break;
  
  case DEV_PSRAM:{
    switch (cmd){
      /* Make sure that no pending write process */
      case CTRL_SYNC: res = RES_OK; break;
        
      /* Get number of sectors on the disk (DWORD) */
      case GET_SECTOR_COUNT: *(DWORD*)buff = 0x800000 / FF_MIN_SS; res = RES_OK; break;
      
      /* Get R/W sector size (WORD) */
      case GET_SECTOR_SIZE: *(WORD*)buff = FF_MIN_SS; res = RES_OK; break;
        
      /* Get erase block size in unit of sector (DWORD) */
      case GET_BLOCK_SIZE: *(DWORD*)buff = 1; res = RES_OK;  break;
      
      default: res = RES_PARERR; break;
    }    
  }
  break;
  }
  
  return res;
}


#if !FF_FS_NORTC && !FF_FS_READONLY
DWORD get_fattime (void)
{
  return (DWORD)0;
}
#endif

