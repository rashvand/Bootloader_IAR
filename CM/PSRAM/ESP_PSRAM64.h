#ifndef	ESP_PSRAM64_H
#define	ESP_PSRAM64_H

#include <main.h>

extern uint8_t PSRAM64_EID[8];

#define PSRAM_CS_LOW                    GPIOE->BSRR = (uint32_t)GPIO_PIN_6 << 16U
#define PSRAM_CS_HIGH                   GPIOE->BSRR = GPIO_PIN_6


#define PSRAM_TotalCapacity             (uint32_t)(8U * 1024U * 1024U)

#define PSRAM_NormalReadBlockSize       (uint16_t)32
#define PSRAM_FastReadBlockSize         (uint16_t)1024


#define PSRAM_Read                      (uint8_t)0x03
#define PSRAM_FastRead                  (uint8_t)0x0B
#define PSRAM_FastReadQuad              (uint8_t)0xEB
#define PSRAM_Write                     (uint8_t)0x02
#define PSRAM_QuadWrite                 (uint8_t)0x38
#define PSRAM_EnterQuadMode             (uint8_t)0x35
#define PSRAM_ExitQuadMode              (uint8_t)0xF5
#define PSRAM_ResetEnable               (uint8_t)0x66
#define PSRAM_Reset                     (uint8_t)0x99
#define PSRAM_SetBurstLength            (uint8_t)0xC0
#define PSRAM_ReadID                    (uint8_t)0x9F


extern void PSRAM_Init(void);
extern void PSRAM_ReadDevID(uint8_t *EID);
extern void PSRAM_Write_byte(const uint32_t address, const uint8_t data);
extern void PSRAM_Read_byte(const uint32_t address, uint8_t *buffer);
extern void PSRAM_FastRead_bytes(const uint32_t address, uint8_t * buffer, uint32_t size);
extern void PSRAM_Write_bytes(const uint32_t address, uint8_t *buffer, uint32_t size);
extern void PSRAM_Read_bytes(const uint32_t address, uint8_t * buffer, uint32_t size);
extern void PSRAM_ResetChip(void);

#endif	//	ESP_PSRAM64_H