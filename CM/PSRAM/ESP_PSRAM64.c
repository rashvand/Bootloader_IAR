
#include "main.h"
#include "ESP_PSRAM64.h"

uint8_t PSRAM64_EID[8];

uint8_t SPI2_ReadWriteByte (const uint8_t byte_s){
  
  SPI2->CR1 |= SPI_CR1_CSTART;
  //	while ((SPI2->SR & SPI_SR_TXP) == 0){}
  *((__IO uint8_t *)&SPI2->TXDR) = byte_s;
  //	while ((SPI2->SR & SPI_FLAG_TXC) == 0){}
  while ((SPI2->SR & SPI_SR_RXPLVL) == LL_SPI_RX_FIFO_0PACKET){}
  return (*((__IO uint8_t *)&SPI2->RXDR));
}

__STATIC_INLINE void opStart(const uint8_t command, const uint32_t address){
  PSRAM_CS_LOW;
  SPI2_ReadWriteByte(command);
  SPI2_ReadWriteByte((uint8_t)(address >> 16));
  SPI2_ReadWriteByte((uint8_t)(address >> 8));
  SPI2_ReadWriteByte((uint8_t)(address));
}

void PSRAM_ReadDevID(uint8_t *EID){
  opStart(PSRAM_ReadID, 0U);
  for(uint8_t i=0; i<8; i++){
    EID[i] = SPI2_ReadWriteByte(0x00);
  }
}

void PSRAM_Write_byte(const uint32_t address, const uint8_t data){
  opStart(PSRAM_Write, address);
  SPI2_ReadWriteByte(data);
  PSRAM_CS_HIGH;
}

void PSRAM_Read_byte(const uint32_t address, uint8_t *buffer){
  opStart(PSRAM_Read, address);
  *buffer = SPI2_ReadWriteByte(0);
  PSRAM_CS_HIGH;
}

void PSRAM_Write_bytes(const uint32_t address, uint8_t *buffer, uint32_t size){
 
  for (uint32_t i = 0; i < size; ) {
    opStart(PSRAM_Write, address + i);
    for (uint8_t j = 0; j < PSRAM_NormalReadBlockSize && i < size; j++, i++) {
      SPI2_ReadWriteByte(buffer[i]);
    }
    PSRAM_CS_HIGH;
  }
}

void PSRAM_Read_bytes(const uint32_t address, uint8_t * buffer, uint32_t size){
  
  for (uint32_t i = 0; i < size; ) { 
    opStart(PSRAM_Read, address + i);
    for (uint8_t j = 0; j < PSRAM_NormalReadBlockSize && i < size; j++, i++) {
      buffer[i] = SPI2_ReadWriteByte(0);
    }
    PSRAM_CS_HIGH;
  }
}

void PSRAM_FastRead_bytes(const uint32_t address, uint8_t * buffer, uint32_t size){
  
  for (uint32_t i = 0; i < size; ) { 
    opStart(PSRAM_FastRead, address + i);
    SPI2_ReadWriteByte(0);
    for (uint32_t j = 0; j < PSRAM_FastReadBlockSize && i < size; j++, i++) {
      buffer[i] = SPI2_ReadWriteByte(0);
    }
    PSRAM_CS_HIGH;
  }
}

void PSRAM_ResetChip(void){
  PSRAM_CS_HIGH;
  PSRAM_CS_LOW;
  SPI2_ReadWriteByte(PSRAM_ResetEnable);
  PSRAM_CS_HIGH;
  PSRAM_CS_LOW;
  SPI2_ReadWriteByte(PSRAM_Reset);
  PSRAM_CS_HIGH;
}

void PSRAM_Init(void){
  PSRAM_ResetChip();
  
  PSRAM_ReadDevID(PSRAM64_EID);
  
  uint8_t datatemp[10];
  PSRAM_Read_byte(0, &datatemp[0]);
}

