/**
  ******************************************************************************
  * File Name          : QUADSPI.h
  * Description        : This file provides code for the configuration
  *                      of the QUADSPI instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __quadspi_H
#define __quadspi_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

void MX_QUADSPI_Init(void);
uint8_t CSP_QUADSPI_Init();
uint8_t BSP_QSPI_WriteMemory(uint8_t* buffer, uint32_t address, uint32_t buffer_size);
uint8_t BSP_QSPI_ReadMemory(uint8_t* pData, uint32_t ReadAddr, uint32_t Size);

#define QSPI_BK2_IO2_Pin GPIO_PIN_9
#define QSPI_BK2_IO2_GPIO_Port GPIOG
#define OSC32_OUT_Pin GPIO_PIN_15
#define OSC32_OUT_GPIO_Port GPIOC
#define OSC32_IN_Pin GPIO_PIN_14
#define OSC32_IN_GPIO_Port GPIOC
#define QSPI_BK2_IO3_Pin GPIO_PIN_14
#define QSPI_BK2_IO3_GPIO_Port GPIOG
#define STLINK_TX_Pin GPIO_PIN_10
#define STLINK_TX_GPIO_Port GPIOA
#define STLINK_RX_Pin GPIO_PIN_9
#define STLINK_RX_GPIO_Port GPIOA
#define CEC_CK_MCO1_Pin GPIO_PIN_8
#define CEC_CK_MCO1_GPIO_Port GPIOA
#define QSPI_BK1_NCS_Pin GPIO_PIN_6
#define QSPI_BK1_NCS_GPIO_Port GPIOG
#define OSC_OUT_Pin GPIO_PIN_1
#define OSC_OUT_GPIO_Port GPIOH
#define OSC_IN_Pin GPIO_PIN_0
#define OSC_IN_GPIO_Port GPIOH
#define QSPI_BK1_IO3_Pin GPIO_PIN_6
#define QSPI_BK1_IO3_GPIO_Port GPIOF
#define QSPI_BK1_IO2_Pin GPIO_PIN_7
#define QSPI_BK1_IO2_GPIO_Port GPIOF
#define QSPI_BK1_IO1_Pin GPIO_PIN_9
#define QSPI_BK1_IO1_GPIO_Port GPIOF
#define QSPI_BK2_IO0_Pin GPIO_PIN_2
#define QSPI_BK2_IO0_GPIO_Port GPIOH
#define QSPI_BK2_IO1_Pin GPIO_PIN_3
#define QSPI_BK2_IO1_GPIO_Port GPIOH
#define QSPI_CLK_Pin GPIO_PIN_2
#define QSPI_CLK_GPIO_Port GPIOB
#define QSPI_BK1_IO0_Pin GPIO_PIN_11
#define QSPI_BK1_IO0_GPIO_Port GPIOD
/* USER CODE BEGIN Prototypes */

#define sFLASH_ID                		0x20BA20	    //MX25L512 Detail pape 35 in datasheet
#define MAX_READ_SIZE                   8


/*MX25L512 memory parameters*/
#define MEMORY_FLASH_SIZE				0x4000000 /* 512 MBits => 64MBytes */
#define MEMORY_BLOCK_SIZE				0x1000   /* 1024 sectors of 64KBytes */
#define MEMORY_SECTOR_SIZE				0x400    /* 16384 subsectors of 4kBytes */
#define MEMORY_PAGE_SIZE				0x100     /* 262144 pages of 256 bytes */


#define BLOCK_SIZE                           511


/*MX25L512 commands */
#define WRITE_ENABLE_CMD 						0x06
#define READ_STATUS_REG_CMD 					0x05
#define WRITE_STATUS_REG_CMD 					0x01
#define SECTOR_ERASE_CMD 						0x20
#define CHIP_ERASE_CMD 							0xC7
#define QUAD_IN_FAST_PROG_CMD 					0x38
#define QUAD_INPUT_PAGE_PROG_CMD             	0x32

#define READ_CONFIGURATION_REG_CMD 				0x15
#define QUAD_READ_IO_CMD						0xEC
#define QUAD_OUT_FAST_READ_CMD 					0x6B
#define QUAD_INOUT_FAST_READ_CMD             0xEB
#define READ_VOL_CFG_REG_CMD                 0x85
#define WRITE_VOL_CFG_REG_CMD                0x81

#define QPI_ENABLE_CMD 						0x35
#define DUMMY_CLOCK_CYCLES_READ_QUAD 		10
#define RESET_ENABLE_CMD 					0x66
#define RESET_EXECUTE_CMD 					0x99
#define DISABLE_QIP_MODE 					0xf5
#define READ_JEDEC_ID_CMD                    0x9F

#define READ_CMD                             0x03

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ quadspi_H */



 HAL_StatusTypeDef EraseSector_QSPI(uint32_t Start_Address ,uint32_t End_Address);
 HAL_StatusTypeDef Write_QSPI(uint8_t* buffer, uint32_t address, uint32_t buffer_size);
 HAL_StatusTypeDef Read_QSPI(uint8_t* pData, uint32_t ReadAddr, uint32_t Size);
 void App_QSPI(void * argument);


 /* USER CODE END Private defines */


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
