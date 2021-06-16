/*
 * uart.c
 *
 *  Created on: Nov 21, 2020
 *      Author: Admin
 */
#include "uart.h"

UART_HandleTypeDef huart8;

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
		//Error_Handler();
	}

	if (HAL_UARTEx_SetTxFifoThreshold(&huart8, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		//Error_Handler();
	}

	if (HAL_UARTEx_SetRxFifoThreshold(&huart8, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		//Error_Handler();
	}

	if (HAL_UARTEx_DisableFifoMode(&huart8) != HAL_OK)
	{
		//Error_Handler();
	}
}

uint8_t uart8_receive()
{
	HAL_UART_Receive(&huart8, (uint8_t*)&headerUART8, 1, 0xFFFFFFFF);
	return headerUART8;
}
