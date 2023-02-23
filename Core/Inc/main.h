
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"
#include "BSP_QSPI.h"
#include "stm32h7xx_ll_spi.h"
#include "stm32h7xx_ll_exti.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


void Error_Handler(void);

#define NRF24_CE_Pin GPIO_PIN_3
#define NRF24_CE_GPIO_Port GPIOE
#define W25Q128_CS_Pin GPIO_PIN_4
#define W25Q128_CS_GPIO_Port GPIOE
#define W25Q128_MISO_Pin GPIO_PIN_5
#define W25Q128_MISO_GPIO_Port GPIOE
#define W25Q128_MOSI_Pin GPIO_PIN_6
#define W25Q128_MOSI_GPIO_Port GPIOE
#define NRF24_IRQ_Pin GPIO_PIN_13
#define NRF24_IRQ_GPIO_Port GPIOC
#define GSM_PWR_KEY_Pin GPIO_PIN_0
#define GSM_PWR_KEY_GPIO_Port GPIOC
#define GPU_SPI_MISO_Pin GPIO_PIN_2
#define GPU_SPI_MISO_GPIO_Port GPIOC
#define GPU_SPI_MOSI_Pin GPIO_PIN_3
#define GPU_SPI_MOSI_GPIO_Port GPIOC
#define WIFI_SDIO_CMD_Pin GPIO_PIN_0
#define WIFI_SDIO_CMD_GPIO_Port GPIOA
#define C1_RS485_RX_Pin GPIO_PIN_3
#define C1_RS485_RX_GPIO_Port GPIOA
#define NRF24_SPI_CS_Pin GPIO_PIN_4
#define NRF24_SPI_CS_GPIO_Port GPIOA
#define NRF24_SPI_SCK_Pin GPIO_PIN_5
#define NRF24_SPI_SCK_GPIO_Port GPIOA
#define NRF24_SPI_MISO_Pin GPIO_PIN_6
#define NRF24_SPI_MISO_GPIO_Port GPIOA
#define USB_PWR_EN_Pin GPIO_PIN_0
#define USB_PWR_EN_GPIO_Port GPIOB
#define USB_OVC_Pin GPIO_PIN_1
#define USB_OVC_GPIO_Port GPIOB
#define BUZZER_Pin GPIO_PIN_11
#define BUZZER_GPIO_Port GPIOE
#define W25Q128_SCK_Pin GPIO_PIN_12
#define W25Q128_SCK_GPIO_Port GPIOE
#define GPU_PDN_Pin GPIO_PIN_13
#define GPU_PDN_GPIO_Port GPIOE
#define GPU_INT_Pin GPIO_PIN_14
#define GPU_INT_GPIO_Port GPIOE
#define SDCARD_SPI_CS_Pin GPIO_PIN_15
#define SDCARD_SPI_CS_GPIO_Port GPIOE
#define SDIO_D0_Pin GPIO_PIN_14
#define SDIO_D0_GPIO_Port GPIOB
#define SDIO_D1_Pin GPIO_PIN_15
#define SDIO_D1_GPIO_Port GPIOB
#define SIM80x_TX_Pin GPIO_PIN_8
#define SIM80x_TX_GPIO_Port GPIOD
#define SIM80x_RX_Pin GPIO_PIN_9
#define SIM80x_RX_GPIO_Port GPIOD
#define DP_INT_Pin GPIO_PIN_10
#define DP_INT_GPIO_Port GPIOD
#define DP_RESET_Pin GPIO_PIN_14
#define DP_RESET_GPIO_Port GPIOD
#define C2_RS485_DE_Pin GPIO_PIN_15
#define C2_RS485_DE_GPIO_Port GPIOD
#define LED_ERROR_Pin GPIO_PIN_6
#define LED_ERROR_GPIO_Port GPIOC
#define SDMMC_CD_Pin GPIO_PIN_7
#define SDMMC_CD_GPIO_Port GPIOC
#define C0_RS232_RTS_Pin GPIO_PIN_8
#define C0_RS232_RTS_GPIO_Port GPIOC
#define C0_RS232_CTS_Pin GPIO_PIN_9
#define C0_RS232_CTS_GPIO_Port GPIOC
#define WIFI_INT_Pin GPIO_PIN_8
#define WIFI_INT_GPIO_Port GPIOA
#define WIFI_REG_ON_Pin GPIO_PIN_15
#define WIFI_REG_ON_GPIO_Port GPIOA
#define DBG_U4_TX_Pin GPIO_PIN_10
#define DBG_U4_TX_GPIO_Port GPIOC
#define C0_RS232_TX_Pin GPIO_PIN_12
#define C0_RS232_TX_GPIO_Port GPIOC
#define WIFI_uSD_SWITCH_Pin GPIO_PIN_0
#define WIFI_uSD_SWITCH_GPIO_Port GPIOD
#define LED_STATUS_Pin GPIO_PIN_1
#define LED_STATUS_GPIO_Port GPIOD
#define C0_RS232_RX_Pin GPIO_PIN_2
#define C0_RS232_RX_GPIO_Port GPIOD
#define GPU_SPI_SCK_Pin GPIO_PIN_3
#define GPU_SPI_SCK_GPIO_Port GPIOD
#define C1_RS485_DE_Pin GPIO_PIN_4
#define C1_RS485_DE_GPIO_Port GPIOD
#define C1_RS485_TX_Pin GPIO_PIN_5
#define C1_RS485_TX_GPIO_Port GPIOD
#define SDIO_CLK_Pin GPIO_PIN_6
#define SDIO_CLK_GPIO_Port GPIOD
#define NRF24_SPI_MOSI_Pin GPIO_PIN_7
#define NRF24_SPI_MOSI_GPIO_Port GPIOD
#define SDIO_D2_Pin GPIO_PIN_3
#define SDIO_D2_GPIO_Port GPIOB
#define SDIO_D3_Pin GPIO_PIN_4
#define SDIO_D3_GPIO_Port GPIOB
#define LED_RUN_Pin GPIO_PIN_5
#define LED_RUN_GPIO_Port GPIOB
#define RTC_SCL_Pin GPIO_PIN_6
#define RTC_SCL_GPIO_Port GPIOB
#define RTC_SDA_Pin GPIO_PIN_7
#define RTC_SDA_GPIO_Port GPIOB
#define SDCARD_DETECT_Pin GPIO_PIN_8
#define SDCARD_DETECT_GPIO_Port GPIOB
#define GPU_SPI_CS_Pin GPIO_PIN_9
#define GPU_SPI_CS_GPIO_Port GPIOB
#define C2_RS485_RX_Pin GPIO_PIN_0
#define C2_RS485_RX_GPIO_Port GPIOE
#define C2_RS485_TX_Pin GPIO_PIN_1
#define C2_RS485_TX_GPIO_Port GPIOE


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
