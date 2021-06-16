/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"


#define HSEM_ID_0 (0U) /* HW semaphore 0*/
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */
#define spi_enable  	HAL_GPIO_WritePin(GPIOK, GPIO_PIN_1, GPIO_PIN_RESET);
#define spi_disable 	HAL_GPIO_WritePin(GPIOK, GPIO_PIN_1, GPIO_PIN_SET);
/* Private variables ---------------------------------------------------------*/

SPI_HandleTypeDef hspi5;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI5_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */
uint8_t send_data = 32;
/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

	  int32_t timeout;

	  SCB_EnableICache();
	  SCB_EnableDCache();

	  timeout = 0xFFFF;
	  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
	  if ( timeout < 0 )
	  {
	    Error_Handler();
	  }

	  HAL_Init();

	  SystemClock_Config();

	  MX_GPIO_Init();
	  MX_SPI5_Init();

	  while (1)
	  {
		  spi_enable;
		  //HAL_Delay(100);
		  HAL_SPI_Transmit_IT(&hspi5, &send_data, 1);
		  //HAL_Delay(100);
		  //spi_disable;
		  send_data++;
			//spi_disable;
		  HAL_Delay(100);
	  }
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if(hspi->Instance == hspi5.Instance)
	{
		spi_disable;
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
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
}

/**
  * @brief SPI5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI5_Init(void)
{
//  hspi5.Instance = SPI5;
//  hspi5.Init.Mode = SPI_MODE_MASTER;
//  hspi5.Init.Direction = SPI_DIRECTION_2LINES;
//  hspi5.Init.DataSize = SPI_DATASIZE_4BIT;
//  hspi5.Init.CLKPolarity = SPI_POLARITY_LOW;
//  hspi5.Init.CLKPhase = SPI_PHASE_1EDGE;
//  hspi5.Init.NSS = SPI_NSS_HARD_OUTPUT;
//  hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
//  hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
//  hspi5.Init.TIMode = SPI_TIMODE_DISABLE;
//  hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//  hspi5.Init.CRCPolynomial = 0x0;
//  hspi5.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
//  hspi5.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
//  hspi5.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
//  hspi5.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
//  hspi5.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
//  hspi5.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
//  hspi5.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
//  hspi5.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
//  hspi5.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
//  hspi5.Init.IOSwap = SPI_IO_SWAP_DISABLE;

	 hspi5.Instance               = SPI5;
	 hspi5.Init.Mode              = SPI_MODE_MASTER;
	 hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	 hspi5.Init.Direction         = SPI_DIRECTION_2LINES;
	 hspi5.Init.CLKPhase          = SPI_PHASE_1EDGE;
	 hspi5.Init.CLKPolarity       = SPI_POLARITY_LOW;
	 hspi5.Init.DataSize          = SPI_DATASIZE_8BIT;
	 hspi5.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	 hspi5.Init.TIMode            = SPI_TIMODE_DISABLE;
	 hspi5.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	 hspi5.Init.CRCPolynomial     = 7;
	 hspi5.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;
	 hspi5.Init.NSS               = SPI_NSS_SOFT;
	 hspi5.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;
	 hspi5.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;
	 if (HAL_SPI_Init(&hspi5) != HAL_OK)
	 {
		Error_Handler();
	 }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOK, GPIO_PIN_1, GPIO_PIN_SET);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
	while(1) { ; }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
