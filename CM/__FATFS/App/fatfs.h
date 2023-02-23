/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.h
  * @brief  Header for fatfs applications
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __fatfs_H
#define __fatfs_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "ff.h"
#include "ff_gen_drv.h"
#include "user_diskio.h" /* defines USER_Driver as external */
   
   

extern uint8_t retMemory; /* Return value for USER */
extern char MemoryPath[4]; /* USER logical drive path */
extern FATFS MemoryFatFS; /* File system object for USER logical drive */
extern FIL MemoryFile; /* File object for USER */

void MX_FATFS_Init(void);


#ifdef __cplusplus
}
#endif
#endif /*__fatfs_H */
