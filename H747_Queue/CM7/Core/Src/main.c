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
//#include <string.h>
//#include <stdio.h>

#include "main.h"
//#include "cmsis_os.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include "stdbool.h"
//#include "app_common.h"
#include "SerialLink.h"

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif


#define COMMAND_BUF_SIZE   20
/* Private macro -------------------------------------------------------------*/
typedef struct
{
    uint8_t  au8Buffer[20];
    uint8_t  u8Pos;
}tsCommand;

tsCommand sCommand;

/* Private variables ---------------------------------------------------------*/

//UART_HandleTypeDef huart8;
//UART_HandleTypeDef huart1;

uint8_t UART8_RxBuffer;
uint8_t UART1_RxBuffer;

uint8_t indexd = 0;
uint8_t buf[20];

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART8_Init(void);
static void MX_USART1_UART_Init(void);

int __io_putchar (int ch) {
	uint8_t c = ch;
	HAL_UART_Transmit(&huart8, &c, 1, HAL_MAX_DELAY);
	return ch;
}

/**************** TASK HANDLERS ***********************/
xTaskHandle Receiver_Handler;

/**************** QUEUE HANDLER ***********************/
xQueueHandle SimpleQueue;

/**************** TASK FUNCTIONS ***********************/
void Sender_HPT_Task (void *argument);
void Sender_LPT_Task (void *argument);
void Receiver_Task (void *argument);

uint8_t Rx_data;

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
	MX_UART8_Init();
	MX_USART1_UART_Init();

	HAL_UART_Receive_IT(&huart1, (uint8_t*)&UART1_RxBuffer, 1);
	//HAL_UART_Receive_IT(&huart8, (uint8_t*)&UART8_RxBuffer, 1);

	/************************* Create Integer Queue ****************************/

	SimpleQueue = xQueueCreate(150, sizeof (int));
	if (SimpleQueue == 0)  // Queue not created
	{
		printf("Unable to create Integer Queue\n\r");
	}
	else
	{
		printf("Integer Queue Created successfully\n\r");
	}

	/****************************** TASK RELATED ******************************/

	//xTaskCreate(Receiver_Task, "Receive", 128, NULL, 1, &Receiver_Handler);

	//HAL_UART_Receive_IT(&huart8, &Rx_data, 1);

	//vTaskStartScheduler();

	while (1)
	{

	}
}

void vProcessCommand(void)
{
	uint8_t Delimiters[] = " ";
	uint8_t *token = NULL;

    printf("Command string = '%s'\n", sCommand.au8Buffer);

    token = (uint8_t *)strtok((char *)sCommand.au8Buffer, (char *)Delimiters);

    printf("Command = %s\n", token);


    if (0 == stricmp((char*)token, "toggle"))
    {
    	printf("Toggle\n");
    }

    if (0 == stricmp((char*)token, "hello"))
    {
    	printf("Hello\n");
    }

    else if (0 == stricmp((char*)token, "steer"))
    {
    	printf("Steer\n");
    }
    else if (0 == stricmp((char*)token, "form"))
    {
    	printf("Form\n");
    }
    else if (0 == stricmp((char*)token, "find"))
    {
    	printf("Find\n");
    }

    memset(sCommand.au8Buffer, 0, 20);
    sCommand.u8Pos = 0;
}


void vProcessRxChar(uint8_t u8Char)
{

    if ((u8Char >= 'a' && u8Char <= 'z'))
    {
        u8Char -= ('a' - 'A');
    }
    if ((sCommand.u8Pos < COMMAND_BUF_SIZE)  && (u8Char != 0x0d))
    {
        sCommand.au8Buffer[sCommand.u8Pos++] = u8Char;
    }
    else if (sCommand.u8Pos >= COMMAND_BUF_SIZE)
    {
    	printf("OverFlow\n");
        memset(sCommand.au8Buffer, 0, COMMAND_BUF_SIZE);
        sCommand.u8Pos = 0;
    }

    if (u8Char == 0x0d)
    {
        vProcessCommand();
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	//HAL_UART_Receive_IT(&huart8, (uint8_t*)&UART8_RxBuffer, 1);
	//vProcessRxChar(UART1_RxBuffer);

	if(huart->Instance == huart1.Instance){
//		buf[indexd] = UART1_RxBuffer;
//		indexd++;
		APP_vProcessIncomingSerialCommands(UART1_RxBuffer);
		HAL_UART_Receive_IT(&huart1, (uint8_t*)&UART1_RxBuffer, 1);
		//HAL_UART_Receive_IT(&huart1, (uint8_t*)&UART1_RxBuffer, 1);
	}
//	 /* The xHigherPriorityTaskWoken parameter must be initialized to pdFALSE as
//	 it will get set to pdTRUE inside the interrupt safe API function if a
//	 context switch is required. */
//	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//
//	xQueueSendToFrontFromISR(SimpleQueue, &UART1_RxBuffer, &xHigherPriorityTaskWoken);
//
//	/* Pass the xHigherPriorityTaskWoken value into portEND_SWITCHING_ISR(). If
//	 xHigherPriorityTaskWoken was set to pdTRUE inside xSemaphoreGiveFromISR()
//	 then calling portEND_SWITCHING_ISR() will request a context switch. If
//	 xHigherPriorityTaskWoken is still pdFALSE then calling
//	 portEND_SWITCHING_ISR() will have no effect */
//
//	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

void Receiver_Task (void *argument)
{
	uint8_t u8RxByte;
	while (1)
	{
		if (xQueueReceive(SimpleQueue, &u8RxByte, portMAX_DELAY) != pdTRUE)
		{
			printf("Error in Receiving from Queue\n\r");
		}
		else
		{
			printf("Receiver data = %d\n\r",UART1_RxBuffer);
			APP_vProcessIncomingSerialCommands(u8RxByte);
		}
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

/**
  * @brief UART8 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART8_Init(void)
{

  /* USER CODE BEGIN UART8_Init 0 */

  /* USER CODE END UART8_Init 0 */

  /* USER CODE BEGIN UART8_Init 1 */

  /* USER CODE END UART8_Init 1 */
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
  /* USER CODE BEGIN UART8_Init 2 */

  /* USER CODE END UART8_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
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
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pins : PI12 PI13 PI14 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
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
