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
#include "cmsis_os.h"
#include "fatfs.h"
#include "mbedtls.h"

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif

#define COMMAND_BUF_SIZE   	   50

typedef struct
{
    uint8_t  au8Buffer[20];
    uint8_t  u8Pos;
}tsCommand;

tsCommand sCommand;

QSPI_HandleTypeDef hqspi;

SD_HandleTypeDef hsd1;

UART_HandleTypeDef huart8;
UART_HandleTypeDef huart1;

//osThreadId defaultTaskHandle;
xQueueHandle SimpleQueue;

uint8_t UART1_RxBuffer;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_QUADSPI_Init(void);
static void MX_UART8_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SDMMC1_SD_Init(void);
void StartDefaultTask(void const * argument);
void APP_taskSerial(void const *argument);
void vProcessRxChar(uint8_t u8Char);

int __io_putchar (int ch) {
	uint8_t c = ch;
	HAL_UART_Transmit(&huart8, &c, 1, HAL_MAX_DELAY);
	return ch;
}

int main(void)
{
	int32_t timeout;

	SCB_EnableDCache();

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
	MX_USART1_UART_Init();
	MX_SDMMC1_SD_Init();
	MX_FATFS_Init();
	MX_MBEDTLS_Init();

	HAL_UART_Receive_IT(&huart1, (uint8_t*)&UART1_RxBuffer, 1);

	osThreadDef(SerialTask, APP_taskSerial, osPriorityNormal, 0, 128);
	osThreadCreate(osThread(SerialTask), NULL);

	//osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
	//osThreadCreate(osThread(defaultTask), NULL);

	osKernelStart();

	while (1)
	{

	}
}

void APP_taskSerial(void const *argument)
{
	uint8_t u8RxByte;
	while (1)
	{
//		printf("Receive PacketType\n\r");
//		osDelay(1000)
		if (xQueueReceive(SimpleQueue, &u8RxByte, portMAX_DELAY) == pdTRUE)
		{
			vProcessRxChar(u8RxByte);
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == huart1.Instance){
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		xQueueSendToFrontFromISR(SimpleQueue, &UART1_RxBuffer, &xHigherPriorityTaskWoken);
		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);

		HAL_UART_Receive_IT(&huart1, (uint8_t*)&UART1_RxBuffer, 1);
	}
}

void vProcessCommand(void)
{
	char Delimiters[] = " ";
	char i = 0;
	char *control[20];
	char *token = strtok((char *)sCommand.au8Buffer, Delimiters);

	while(token != NULL)
	{
		printf("%s\n\r", token);
		control[i] = token;
		i++;
		token = strtok(NULL, Delimiters);
	}
	printf("%s\n\r", control[0]);

    if (0 == stricmp(control[0], "1111"))
    {
    	printf("u16PacketType =  toggle\n\r");
    }

    if (0 == stricmp(control[0], "hello"))
    {
    	printf("u16PacketType = hello\n\r");
    }

    else if (0 == stricmp(control[0], "steer"))
    {
    	printf("u16PacketType = steer \n\r");
    }
    memset(sCommand.au8Buffer, 0, COMMAND_BUF_SIZE);
    sCommand.u8Pos = 0;
}

void vProcessRxChar(uint8_t u8Char)
{
    if ((sCommand.u8Pos < COMMAND_BUF_SIZE)  && (u8Char != 0x0d))
    {
        sCommand.au8Buffer[sCommand.u8Pos++] = u8Char;
    }
    else if (sCommand.u8Pos >= COMMAND_BUF_SIZE)
    {
        memset(sCommand.au8Buffer, 0, COMMAND_BUF_SIZE);
        sCommand.u8Pos = 0;
    }

    if (u8Char == 0x0d)
    {
    	//printf("Receive PacketType\n\r");
    	vProcessCommand();
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
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_HSI |RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 15;
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

  /*
  __HAL_RCC_CSI_ENABLE() ;

  __HAL_RCC_SYSCFG_CLK_ENABLE() ;

  HAL_EnableCompensationCell();
  */
}

static void MX_QUADSPI_Init(void)
{
	hqspi.Instance = QUADSPI;
	hqspi.Init.ClockPrescaler = 255;
	hqspi.Init.FifoThreshold = 1;
	hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
	hqspi.Init.FlashSize = 1;
	hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
	hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
	hqspi.Init.FlashID = QSPI_FLASH_ID_1;
	hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
	if (HAL_QSPI_Init(&hqspi) != HAL_OK)
	{
	Error_Handler();
	}
}

static void MX_SDMMC1_SD_Init(void)
{
	hsd1.Instance = SDMMC1;
	hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
	hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
	hsd1.Init.BusWide = SDMMC_BUS_WIDE_1B;
	hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
	hsd1.Init.ClockDiv = 0;
	hsd1.Init.TranceiverPresent = SDMMC_TRANSCEIVER_NOT_PRESENT;
	if (HAL_SD_Init(&hsd1) != HAL_OK)
	{
		Error_Handler();
	}
}

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

static void MX_USART1_UART_Init(void)
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
	{
		Error_Handler();
	}
}

static void MX_GPIO_Init(void)
{
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOJ_CLK_ENABLE();
}

void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
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
