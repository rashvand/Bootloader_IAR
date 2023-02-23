/* USER CODE BEGIN Includes */
#include <string.h>
#include "stm32746g_qspi.h"
/* USER CODE END Includes */
/* USER CODE BEGIN Includes */
extern QSPI_HandleTypeDef hqspi;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
QSPI_CommandTypeDef s_command;
uint8_t pData[3];
     
uint8_t wData[0x100];
uint8_t rData[0x100];
 
uint32_t i;

static void QSPI_DummyCyclesCfg(QSPI_HandleTypeDef *hqspi);

/* USER CODE END PV */
/*
	ʹ��spi��dspi��qspi��ID
	Ȼ����qspi���Բ�����д������
	�����Կ������ڲ���SPI�Ƿ����á�
	�������ú��ٽ���ӳ��
*/
void QSPI_TEST_1(void)
{

    uart_printf("QuadSPI Test ....\r\n\r\n");
 
    /* ��ʼ��QSPI*/
    BSP_QSPI_Init();
 
    /*��ID����*/
    /* Read Manufacture/Device ID */
    s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction       = READ_ID_CMD;
    s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
    s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
    s_command.Address           = 0x000000;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode          = QSPI_DATA_1_LINE;
    s_command.DummyCycles       = 0;
    s_command.NbData            = 2;
    s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
     
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_QSPI_Receive(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        Error_Handler();
    }
    uart_printf("SPI  I/0 Read Device ID : 0x%2X 0x%2X\r\n", pData[0],pData[1]);
     
    /* Read Manufacture/Device ID Dual I/O*/
    s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction       = DUAL_READ_ID_CMD;
    s_command.AddressMode       = QSPI_ADDRESS_2_LINES;
    s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
    s_command.Address           = 0x000000;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_2_LINES;
    s_command.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
    s_command.AlternateBytes    = 0;
    s_command.DataMode          = QSPI_DATA_2_LINES;
    s_command.DummyCycles       = 0;
    s_command.NbData            = 4;
    s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
 
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK){
        Error_Handler();
    }
	
    if (HAL_QSPI_Receive(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK){
        Error_Handler();
    }
	
    uart_printf("Dual I/O Read Device ID : 0x%2X 0x%2X\r\n",pData[0],pData[1]);
     
    /* Read Manufacture/Device ID Quad I/O*/
    s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction       = QUAD_READ_ID_CMD;
    s_command.AddressMode       = QSPI_ADDRESS_4_LINES;
    s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
    s_command.Address           = 0x000000;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES;
    s_command.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
    s_command.AlternateBytes    = 0x00;
    s_command.DataMode          = QSPI_DATA_4_LINES;
    s_command.DummyCycles       = 4;
    s_command.NbData            = 2;
    s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
 
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_QSPI_Receive(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        Error_Handler();
    }
    uart_printf("Quad I/O Read Device ID : 0x%2X 0x%2X\r\n",pData[0],pData[1]);
 
    /* Read JEDEC ID */
    s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction       = READ_JEDEC_ID_CMD;
    s_command.AddressMode       = QSPI_ADDRESS_NONE;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode          = QSPI_DATA_1_LINE;
    s_command.DummyCycles       = 0;
    s_command.NbData            = 3;
    s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
 
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_QSPI_Receive(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        Error_Handler();
    }
    uart_printf("Read JEDEC ID :  0x%2X 0x%2X 0x%2X\r\n\r\n",pData[0],pData[1],pData[2]);
     
     
    /*-3-QSPI Erase/Write/Read Test */
    /* ���������� */
    for(i =0;i<0x100;i ++)
    {
        //wData[i] = 0x55;
        wData[i] = i;
        rData[i] = 0;
    }
 
    if(BSP_QSPI_Erase_Block(0) == QSPI_OK)
        uart_printf(" QSPI Erase Block ok\r\n");
    else
        Error_Handler();
 
    if(BSP_QSPI_Write(wData,0x00,0x100)== QSPI_OK)
        uart_printf(" QSPI Write ok\r\n");
    else
        Error_Handler();
 
    if(BSP_QSPI_Read(rData,0x00,0x100)== QSPI_OK)
        uart_printf(" QSPI Read ok\r\n\r\n");
    else
        Error_Handler();
 
    uart_printf("QSPI Read Data : \r\n");
    for(i =0;i<0x100;i++)
        uart_printf("0x%02X  ",rData[i]);
    uart_printf("\r\n");
 
    for(i =0;i<0x100;i++)
        if(rData[i] != wData[i])uart_printf("0x%02X 0x%02X ",wData[i],rData[i]);
    uart_printf("\r\n");
    /* check date */
    if(memcmp(wData,rData,0x100) == 0 ) 
        uart_printf(" W25Q128FV QuadSPI Test OK\r\n");
    else
        uart_printf(" W25Q128FV QuadSPI Test False\r\n");
	
	/* �ڴ�ӳ��ģʽ https://www.jianshu.com/p/77f7bd0817e9 */
	QSPI_MemoryMappedTypeDef sMemMappedCfg;
	__IO uint8_t *qspi_addr = (__IO uint8_t *)(0x90000000);

	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = QUAD_OUT_FAST_READ_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.Address           = 0;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_4_LINES;
	s_command.DummyCycles       = W25Q128FV_DUMMY_CYCLES_READ_QUAD;
	s_command.NbData            = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	sMemMappedCfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;

	if (HAL_QSPI_MemoryMapped(&hqspi, &s_command, &sMemMappedCfg) != HAL_OK){
		Error_Handler();
	}

	for (i = 0; i < 0x100; i++){
		uart_printf("0x%02X ", *qspi_addr);
		qspi_addr++;
	}
	uart_printf("--------ok\r\n");
  /* USER CODE END 2 */
}


/*����
STM32Cube_FW_H7_V1.6.0\Projects\STM32H750B-DK\Examples\QSPI\QSPI_MemoryMappedDual\
�е����̣�Ҳ�Ƕ�дFLASH������ڴ0�ӳ�䣬��д�õ����ж�ģʽ��
û�е�ͨ��
*/
#if 0
__IO uint8_t CmdCplt, RxCplt, TxCplt, StatusMatch;

/* Size of buffers */
#define BUFFERSIZE                 (COUNTOF(aTxBuffer) - 1)

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)        (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

/* Buffer used for transmission */
uint8_t aTxBuffer[] = " ****Memory-mapped QSPI communication****  ****Memory-mapped QSPI communication****  ****Memory-mapped QSPI communication****  ****Memory-mapped QSPI communication****  ****Memory-mapped QSPI communication****  ****Memory-mapped QSPI communication**** ";

/* Private function prototypes -----------------------------------------------*/
static void QSPI_WriteEnable(QSPI_HandleTypeDef *hqspi);
static void QSPI_AutoPollingMemReady(QSPI_HandleTypeDef *hqspi);
static void QSPI_DummyCyclesCfg(QSPI_HandleTypeDef *hqspi);
static void QSPI_EnterFourBytesAddress(QSPI_HandleTypeDef *hqspi);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int qspi_test_2(void)
{
  QSPI_CommandTypeDef      sCommand;
  QSPI_MemoryMappedTypeDef sMemMappedCfg;
  uint32_t address = 0;
  __IO uint8_t *qspi_addr = (__IO uint8_t *)(0x90000000);
  uint16_t index;
  __IO uint8_t step = 0;
  

  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.AddressSize       = QSPI_ADDRESS_32_BITS;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  while(1)
  {
    switch(step)
    {
      case 0:
	  	uart_printf("step:0 ");
        CmdCplt = 0;
        
        /* Enable 4 bytes Address mode */
        QSPI_EnterFourBytesAddress(&hqspi);
        
        HAL_Delay(10);
        /* Enable write operations ------------------------------------------- */
        QSPI_WriteEnable(&hqspi);

        /* Erasing Sequence -------------------------------------------------- */
        sCommand.Instruction = SECTOR_ERASE_CMD;
        sCommand.AddressMode = QSPI_ADDRESS_1_LINE;
        sCommand.Address     = address;
        sCommand.DataMode    = QSPI_DATA_NONE;
        sCommand.DummyCycles = 0;

        if (HAL_QSPI_Command_IT(&hqspi, &sCommand) != HAL_OK)
        {
          Error_Handler();
        }
        
        step++;
        break;

      case 1:
	  	uart_printf("step:1 ");
        if(CmdCplt != 0)
        {
          CmdCplt = 0;
          StatusMatch = 0;

          /* Configure automatic polling mode to wait for end of erase ------- */  
          QSPI_AutoPollingMemReady(&hqspi);

          step++;
        }
        break;
        
      case 2:
	  	uart_printf("step:2 ");
        if(StatusMatch != 0)
        {
          StatusMatch = 0;
          TxCplt = 0;
          
          /* Enable write operations ----------------------------------------- */
          QSPI_WriteEnable(&hqspi);

          /* Writing Sequence ------------------------------------------------ */
          sCommand.Instruction = QUAD_INPUT_PAGE_PROG_CMD;
          sCommand.AddressMode = QSPI_ADDRESS_1_LINE;
          sCommand.DataMode    = QSPI_DATA_4_LINES;
          sCommand.NbData      = BUFFERSIZE;

          if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
          {
            Error_Handler();
          }

          if (HAL_QSPI_Transmit_IT(&hqspi, aTxBuffer) != HAL_OK)
          {
            Error_Handler();
          }

          step++;
        }
        break;

      case 3:
	  	uart_printf("step:3 ");
        if(TxCplt != 0)
        {
          TxCplt = 0;
          StatusMatch = 0;

          /* Configure automatic polling mode to wait for end of program ----- */  
          QSPI_AutoPollingMemReady(&hqspi);
        
          step++;
        }
        break;
        
      case 4:
	  	uart_printf("step:4 ");
        if(StatusMatch != 0)
        {
          StatusMatch = 0;
          RxCplt = 0;

          /* Configure Volatile Configuration register (with new dummy cycles) */
          QSPI_DummyCyclesCfg(&hqspi);
          
          /* Reading Sequence ------------------------------------------------ */
          sCommand.Instruction = QUAD_OUT_FAST_READ_CMD;
          sCommand.DummyCycles = W25Q128FV_DUMMY_CYCLES_READ_QUAD;

          sMemMappedCfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;

          if (HAL_QSPI_MemoryMapped(&hqspi, &sCommand, &sMemMappedCfg) != HAL_OK)
          {
            Error_Handler();
          }

          for (index = 0; index < BUFFERSIZE; index++)
          {
            if (*qspi_addr != aTxBuffer[index])
            {
              Error_Handler();
            }
            qspi_addr++;
          }
          uart_printf("ok\r\n");

          address += QSPI_PAGE_SIZE;
          if(address >= QSPI_END_ADDR)
          {
            address = 0;
          }
          qspi_addr = (__IO uint8_t *)(0x90000000 + address);

          step = 0;
        }
        break;
        
      default :
        Error_Handler();
    }
  }


}

/**
  * @brief  Command completed callbacks.
  * @param  hqspi: QSPI handle
  * @retval None
  */
void HAL_QSPI_CmdCpltCallback(QSPI_HandleTypeDef *hqspi)
{
  CmdCplt++;
}

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  hqspi: QSPI handle
  * @retval None
  */
void HAL_QSPI_RxCpltCallback(QSPI_HandleTypeDef *hqspi)
{
  RxCplt++;
}

/**
  * @brief  Tx Transfer completed callbacks.
  * @param  hqspi: QSPI handle
  * @retval None
  */
void HAL_QSPI_TxCpltCallback(QSPI_HandleTypeDef *hqspi)
{
  TxCplt++; 
}

/**
  * @brief  Status Match callbacks
  * @param  hqspi: QSPI handle
  * @retval None
  */
void HAL_QSPI_StatusMatchCallback(QSPI_HandleTypeDef *hqspi)
{
  StatusMatch++;
}

/**
  * @brief  This function send a Write Enable and wait it is effective.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static void QSPI_WriteEnable(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef     s_command;
  QSPI_AutoPollingTypeDef s_config;

  /* Enable write operations */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = WRITE_ENABLE_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure automatic polling mode to wait for write enabling */
  s_config.Match           = 0;
  s_config.Mask            = 0x01|(0x01<<8);
  s_config.MatchMode       = QSPI_MATCH_MODE_AND;
  s_config.StatusBytesSize = 2;
  s_config.Interval        = 0x10;
  s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  s_command.Instruction    = READ_STATUS_REG1_CMD;
  s_command.DataMode       = QSPI_DATA_1_LINE;

  if (HAL_QSPI_AutoPolling(hqspi, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

}

/**
  * @brief  This function read the SR of the memory and wait the EOP.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static void QSPI_AutoPollingMemReady(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef     sCommand;
  QSPI_AutoPollingTypeDef sConfig;

  /* Configure automatic polling mode to wait for memory ready ------ */  
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = READ_STATUS_REG1_CMD;
  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_1_LINE;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  sConfig.Match           = 0x00;
  sConfig.Mask            = 0x0101;
  sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
  sConfig.StatusBytesSize = 2;
  sConfig.Interval        = 0x10;
  sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  if (HAL_QSPI_AutoPolling_IT(hqspi, &sCommand, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function configure the dummy cycles on memory side.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static void QSPI_DummyCyclesCfg(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef s_command;
  uint16_t reg=0;

  /* Initialize the read volatile configuration register command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = READ_VOL_CFG_REG_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_1_LINE;
  s_command.DummyCycles       = 10;
  s_command.NbData            = 2;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(hqspi, (uint8_t *)(&reg), HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable write operations */
  QSPI_WriteEnable(hqspi);

  /* Update volatile configuration register (with new dummy cycles) */
  s_command.Instruction = WRITE_VOL_CFG_REG_CMD;
  MODIFY_REG(reg, 0xF0F0, ((W25Q128FV_DUMMY_CYCLES_READ_QUAD << 4) |
                           (W25Q128FV_DUMMY_CYCLES_READ_QUAD << 12)));

  /* Configure the write volatile configuration register command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Transmission of the data */
  if (HAL_QSPI_Transmit(hqspi, (uint8_t *)(&reg), HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

}

/**
  * @brief  This function set the QSPI memory in 4-byte address mode
  * @param  hqspi: QSPI handle
  * @retval None
  */
static void QSPI_EnterFourBytesAddress(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef s_command;

  /* Initialize the command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = ENTER_4_BYTE_ADDR_MODE_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Enable write operations */
  QSPI_WriteEnable(hqspi);
  
  /* Send the command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure automatic polling mode to wait the memory is ready */
  QSPI_AutoPollingMemReady(hqspi);

}

#endif

