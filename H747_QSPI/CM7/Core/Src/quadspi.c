/**
  ******************************************************************************
  * File Name          : QUADSPI.c
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

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "quadspi.h"

/* USER CODE BEGIN 0 */
static uint8_t QSPI_WriteEnable(void);
static uint8_t QSPI_AutoPollingMemReady(void);
static uint8_t QSPI_Configuration(void);
static uint32_t QSPI_FLASH_ReadID();
static uint8_t QSPI_ResetChip();
static uint8_t BSP_QSPI_EraseSector(uint32_t EraseStartAddress ,uint32_t EraseEndAddress);

QSPI_HandleTypeDef hqspi;

typedef enum
{
	QSPI_READ_EVENT = 1,
	QSPI_WRITE_EVENT,
	QSPI_ERASE_EVENT,
}QSPI_EventTypeDef;

typedef enum
{	_None_ = 0,
    QSPI_TX_COMPLETED,
    QSPI_RX_COMPLETED,
	QSPI_ERATE_COMPLETED
}QSPI_status_t;

volatile static QSPI_status_t StorageStatus_QSPI_TX;
volatile static QSPI_status_t StorageStatus_QSPI_RX;
volatile static QSPI_status_t StorageStatus_QSPI_ERATE;

volatile uint8_t rx_complete = 0;
volatile uint8_t tx_complete = 0;
volatile uint8_t status_matched = 0;
volatile uint8_t command_complete = 0;
volatile uint8_t qspi_locked = 0;

static uint32_t EraseS_Address;
volatile static uint32_t EraseE_Address;

static uint8_t* pData_Read;
static uint32_t address_Read;
volatile static uint32_t size_Read;

static uint8_t* pData_Write;
static uint32_t address_Write;
volatile static uint32_t size_Write;

/* QUADSPI init function */
void MX_QUADSPI_Init(void)
{

  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 2;
  hqspi.Init.FifoThreshold = 4;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
  hqspi.Init.FlashSize = 25;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  hqspi.Init.FlashID = QSPI_FLASH_ID_1;
  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
	  while(1);
  }

}

void HAL_QSPI_MspInit(QSPI_HandleTypeDef* hqspi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if(hqspi->Instance==QUADSPI)
  {
  /* USER CODE BEGIN QUADSPI_MspInit 0 */

  /* USER CODE END QUADSPI_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_QSPI_CLK_ENABLE();

    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**QUADSPI GPIO Configuration
    PG9     ------> QUADSPI_BK2_IO2
    PG14     ------> QUADSPI_BK2_IO3
    PG6     ------> QUADSPI_BK1_NCS
    PF6     ------> QUADSPI_BK1_IO3
    PF7     ------> QUADSPI_BK1_IO2
    PF9     ------> QUADSPI_BK1_IO1
    PH2     ------> QUADSPI_BK2_IO0
    PH3     ------> QUADSPI_BK2_IO1
    PB2     ------> QUADSPI_CLK
    PD11     ------> QUADSPI_BK1_IO0
    */
    GPIO_InitStruct.Pin = QSPI_BK2_IO2_Pin|QSPI_BK2_IO3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = QSPI_BK1_NCS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(QSPI_BK1_NCS_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = QSPI_BK1_IO3_Pin|QSPI_BK1_IO2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = QSPI_BK1_IO1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(QSPI_BK1_IO1_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = QSPI_BK2_IO0_Pin|QSPI_BK2_IO1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = QSPI_CLK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(QSPI_CLK_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = QSPI_BK1_IO0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(QSPI_BK1_IO0_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN QUADSPI_MspInit 1 */

  /* USER CODE END QUADSPI_MspInit 1 */
  }


  /*##-3- Configure the NVIC for QSPI #########################################*/
    /* NVIC configuration for QSPI interrupt */
    HAL_NVIC_SetPriority(QUADSPI_IRQn, 0x0F, 0);
    HAL_NVIC_EnableIRQ(QUADSPI_IRQn);

}

/**
* @brief QSPI MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hqspi: QSPI handle pointer
* @retval None
*/
void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef* hqspi)
{

  if(hqspi->Instance==QUADSPI)
  {
  /* USER CODE BEGIN QUADSPI_MspDeInit 0 */

  /* USER CODE END QUADSPI_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_QSPI_CLK_DISABLE();

    /**QUADSPI GPIO Configuration
    PG9     ------> QUADSPI_BK2_IO2
    PG14     ------> QUADSPI_BK2_IO3
    PG6     ------> QUADSPI_BK1_NCS
    PF6     ------> QUADSPI_BK1_IO3
    PF7     ------> QUADSPI_BK1_IO2
    PF9     ------> QUADSPI_BK1_IO1
    PH2     ------> QUADSPI_BK2_IO0
    PH3     ------> QUADSPI_BK2_IO1
    PB2     ------> QUADSPI_CLK
    PD11     ------> QUADSPI_BK1_IO0
    */
    HAL_GPIO_DeInit(GPIOG, QSPI_BK2_IO2_Pin|QSPI_BK2_IO3_Pin|QSPI_BK1_NCS_Pin);

    HAL_GPIO_DeInit(GPIOF, QSPI_BK1_IO3_Pin|QSPI_BK1_IO2_Pin|QSPI_BK1_IO1_Pin);

    HAL_GPIO_DeInit(GPIOH, QSPI_BK2_IO0_Pin|QSPI_BK2_IO1_Pin);

    HAL_GPIO_DeInit(QSPI_CLK_GPIO_Port, QSPI_CLK_Pin);

    HAL_GPIO_DeInit(QSPI_BK1_IO0_GPIO_Port, QSPI_BK1_IO0_Pin);

  /* USER CODE BEGIN QUADSPI_MspDeInit 1 */

  /* USER CODE END QUADSPI_MspDeInit 1 */
  }
  /*##-1- Disable the NVIC for QSPI##################################*/
  HAL_NVIC_DisableIRQ(QUADSPI_IRQn);

  /*##-2- Disable peripherals ################################################*/
  /* De-configure DMA channel */
}

/* USER CODE BEGIN 1 */

/* QUADSPI init function */
uint8_t CSP_QUADSPI_Init() {
	//prepare QSPI peripheral for ST-Link Utility operations
	if (HAL_QSPI_DeInit(&hqspi) != HAL_OK) {
		while(1);
	}

	MX_QUADSPI_Init();

	if (QSPI_ResetChip() != HAL_OK) {
		while(1);
	}

	if (sFLASH_ID != QSPI_FLASH_ReadID())
	{
		while(1);
	}

	if (QSPI_Configuration() != HAL_OK) {
		while(1);
	}

	return HAL_OK;
}

static uint8_t QSPI_AutoPollingMemReady(void) {

	QSPI_CommandTypeDef sCommand;
	 QSPI_AutoPollingTypeDef sConfig;

	/* Configure automatic polling mode to wait for memory ready ------ */
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction = READ_STATUS_REG_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = QSPI_DATA_1_LINE;
	sCommand.DummyCycles = 0;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	sConfig.Match = 0x00;
	sConfig.Mask = 0x01;
	sConfig.MatchMode = QSPI_MATCH_MODE_AND;
	sConfig.StatusBytesSize = 1;
	sConfig.Interval = 0x10;
	sConfig.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

	status_matched = 0;
	if (HAL_QSPI_AutoPolling_IT(&hqspi, &sCommand, &sConfig) != HAL_OK) {
		return HAL_ERROR;
	}
	while(!status_matched);
	return HAL_OK;
}

static uint8_t QSPI_WriteEnable(void) {
	QSPI_CommandTypeDef sCommand;
	QSPI_AutoPollingTypeDef sConfig;

	/* Enable write operations ------------------------------------------ */
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction = WRITE_ENABLE_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = QSPI_DATA_NONE;
	sCommand.DummyCycles = 0;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {

		return HAL_ERROR;
	}


	/* Configure automatic polling mode to wait for write enabling ---- */
	sConfig.Match = 0x02;
	sConfig.Mask = 0x02;
	sConfig.MatchMode = QSPI_MATCH_MODE_AND;
	sConfig.StatusBytesSize = 1;
	sConfig.Interval = 0x10;
	sConfig.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

	sCommand.Instruction = READ_STATUS_REG_CMD;
	sCommand.DataMode = QSPI_DATA_1_LINE;

	status_matched = 0;
	if (HAL_QSPI_AutoPolling_IT(&hqspi, &sCommand, &sConfig) != HAL_OK) {
		return HAL_ERROR;
	}
	while(!status_matched);

	return HAL_OK;
}


/*Enable quad mode and set dummy cycles count*/
static uint8_t QSPI_Configuration(void) {

	QSPI_CommandTypeDef sCommand;
	uint8_t test_buffer[4] = { 0 };
	/*read status register*/
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction = READ_STATUS_REG_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = QSPI_DATA_1_LINE;
	sCommand.DummyCycles = 0;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.NbData = 1;

	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {

		return HAL_ERROR;
	}



	rx_complete = 0;
	if (HAL_QSPI_Receive_IT(&hqspi, test_buffer) != HAL_OK) {
		return HAL_ERROR;
	}
	while(!rx_complete);
	/*read configuration register*/
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction = READ_CONFIGURATION_REG_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = QSPI_DATA_1_LINE;
	sCommand.DummyCycles = 0;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.NbData = 1;

	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {

		return HAL_ERROR;
	}

	rx_complete = 0;
	if (HAL_QSPI_Receive_IT(&hqspi, &(test_buffer[1])) != HAL_OK) {
		return HAL_ERROR;
	}
	while(!rx_complete);
	/*modify buffer to enable quad mode*/
	test_buffer[0] |= 0x40;

	/*set dummy cycles*/
	test_buffer[1] |= 0xC0;

	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.Instruction = WRITE_STATUS_REG_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.DataMode = QSPI_DATA_1_LINE;
	sCommand.DummyCycles = 0;
	sCommand.NbData = 2;

	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {

		return HAL_ERROR;
	}

	tx_complete = 0;
	if (HAL_QSPI_Transmit_IT(&hqspi, test_buffer) != HAL_OK) {
		return HAL_ERROR;
	}
	while(!tx_complete);
	return HAL_OK;
}

static uint8_t BSP_QSPI_EraseSector(uint32_t EraseStartAddress, uint32_t EraseEndAddress) {

	QSPI_CommandTypeDef sCommand;

	EraseStartAddress = EraseStartAddress
			- EraseStartAddress % MEMORY_SECTOR_SIZE;

	/* Erasing Sequence -------------------------------------------------- */
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.Instruction = SECTOR_ERASE_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_1_LINE;

	sCommand.DataMode = QSPI_DATA_NONE;
	sCommand.DummyCycles = 0;

	while (EraseEndAddress >= EraseStartAddress) {
		sCommand.Address = (EraseStartAddress & 0x0FFFFFFF);

		if (QSPI_WriteEnable() != HAL_OK) {
			return HAL_ERROR;
		}
		command_complete = 0;
		if (HAL_QSPI_Command_IT(&hqspi, &sCommand)
				!= HAL_OK) {
			return HAL_ERROR;
		}
		while(!command_complete);

		EraseStartAddress += MEMORY_SECTOR_SIZE;

		if (QSPI_AutoPollingMemReady() != HAL_OK) {
			return HAL_ERROR;
		}
	}

	return HAL_OK;
}


uint8_t BSP_QSPI_WriteMemory(uint8_t* buffer, uint32_t address,uint32_t buffer_size) {

	QSPI_CommandTypeDef sCommand;
	uint32_t end_addr, current_size, current_addr;

	/* Erase before write */
	uint32_t era_start,era_end;
	era_start = address;
	era_end = address + buffer_size;

	if(BSP_QSPI_EraseSector(era_start, era_end) != HAL_OK)
	{
		return HAL_ERROR;
	}

	/* Calculation of the size between the write address and the end of the page */
	current_addr = 0;



	while (current_addr <= address) {
		current_addr += MEMORY_PAGE_SIZE;
	}
	current_size = current_addr - address;

	/* Check if the size of the data is less than the remaining place in the page */
	if (current_size > buffer_size) {
		current_size = buffer_size;
	}

	/* Initialize the adress variables */
	current_addr = address;
	end_addr = address + buffer_size;

	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.Instruction = QUAD_IN_FAST_PROG_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_4_LINES;
	sCommand.DataMode = QSPI_DATA_4_LINES;
	sCommand.NbData = buffer_size;
	sCommand.Address = address;
	sCommand.DummyCycles = 0;

	/* Perform the write page by page */
	do {
		sCommand.Address = current_addr;
		sCommand.NbData = current_size;

		if (current_size == 0) {
			return HAL_OK;
		}

		/* Enable write operations */
		if (QSPI_WriteEnable() != HAL_OK) {
			return HAL_ERROR;
		}

		/* Configure the command */
		if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
				!= HAL_OK) {

			return HAL_ERROR;
		}


		tx_complete = 0;
		/* Transmission of the data */
		if (HAL_QSPI_Transmit_IT(&hqspi, buffer) != HAL_OK) {

			return HAL_ERROR;
		}
		while(!tx_complete);

		/* Configure automatic polling mode to wait for end of program */
		if (QSPI_AutoPollingMemReady() != HAL_OK) {
			return HAL_ERROR;
		}

		/* Update the address and size variables for next page programming */
		current_addr += current_size;
		buffer += current_size;
		current_size =
				((current_addr + MEMORY_PAGE_SIZE) > end_addr) ?
						(end_addr - current_addr) : MEMORY_PAGE_SIZE;
	} while (current_addr <= end_addr);

	return HAL_OK;

}


static uint8_t QSPI_ResetChip() {
	QSPI_CommandTypeDef sCommand;
	uint32_t temp = 0;
	/* Erasing Sequence -------------------------------------------------- */
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.Instruction = RESET_ENABLE_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.Address = 0;
	sCommand.DataMode = QSPI_DATA_NONE;
	sCommand.DummyCycles = 0;

	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {

		return HAL_ERROR;
	}


	for (temp = 0; temp < 0x2f; temp++) {
		__NOP();
	}

	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.Instruction = RESET_EXECUTE_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.Address = 0;
	sCommand.DataMode = QSPI_DATA_NONE;
	sCommand.DummyCycles = 0;

	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {

		return HAL_ERROR;
	}

	return HAL_OK;
}



uint8_t BSP_QSPI_ReadMemory(uint8_t* pData, uint32_t ReadAddr, uint32_t Size)
{
    QSPI_CommandTypeDef s_command;
    if(Size == 0)
    	Size = 1;
     /* Initialize read command */
     s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
     s_command.Instruction       = READ_CMD;
     s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
     s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
     s_command.Address           = ReadAddr;
     s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
     s_command.DataMode          = QSPI_DATA_1_LINE;
     s_command.DummyCycles       = 0;
     s_command.NbData            = Size;
     s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
     s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
     s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

     /* configuration command */
 	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
 			!= HAL_OK) {

 		return HAL_ERROR;
 	}

    /* Receive data */
     rx_complete = 0;
     if(HAL_QSPI_Receive_IT(&hqspi, pData)!= HAL_OK) {
         return HAL_ERROR;
     }
     while(!rx_complete);

     return HAL_OK;
 }

static uint32_t QSPI_FLASH_ReadID()
 {
     QSPI_CommandTypeDef s_command;
     uint32_t Temp = 0;
     uint8_t pData[3];
     /* Read JEDEC ID */
     s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
     s_command.Instruction       = READ_JEDEC_ID_CMD;
     s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
     s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
     s_command.DataMode          = QSPI_DATA_1_LINE;
     s_command.AddressMode       = QSPI_ADDRESS_NONE;
     s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
     s_command.DummyCycles       = 0;
     s_command.NbData            = 3;
     s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
     s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
     s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

     if(HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
  		return HAL_ERROR;

     }

     if(HAL_QSPI_Receive(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)!= HAL_OK) {
       /* Users can add some code here to handle this error */
  		return HAL_ERROR;

     }

    Temp = ( pData[2] | pData[1]<<8 )| ( pData[0]<<16 );
	return Temp;

}


/**
 * @brief  Rx Transfer completed callbacks.
 * @param  hqspi: QSPI handle
 * @retval None
 */
void HAL_QSPI_RxCpltCallback(QSPI_HandleTypeDef *hqspi)
{
	rx_complete = 1;
}

/**
 * @brief  Rx Transfer completed callbacks.
 * @param  hqspi: QSPI handle
 * @retval None
 */
void HAL_QSPI_TxCpltCallback(QSPI_HandleTypeDef *hqspi)
{
	tx_complete = 1;
}

/**
 * @brief  Status Match callback.
 * @param  hqspi: QSPI handle
 * @retval None
 */
void HAL_QSPI_StatusMatchCallback(QSPI_HandleTypeDef *hqspi)
{
	status_matched = 1;
}

/**
  * @brief  Command completed callbacks.
  * @param  hqspi: QSPI handle
  * @retval None
  */
void HAL_QSPI_CmdCpltCallback(QSPI_HandleTypeDef *hqspi)
{
	command_complete = 1;
}

HAL_StatusTypeDef EraseSector_QSPI(uint32_t Start_Address ,uint32_t End_Address)
{

	while(EraseE_Address != 0)
	{
		HAL_Delay(1);
	}
	EraseS_Address = Start_Address;
	EraseE_Address = End_Address;

	while(StorageStatus_QSPI_ERATE != QSPI_ERATE_COMPLETED)
	{
		HAL_Delay(1);
	}
	StorageStatus_QSPI_ERATE = 0;
	EraseE_Address = 0;


	return HAL_OK;
}

HAL_StatusTypeDef Read_QSPI(uint8_t* pData, uint32_t ReadAddr, uint32_t Size)
{


	while(size_Read != 0)
	{
		HAL_Delay(1);
	}
	address_Read 	= ReadAddr;
	size_Read 		= Size;

    while(StorageStatus_QSPI_RX != QSPI_RX_COMPLETED)
    {
		HAL_Delay(1);
    }

    memcpy(pData, pData_Read, Size);
    free(pData_Read);
    StorageStatus_QSPI_RX = 0;
    size_Read = 0;

    return HAL_OK;

}


HAL_StatusTypeDef Write_QSPI(uint8_t* buffer, uint32_t address, uint32_t buffer_size)
{
	while(size_Write != 0)
	{
		HAL_Delay(1);
	}


	pData_Write = malloc(buffer_size*sizeof(uint8_t));
    memcpy(pData_Write, buffer, buffer_size);
    address_Write = address;
    size_Write = buffer_size;

    while(StorageStatus_QSPI_TX != QSPI_TX_COMPLETED)
    {
		HAL_Delay(1);
    }
    StorageStatus_QSPI_TX = 0;
    size_Write = 0;

    return HAL_OK;

}


//void App_QSPI(void * argument)
//{
//	 __State_t *state_hal = (__State_t *)(argument);
//
//	 /* Create Storage Message Queue */
//	 osMessageQDef(osqueue_QSPI, 10, uint16_t);
//	 StorageEvent_QSPI = osMessageCreate (osMessageQ(osqueue_QSPI), NULL);
//
//	  if(  CSP_QUADSPI_Init() != HAL_OK)
//			goto unlock_exit;
//
//	  state_hal->state = TASK_READY;
//	  state_hal->error = __PASS__;
//
//	  osEvent event;
//
//	 	   for( ;; )
//	 	   {
//	 	     event = osMessageGet( StorageEvent_QSPI, osWaitForever );
//
//	 	     if( event.status == osEventMessage )
//	 	     {
//	 	       switch(event.value.v)
//	 	       {
//
//	 	         case QSPI_READ_EVENT:
//	 	        	pData_Read  	= malloc(size_Read*sizeof(uint8_t));
//	 	        	if(BSP_QSPI_ReadMemory(pData_Read, address_Read, size_Read) == HAL_OK)
//	 	        		StorageStatus_QSPI_RX = QSPI_RX_COMPLETED;
//	 	        	 	 break;
//	 	         case QSPI_WRITE_EVENT:
//
//	 	        	 if(BSP_QSPI_WriteMemory(pData_Write, address_Write, size_Write) == HAL_OK)
//	 	        		 free(pData_Write);
//
//	 	        	StorageStatus_QSPI_TX = QSPI_TX_COMPLETED;
//
//	 	        	 	 break;
//	 	        case QSPI_ERASE_EVENT:
//
//	 	        	if(BSP_QSPI_EraseSector(EraseS_Address, EraseE_Address) == HAL_OK)
//	 	        		StorageStatus_QSPI_ERATE = QSPI_ERATE_COMPLETED;
//	 	        		break;
//	 	         default:
//	 	         break;
//	 	       }
//	 	     }
//	 	   }
//
// unlock_exit:
// 	 while(1)
// 	 {
// 		 state_hal->state = TASK_INTIALIZE;
// 		 state_hal->error = __ISSUE__;
// 		osDelay(100000);  /* kill app */
// 	 }
//}


/**
 * @brief  Timeout callback.
 * @param  hqspi: QSPI handle
 * @retval None
 */


/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
