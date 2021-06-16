/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "quadspi.h"

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif

#define SECTORS_COUNT 100

//QSPI_HandleTypeDef hqspi;

UART_HandleTypeDef huart8;

#define SECTORS_COUNT 100
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
//static void MX_QUADSPI_Init(void);
static void MX_UART8_Init(void);

int __io_putchar (int ch) {
	uint8_t c = ch;
	HAL_UART_Transmit(&huart8, &c, 1, HAL_MAX_DELAY);
	return ch;
}

int main(void)
{
	int32_t timeout;
	timeout = 0xFFFF;
	while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
	if ( timeout < 0 )
	{
		Error_Handler();
	}
	HAL_Init();
	SystemClock_Config();
	__HAL_RCC_HSEM_CLK_ENABLE();
	HAL_HSEM_FastTake(HSEM_ID_0);
	HAL_HSEM_Release(HSEM_ID_0,0);
	timeout = 0xFFFF;
	while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0));
	if ( timeout < 0 )
	{
		Error_Handler();
	}

	MX_GPIO_Init();
	MX_QUADSPI_Init();
	MX_UART8_Init();

  	uint8_t buffer_test[MEMORY_SECTOR_SIZE];
      uint32_t var = 0;

  	CSP_QUADSPI_Init();

  	for (var = 0; var < MEMORY_SECTOR_SIZE; var++) {
  		buffer_test[var] = (var & 0xff);
  	}

  	for (var = 0; var < SECTORS_COUNT; var++) {

  		if (CSP_QSPI_EraseSector(var * MEMORY_SECTOR_SIZE,
  				(var + 1) * MEMORY_SECTOR_SIZE - 1) != HAL_OK) {

  			while (1)
  				;  //breakpoint - error detected
  		}

  		if (CSP_QSPI_WriteMemory(buffer_test, var * MEMORY_SECTOR_SIZE,
  				sizeof(buffer_test)) != HAL_OK) {

  			while (1)
  				;  //breakpoint - error detected
  		}

  	}

  	if (CSP_QSPI_EnableMemoryMappedMode() != HAL_OK) {

  		while (1)
  			; //breakpoint - error detected
  	}

  	for (var = 0; var < SECTORS_COUNT; var++) {
  		if (memcmp(buffer_test,
  				(uint8_t*) (0x90000000 + var * MEMORY_SECTOR_SIZE),
  				MEMORY_SECTOR_SIZE) != HAL_OK) {
  			while (1)
  				;  //breakpoint - error detected - otherwise QSPI works properly
  		}
  	}

	while (1)
	{
		printf("Flash Memory\r\n");
		HAL_Delay(1000);
	}
}

void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	HAL_StatusTypeDef ret = HAL_OK;

	/*!< Supply configuration update enable */
	HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

	/* The voltage scaling allows optimizing the power consumption when the device is
	 clocked below the maximum system frequency, to update the voltage scaling value
	 regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
	RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	RCC_OscInitStruct.PLL.PLLM = 5;
	RCC_OscInitStruct.PLL.PLLN = 160;
	RCC_OscInitStruct.PLL.PLLFRACN = 0;
	RCC_OscInitStruct.PLL.PLLP = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLQ = 4;

	RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
	RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
	ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
	if(ret != HAL_OK)
	{
	Error_Handler();
	}

	/* Select PLL as system clock source and configure  bus clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
								 RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
	RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
	ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
	if(ret != HAL_OK)
	{
	Error_Handler();
	}
	/*
	Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
		  (GPIO, SPI, FMC, QSPI ...)  when  operating at  high frequencies(please refer to product datasheet)
		  The I/O Compensation Cell activation  procedure requires :
		- The activation of the CSI clock
		- The activation of the SYSCFG clock
		- Enabling the I/O Compensation Cell : setting bit[0] of register SYSCFG_CCCSR

		  To do this please uncomment the following code
	*/

	__HAL_RCC_CSI_ENABLE() ;

	__HAL_RCC_SYSCFG_CLK_ENABLE() ;

	HAL_EnableCompensationCell();

}

//static void MX_QUADSPI_Init(void)
//{
//	/* QUADSPI parameter configuration*/
//	hqspi.Instance = QUADSPI;
//	hqspi.Init.ClockPrescaler = 2;
//	hqspi.Init.FifoThreshold = 4;
//	hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
//	hqspi.Init.FlashSize = 25;
//	hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
//	hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
//	hqspi.Init.FlashID = QSPI_FLASH_ID_1;
//	hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
//	if (HAL_QSPI_Init(&hqspi) != HAL_OK)
//	{
//	Error_Handler();
//	}
//}

static void MX_UART8_Init(void)
{
  huart8.Instance = UART8;
  huart8.Init.BaudRate = 115200;
  huart8.Init.WordLength = UART_WORDLENGTH_8B;
  huart8.Init.StopBits = UART_STOPBITS_1;
  huart8.Init.Parity = UART_PARITY_NONE;
  huart8.Init.Mode = UART_MODE_TX_RX;
  huart8.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart8.Init.OverSampling = UART_OVERSAMPLING_16;
  huart8.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart8.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart8.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart8, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart8, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart8) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pins : PI12 PI13 PI14 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

}

void Error_Handler(void)
{

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
