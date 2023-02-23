/* USER CODE BEGIN Header */
/**
******************************************************************************
* @file   fatfs.c
* @brief  Code for fatfs applications
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
#include "fatfs.h"

uint8_t retUSER;    /* Return value for USER */
char MemoryPath[4];   /* USER logical drive path */
FATFS MemoryFatFS;    /* File system object for USER logical drive */
FIL MemoryFile;       /* File object for USER */

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the Memory driver ###########################*/
  retUSER = FATFS_LinkDriver(&Memory_Driver, MemoryPath);
  
}

/**
* @brief  Gets Time from RTC
* @param  None
* @retval Time in DWORD
*/
DWORD get_fattime(void)
{
  return 0;
}

