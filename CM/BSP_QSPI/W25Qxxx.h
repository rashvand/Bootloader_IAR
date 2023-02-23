/**
  ******************************************************************************
  * @file    n25q128a.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2015
  * @brief   This file contains all the description of the N25Q128A QSPI memory.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __W25QXXX_H
#define __W25QXXX_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @addtogroup W25Qxxx
  * @{
  */

/** @defgroup W25Qxxx_Exported_Types
  * @{
  */
   
/**
  * @}
  */ 

/** @defgroup W25Qxxx_Exported_Constants
  * @{
  */
   
/** 
  * @brief  W25Qxxx Configuration  
  */  
   
   
#ifdef W25Q256JV   
  #define W25Qxxx_FLASH_SIZE                  0x2000000  /* 256 MBits => 32MBytes */
  #define W25Qxxx_SECTOR_SIZE                 0x10000    /* 4096 sectors of 64KBytes */
  #define W25Qxxx_SUBSECTOR_SIZE              0x1000     /* 65536 subsectors of 4kBytes */
  #define W25Qxxx_PAGE_SIZE                   0x100      /* 1048576 pages of 256 bytes */

  #define W25Qxxx_DUMMY_CYCLES_READ           4
  #define W25Qxxx_DUMMY_CYCLES_READ_QUAD      10

  #define W25Qxxx_BULK_ERASE_MAX_TIME         250000
  #define W25Qxxx_SECTOR_ERASE_MAX_TIME       3000
  #define W25Qxxx_SUBSECTOR_ERASE_MAX_TIME    800
#endif
    
#ifdef W25Q64JV   
  #define W25Q64JV_FLASH_SIZE                  0x800000  /* 64 MBits => 8MBytes */
  #define W25Q64JV_SECTOR_SIZE                 0x10000   /* 128 sectors of 64KBytes */
  #define W25Q64JV_SUBSECTOR_SIZE              0x1000    /* 2048 subsectors of 4kBytes */
  #define W25Q64JV_PAGE_SIZE                   0x100     /* 32768 pages of 256 bytes */

  #define W25Q64JV_DUMMY_CYCLES_READ           4
  #define W25Q64JV_DUMMY_CYCLES_READ_QUAD      10

  #define W25Q64JV_BULK_ERASE_MAX_TIME         250000
  #define W25Q64JV_SECTOR_ERASE_MAX_TIME       3000
  #define W25Q64JV_SUBSECTOR_ERASE_MAX_TIME    800    
#endif

#ifdef W25Q32JV
  #define W25Qxxx_FLASH_SIZE                  0x400000  /* 32 MBits => 4MBytes */
  #define W25Qxxx_SECTOR_SIZE                 0x10000    /* 4096 sectors of 64KBytes */
  #define W25Qxxx_SUBSECTOR_SIZE              0x1000     /* 65536 subsectors of 4kBytes */
  #define W25Qxxx_PAGE_SIZE                   0x100      /* 1048576 pages of 256 bytes */

  #define W25Qxxx_DUMMY_CYCLES_READ           4
  #define W25Qxxx_DUMMY_CYCLES_READ_QUAD      10

  #define W25Qxxx_BULK_ERASE_MAX_TIME         250000
  #define W25Qxxx_SECTOR_ERASE_MAX_TIME       3000
  #define W25Qxxx_SUBSECTOR_ERASE_MAX_TIME    800
#endif
   
   
   
   

/** 
  * @brief  W25Qxxx Commands  
  */  
/* Reset Operations */
#define RESET_ENABLE_CMD                     0x66
#define RESET_MEMORY_CMD                     0x99

#define ENTER_QPI_MODE_CMD                   0x38
#define EXIT_QPI_MODE_CMD                    0xFF

/* Identification Operations */
#define READ_ID_CMD                          0x90
#define DUAL_READ_ID_CMD                     0x92
#define QUAD_READ_ID_CMD                     0x94
#define READ_JEDEC_ID_CMD                    0x9F

/* Read Operations */
#define READ_CMD                             0x03
#define FAST_READ_CMD                        0x0B
#define DUAL_OUT_FAST_READ_CMD               0x3B
#define DUAL_INOUT_FAST_READ_CMD             0xBB
#define QUAD_OUT_FAST_READ_CMD               0x6B
#define QUAD_INOUT_FAST_READ_CMD             0xEB

/* Write Operations */
#define WRITE_ENABLE_CMD                     0x06
#define WRITE_DISABLE_CMD                    0x04

/* Register Operations */
#define READ_STATUS_REG1_CMD                  0x05
#define READ_STATUS_REG2_CMD                  0x35
#define READ_STATUS_REG3_CMD                  0x15

#define WRITE_STATUS_REG1_CMD                 0x01
#define WRITE_STATUS_REG2_CMD                 0x31
#define WRITE_STATUS_REG3_CMD                 0x11


/* Program Operations */
#define PAGE_PROG_CMD                        0x02
#define QUAD_INPUT_PAGE_PROG_CMD             0x32


/* Erase Operations */
#define SECTOR_4KB_ERASE_CMD                 0x20
#define BLOCK_32KB_ERASE_CMD                 0x52
#define BLOCK_64KB_ERASE_CMD                 0xD8
#define CHIP_ERASE_CMD                       0xC7

#define PROG_ERASE_RESUME_CMD                0x7A
#define PROG_ERASE_SUSPEND_CMD               0x75


/* Flag Status Register */
#define W25Qxxx_FSR_BUSY                    ((uint8_t)0x01)    /*!< busy */
#define W25Qxxx_FSR_WREN                    ((uint8_t)0x02)    /*!< write enable */
#define W25Qxxx_FSR_QE                      ((uint8_t)0x02)    /*!< quad enable */

/**
  * @}
  */
  
/** @defgroup W25Qxxx_Exported_Functions
  * @{
  */ 
/**
  * @}
  */ 
/* ��ȷ�� */
#define READ_VOL_CFG_REG_CMD                 0x85
#define WRITE_VOL_CFG_REG_CMD                0x81
/* 4-byte Address Mode Operations */
#define ENTER_4_BYTE_ADDR_MODE_CMD           ENTER_QPI_MODE_CMD
#define EXIT_4_BYTE_ADDR_MODE_CMD            EXIT_QPI_MODE_CMD
      
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /*  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
