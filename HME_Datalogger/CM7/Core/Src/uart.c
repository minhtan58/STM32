/*
 * uart.c
 *
 *  Created on: Jun 7, 2021
 *      Author: Admin
 */
#include "uart.h"

UART_HandleTypeDef huart8;
UART_HandleTypeDef huart1;

int __io_putchar (int ch) {
	uint8_t c = ch;
	HAL_UART_Transmit(&huart8, &c, 1, 100);
	return ch;
}

void Error_Handler_Uart(void)
{
	//Do nothing
}

void MX_UART8_Init(void)
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
		Error_Handler_Uart();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&huart8, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler_Uart();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&huart8, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler_Uart();
	}
	if (HAL_UARTEx_DisableFifoMode(&huart8) != HAL_OK)
	{
		Error_Handler_Uart();
	}
}

void MX_USART1_UART_Init(void)
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
		Error_Handler_Uart();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler_Uart();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler_Uart();
	}
	if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
	{
		Error_Handler_Uart();
	}
}
