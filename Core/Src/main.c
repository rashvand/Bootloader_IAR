
#include "main.h"
#include "ff.h"
#include "diskio.h"
#include "w25qxx.h"
#include "ESP_PSRAM64.h"
#include "cJSON.h"


QSPI_HandleTypeDef hqspi        = {0};
SPI_HandleTypeDef hspi2         = {0};
QSPI_Info QSPI_info 	        = {0};


FATFS FlashFatFs, PSRamFatFs;   /* File system object for each logical drive */
FIL   FlashFile, PSRamFile;     /* File objects */
DIR   FlashDir, PSRamDir;       /* Directory object */
uint32_t FS_read_byte, FS_write_byte;


#define EnablePrivilegedMode() __asm("SVC #0")

bool FLASH_EXECUTE____FLAG = false;

typedef  void (*pFunction)(void);
pFunction JumpToApplication;

extern const uint32_t app_vector;   // Application vector address symbol from 
                                    // the linker configuration file

void SystemClock_Config(void);
static void MPU_RegionConfig(void);
static void MX_GPIO_Init(void);
static void MX_QUADSPI_Init(void);
static void MX_SPI2_Init(void);

struct _Boot{
  char product[20];
  char releasedate[20];
  char bootname[20];
  uint32_t bootver;
  uint32_t installedver;
  bool forceupdate;
}Boot;

int main(void){
  
  QSPI_CommandTypeDef s_command = {0};
  QSPI_MemoryMappedTypeDef sMemMappedCfg = {0};
  FRESULT fr;
  
  MPU_RegionConfig();
  
  
  SCB_EnableICache();
  SCB_EnableDCache();
  
  HAL_Init();
  
  SystemClock_Config();
  
  MX_GPIO_Init();
  MX_QUADSPI_Init();
  MX_SPI2_Init();
  
  if(BSP_QSPI_Init() != QSPI_OK)  {
    Error_Handler();
  }
  
  BSP_QSPI_GetInfo(&QSPI_info);
 
#if(0)
  if(BSP_QSPI_Erase_Chip() == QSPI_OK){ // ~37 sec
    printf("QSPI Full erased\n");
  }
#endif
  
  
#if(0) // QSPI FW Flash Full Erase for first time boot
  if(BSP_QSPI_Erase_Chip() == QSPI_OK){
    FLASH_EXECUTE____FLAG = true;
  }
  else  {
    Error_Handler();
  }
#endif
 
  
#if(0) // Only First boot
  W25QXX_Erase_Chip();
  MKFS_PARM mkfs_parm_Flash = { .fmt = FM_ANY, .n_fat = 2, .align = 1 };
  fr = f_mkfs("/flash", &mkfs_parm_Flash, NULL, FF_MAX_SS);
  if (fr == FR_OK){
    printf("Flash f_mkfs Ok!\r\n");
  }
  else{
    printf("Flash f_mkfs Error, error code:%d.\r\n",fr);
    while(1);
  }
#endif
  
  //---------- S: Mount Flash and PSRAM ----------
  // Flash
  fr = f_mount(&FlashFatFs, "/flash", 0);
  if(fr == FR_OK){
    printf("Flash mount ok!\r\n");
  }
  else{
    printf("Flash mount error, error code:%d.\r\n",fr);
    while(1);
  }
  
  
  // PSRAM
  fr = f_mount(&PSRamFatFs, "/psram", 0);
  if(fr == FR_OK){
    printf("PSRAM mount ok!\r\n");
  }
  else{
    printf("PSRAM mount error, error code:%d.\r\n",fr);
    while(1);
  }
  
  MKFS_PARM mkfs_parm_PSRAM = { .fmt = FM_FAT, .n_fat = 2, .align = 1 };
  fr = f_mkfs("/psram", &mkfs_parm_PSRAM, NULL, FF_MIN_SS);
  if (fr == FR_OK){
    printf("PSRAM f_mkfs Ok!\r\n");
  }
  else{
    printf("PSRAMf_mkfs Error, error code:%d.\r\n",fr);
    while(1);
  }
  //---------- E: Mount Flash and PSRAM ----------
  
  //----------- S: Boot Firmware -----------
#if(0) // only for first boot time
  // boot.json
  fr = f_open(&FlashFile, "/flash/boot.json", FA_CREATE_ALWAYS | FA_WRITE);
  if(fr == FR_OK){
    printf("open file ok! \r\n");
        
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "product", "Modem");
    cJSON_AddStringToObject(root, "releasedate", "2023/02/14-10:12");
    cJSON_AddStringToObject(root, "bootname", "boot.bin");
    cJSON_AddNumberToObject(root, "bootver", 835010200);
    cJSON_AddNumberToObject(root, "installedver", 835010200);
    cJSON_AddFalseToObject (root, "forceupdate");
    
    char *json_str = cJSON_Print(root);

    printf("%d\n", strlen(json_str));
    printf("%s\n", json_str);        
    
    FS_write_byte = 0;
    fr = f_write(&FlashFile, json_str, strlen(json_str), (void *)&FS_write_byte);
    if(fr == FR_OK){
      printf("write %d data to file file.\r\n", FS_write_byte);
      f_close(&FlashFile);
    }
    else{
      printf("write error,error code: %d\r\n", fr);
      while(1);
    }
    cJSON_Delete(root);
    free(json_str);

  }
  else{
    printf("open file error: %d\r\n", fr);
    while(1);
  }
  
  // config.json
  fr = f_open(&FlashFile, "/flash/config.json", FA_CREATE_ALWAYS | FA_WRITE);
  if(fr == FR_OK){
    printf("open file ok! \r\n");
    f_close(&FlashFile);
  }
  else{
    printf("open file error: %d\r\n", fr);
  }
  
  // boot.bin
  fr = f_open(&FlashFile, "/flash/boot.bin", FA_CREATE_ALWAYS | FA_WRITE);
  if(fr == FR_OK){
    printf("open file ok! \r\n");
    f_close(&FlashFile);
  }
  else{
    printf("open file error: %d\r\n", fr);
  }
#endif  
  
  fr = f_open(&FlashFile, "/flash/boot.json", FA_READ);
  if(fr == FR_OK){
    printf("open file ok! \r\n");    
    uint32_t json_size = f_size(&FlashFile);
    
    char *json_buffer = (char*) malloc(json_size);
    if(json_buffer == NULL){
      f_close(&FlashFile);
      printf("Error in memory allocating file\n");
      while(1);
    }
    else{
      FS_read_byte = 0;
      f_read(&FlashFile, json_buffer, json_size, &FS_read_byte);
      
      printf(json_buffer); printf("\n");
          
      cJSON *root = cJSON_ParseWithLength(json_buffer, FS_read_byte);
      if(root == NULL){
        printf("Error in reading json file\n");
        while(1);
      }
      
      strcpy(Boot.product, cJSON_GetObjectItem(root, "product")->valuestring);
      strcpy(Boot.releasedate, cJSON_GetObjectItem(root, "releasedate")->valuestring);
      strcpy(Boot.bootname, cJSON_GetObjectItem(root, "bootname")->valuestring);
      Boot.bootver = (uint32_t)(cJSON_GetObjectItem(root, "bootver")->valueint);
      Boot.installedver = (uint32_t)(cJSON_GetObjectItem(root, "installedver")->valueint);
      Boot.forceupdate = (uint32_t)(cJSON_GetObjectItem(root, "forceupdate")->valueint);
      
      printf("product: %s\r\n", Boot.product);
      printf("releasedate: %s\r\n", Boot.releasedate);
      printf("bootname: %s\r\n", Boot.bootname);
      printf("bootver: %d\r\n", Boot.bootver);
      printf("installedver: %d\r\n", Boot.installedver);
      printf("forceupdate: %d\r\n", Boot.forceupdate);
      
      // Cleanup
      f_close(&FlashFile);
      cJSON_Delete(root);
      free(json_buffer);
    }
  }
  else{
    printf("open file error: %d\r\n", fr);
  }
  //----------- E: Boot Firmware -----------
  
  while (1){
    if(Boot.bootver == Boot.installedver && Boot.forceupdate == false){
      FLASH_EXECUTE____FLAG = true;
    }
    else if(Boot.bootver != Boot.installedver || Boot.forceupdate == true){
      char bootpath[50];
      sprintf(bootpath,"/flash/%s", Boot.bootname);
      bool ErrorInProgress = false;
      
      fr = f_open(&FlashFile, bootpath, FA_READ);
      if(fr == FR_OK){
        printf("open file ok! \r\n");    
        //uint32_t boot_size = f_size(&FlashFile);
      
        uint8_t *DataPacket = (uint8_t *) malloc(4096U);
        uint8_t *DataPacket____Temp = (uint8_t *) malloc(4096U);
        if(DataPacket != NULL && DataPacket____Temp != NULL){
          FS_read_byte = 0;
          uint32_t DataPacket____Counter = 0U;
          for(;;){
            f_read(&FlashFile, DataPacket, 4096, &FS_read_byte);
            if(FS_read_byte == 0) 
              break; 
            
            if(BSP_QSPI_Erase_Sector_4KB(DataPacket____Counter) == QSPI_OK){
              if(BSP_QSPI_Write(DataPacket, DataPacket____Counter, FS_read_byte) == QSPI_OK){
                BSP_QSPI_Read(DataPacket____Temp, DataPacket____Counter, FS_read_byte);
                if(memcmp(DataPacket, DataPacket____Temp, FS_read_byte) == 0U){
                  DataPacket____Counter += 4096U;
                }
                else{
                  ErrorInProgress = true;
                  break;
                }
              }
              else{
                ErrorInProgress = true;
                break;
              }
            }
            else{
              ErrorInProgress = true;
              break;
            }
          }
        }
        else{
          ErrorInProgress = true;
        }
        free(DataPacket);
        free(DataPacket____Temp);
      }
      else{
        printf("open file error: %d\r\n", fr);
      }
      
      if(ErrorInProgress == false){
        fr = f_open(&FlashFile, "/flash/boot.json", FA_READ | FA_WRITE);
        if(fr == FR_OK){
          printf("open file ok! \r\n");    
          uint32_t json_size = f_size(&FlashFile);
        
          char *json_string = (char*) malloc(json_size);
          if(json_string == NULL){
            
          }
          else{
            FS_read_byte = 0;
            f_read(&FlashFile, json_string, json_size, &FS_read_byte);
                          
            cJSON *root = cJSON_ParseWithLength(json_string, FS_read_byte);
            if(root == NULL){
              printf("Error in reading json file\n");
            }
            else{
              cJSON* boot_installedver = cJSON_GetObjectItem(root, "installedver");
              if(boot_installedver != NULL){
                cJSON_SetNumberValue(boot_installedver, Boot.bootver);
                
                cJSON* boot_forceupdate = cJSON_GetObjectItem(root, "forceupdate");
                if(boot_forceupdate != NULL){
                  cJSON_SetBoolValue(boot_forceupdate, false);
                  
                  char *json_str = cJSON_Print(root);
                  
                  FS_write_byte = 0;
                  fr = f_write(&FlashFile, json_str, strlen(json_str), (void *)&FS_write_byte);
                  if(fr == FR_OK){
                    printf("write %d data to file file.\r\n", FS_write_byte);
                    FLASH_EXECUTE____FLAG = true;
                  }
                  else{
                    printf("write error,error code: %d\r\n", fr);
                    while(1);
                  }
                  cJSON_Delete(boot_forceupdate);
                }
                cJSON_Delete(boot_installedver);
              }
              cJSON_Delete(root);
            }
            f_close(&FlashFile);
            free(json_string);
          }
        }
        else{
          printf("unable open /flash/boot.json file\n");
        }
      }
    }
    
    if(FLASH_EXECUTE____FLAG == true){
      FLASH_EXECUTE____FLAG = false;
      
      sMemMappedCfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
      
      s_command.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;
      s_command.Instruction       	= QUAD_INOUT_FAST_READ_CMD;
      s_command.AddressMode       	= QSPI_ADDRESS_4_LINES;
      s_command.Address           	= 0;
      s_command.AddressSize       	= QSPI_ADDRESS_24_BITS;
      s_command.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_4_LINES;
      s_command.AlternateBytes 		= 0x00;
      s_command.AlternateBytesSize 	= QSPI_ALTERNATE_BYTES_8_BITS;
      s_command.DataMode          	= QSPI_DATA_4_LINES;
      s_command.DummyCycles       	= 4;
      s_command.NbData            	= 0;
      s_command.DdrMode           	= QSPI_DDR_MODE_DISABLE;
      s_command.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;
      s_command.SIOOMode          	= QSPI_SIOO_INST_EVERY_CMD;
      //s_command.SIOOMode          = QSPI_SIOO_INST_ONLY_FIRST_CMD;
      
      
      if(HAL_QSPI_MemoryMapped(&hqspi, &s_command, &sMemMappedCfg) == HAL_OK){
        
        __disable_irq();
        
        // 1. Make sure, the CPU is in privileged mode.
        if( CONTROL_nPRIV_Msk & __get_CONTROL() ){  
          /* not in privileged mode */
          EnablePrivilegedMode();
        }
        
        // 2. Disable all enabled interrupts in NVIC.
        NVIC->ICER[0] = 0xFFFFFFFF;
        NVIC->ICER[1] = 0xFFFFFFFF;
        NVIC->ICER[2] = 0xFFFFFFFF;
        NVIC->ICER[3] = 0xFFFFFFFF;
        NVIC->ICER[4] = 0xFFFFFFFF;
        NVIC->ICER[5] = 0xFFFFFFFF;
        NVIC->ICER[6] = 0xFFFFFFFF;
        NVIC->ICER[7] = 0xFFFFFFFF;
        
        HAL_MspDeInit();
        
        SCB_DisableICache();
        SCB_DisableDCache(); SCB_CleanDCache();	
        
        HAL_RCC_DeInit();
        
        // 4. Clear all pending interrupt requests in NVIC.
        NVIC->ICPR[0] = 0xFFFFFFFF;
        NVIC->ICPR[1] = 0xFFFFFFFF;
        NVIC->ICPR[2] = 0xFFFFFFFF;
        NVIC->ICPR[3] = 0xFFFFFFFF;
        NVIC->ICPR[4] = 0xFFFFFFFF;
        NVIC->ICPR[5] = 0xFFFFFFFF;
        NVIC->ICPR[6] = 0xFFFFFFFF;
        NVIC->ICPR[7] = 0xFFFFFFFF;
        
        // 5. Disable SysTick and clear its exception pending bit, if it is used in the bootloader, e. g. by the RTX.
        SysTick->CTRL = 0;
        SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk;
        
        // 6. Disable individual fault handlers if the bootloader used them.
        SCB->SHCSR &= ~( SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk );
        
        // 7. Activate the MSP, if the core is found to currently run with the PSP.
        if( CONTROL_SPSEL_Msk & __get_CONTROL() ){
          /* MSP is not active */
          __set_CONTROL( __get_CONTROL() & ~CONTROL_SPSEL_Msk );
        }
        
        // 8. Load the vector table address of the user application into SCB->VTOR register. Make sure the address meets the alignment requirements.
        SCB->VTOR = QSPI_BASE;
        
        // 9. Set the MSP to the value found in the user application vector table.
        __set_MSP((*(__IO uint32_t*)QSPI_BASE));
        //__set_SP(vector_p->stack_addr);
        
        // 10. Set the PC to the reset vector value of the user application via a function call.
        JumpToApplication = (pFunction)(*(__IO uint32_t*)(QSPI_BASE+4));
        JumpToApplication();
      }
      else{
        
      }
    }
  }
}

void SystemClock_Config(void){
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  
  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 10;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
    |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
      |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_QUADSPI_Init(void){
  
  HAL_QSPI_DeInit(&hqspi);
  
  /* ClockPrescaler:  		QPSI frequency division ratio, the maximum frequency of W25Q32 is 133MHz in 3.0V-3.6V, here 240/(1+1)=120MHz */
  /* FifoThreshold : 		FIFO depth is set to 32 bytes */
  /* SampleShifting : 		Configure QSPI to sample Flash drive data after half a CLK cycle after the Flash drive signal */
  /* FlashSize : 		21+1=23, 24-bit addressing, where 24-bit addressing is the byte search address (W25Q32 is 4M bytes) */
  /* ChipSelectHighTime :       The chip select high level time is 8 clocks (12.5*8=100ns), which is the tSHSL parameter in the manual */
  
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 2; 																
  hqspi.Init.FifoThreshold = 32; 																
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE; 

#ifdef W25Q32JV
  hqspi.Init.FlashSize = 21; 		
#elif  W25Q64JV
  hqspi.Init.FlashSize = 22; 
#elif W25Q256JV
  hqspi.Init.FlashSize = 25; 
#endif
  
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_8_CYCLE; 		
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  hqspi.Init.FlashID = QSPI_FLASH_ID_1;
  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK){
    Error_Handler();
  }
  
}

static void MX_SPI2_Init(void){
  
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 0x0;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi2.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi2.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi2.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi2.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi2.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi2.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi2.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi2.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;
  hspi2.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  
  MODIFY_REG(SPI2->CR2, SPI_CR2_TSIZE, 0);
  LL_SPI_Enable(SPI2);
}
static void MX_GPIO_Init(void){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4 | GPIO_PIN_6, GPIO_PIN_SET);
  
  /*Configure GPIO pin : SPI2_CS_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}
void MPU_RegionConfig(void){
  MPU_Region_InitTypeDef MPU_InitStruct = {0};
  
  /* Disables the MPU */
  HAL_MPU_Disable();
  
  MPU_InitStruct.Enable                                 = MPU_REGION_ENABLE;
  MPU_InitStruct.Number                                 = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress                            = 0x0;
  MPU_InitStruct.Size                                   = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable                       = 0x87;
  MPU_InitStruct.TypeExtField                           = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission                       = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsShareable                            = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable                            = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable                           = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.DisableExec                            = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  
  /* ITCMRAM 64KB */
  MPU_InitStruct.Enable 	  			= MPU_REGION_ENABLE;
  MPU_InitStruct.Number 	  			= MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress 	  	                = 0x00000000;
  MPU_InitStruct.Size 		  			= MPU_REGION_SIZE_64KB;
  MPU_InitStruct.SubRegionDisable                       = 0x0;
  MPU_InitStruct.TypeExtField 	                        = MPU_TEX_LEVEL1;
  MPU_InitStruct.AccessPermission                       = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsShareable 	  	                = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable 	  	                = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable 	                        = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.DisableExec 	  	                = MPU_INSTRUCTION_ACCESS_ENABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  
  /* DTCMRAM 128KB */
  MPU_InitStruct.Enable 	  			= MPU_REGION_ENABLE;
  MPU_InitStruct.Number 	  			= MPU_REGION_NUMBER2;
  MPU_InitStruct.BaseAddress 	  	                = 0x20000000;
  MPU_InitStruct.Size 		  			= MPU_REGION_SIZE_128KB;
  MPU_InitStruct.SubRegionDisable                       = 0x0;
  MPU_InitStruct.TypeExtField 	                        = MPU_TEX_LEVEL1;
  MPU_InitStruct.AccessPermission                       = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsShareable 	  	                = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable 	  	                = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable 	                        = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.DisableExec 	  	                = MPU_INSTRUCTION_ACCESS_ENABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);	
  
  /* AXISRAM 512KB */
  MPU_InitStruct.Enable 	  			= MPU_REGION_ENABLE;
  MPU_InitStruct.Number 	  			= MPU_REGION_NUMBER3;
  MPU_InitStruct.BaseAddress 	  	                = 0x24000000;
  MPU_InitStruct.Size 		  			= MPU_REGION_SIZE_512KB;
  MPU_InitStruct.SubRegionDisable                       = 0x0;
  MPU_InitStruct.TypeExtField 	                        = MPU_TEX_LEVEL1;
  MPU_InitStruct.AccessPermission                       = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsShareable 	  	                = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable 	  	                = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable 	                        = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.DisableExec 	  	                = MPU_INSTRUCTION_ACCESS_ENABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  
  /* SRAM1 128KB*/
  MPU_InitStruct.Enable                                 = MPU_REGION_ENABLE;
  MPU_InitStruct.Number                                 = MPU_REGION_NUMBER4;
  MPU_InitStruct.BaseAddress                            = 0x30000000;
  MPU_InitStruct.Size                                   = MPU_REGION_SIZE_128KB;
  MPU_InitStruct.AccessPermission                       = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable                           = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.IsCacheable                            = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable                            = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.TypeExtField                           = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable                       = 0x00;
  MPU_InitStruct.DisableExec                            = MPU_INSTRUCTION_ACCESS_ENABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  
  /* SRAM2 128KB */
  MPU_InitStruct.Enable 	  			= MPU_REGION_ENABLE;
  MPU_InitStruct.Number 	  			= MPU_REGION_NUMBER5;
  MPU_InitStruct.BaseAddress 	  	                = 0x30020000;
  MPU_InitStruct.Size                                   = MPU_REGION_SIZE_128KB;
  MPU_InitStruct.AccessPermission                       = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable                           = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.IsCacheable                            = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable                            = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.TypeExtField                           = MPU_TEX_LEVEL1;
  MPU_InitStruct.SubRegionDisable                       = 0x00;
  MPU_InitStruct.DisableExec                            = MPU_INSTRUCTION_ACCESS_ENABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  
  /* SRAM3 : Ethernet  */
  MPU_InitStruct.Enable 	                        = MPU_REGION_ENABLE;
  MPU_InitStruct.Number 	                        = MPU_REGION_NUMBER6;
  MPU_InitStruct.BaseAddress 	                        = 0x30040000;
  MPU_InitStruct.Size 		                        = MPU_REGION_SIZE_32KB;
  MPU_InitStruct.AccessPermission                       = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable 	                        = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.IsCacheable 	                        = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable 	                        = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.TypeExtField 	                        = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable                       = 0x00;
  MPU_InitStruct.DisableExec 	                        = MPU_INSTRUCTION_ACCESS_ENABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  
  /* SRAM4 64KB */
  MPU_InitStruct.Enable 	  			= MPU_REGION_ENABLE;
  MPU_InitStruct.Number 	  			= MPU_REGION_NUMBER7;
  MPU_InitStruct.BaseAddress 	  	                = 0x38000000;
  MPU_InitStruct.Size 		  			= MPU_REGION_SIZE_64KB;
  MPU_InitStruct.SubRegionDisable                       = 0x0;
  MPU_InitStruct.TypeExtField 	                        = MPU_TEX_LEVEL1;
  MPU_InitStruct.AccessPermission                       = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsShareable 	  	                = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable 	  	                = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable 	                        = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.DisableExec 	  	                = MPU_INSTRUCTION_ACCESS_ENABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  
  /* Configure the MPU attributes as WT for QSPI (used 32Mbytes) */
  MPU_InitStruct.Enable 				= MPU_REGION_ENABLE;
  MPU_InitStruct.Number 				= MPU_REGION_NUMBER8;
  MPU_InitStruct.BaseAddress 			        = 0x90000000;
  MPU_InitStruct.Size 					= MPU_REGION_SIZE_32MB;
  MPU_InitStruct.SubRegionDisable                       = 0x00;
  MPU_InitStruct.TypeExtField 		                = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission                       = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsShareable 			        = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable 			        = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable 		                = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.DisableExec 			        = MPU_INSTRUCTION_ACCESS_ENABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
