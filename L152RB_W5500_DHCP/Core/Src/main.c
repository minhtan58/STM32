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
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "main.h"
#include "socket.h"
#include "wizchip_conf.h"
#include "w5500.h"
#include "dhcp.h"
#include "spi_cf.h"

#define HAL_TIMEOUT_VALUE 0xFFFFFFFF
#define countof(a) (sizeof(a) / sizeof(*(a)))
#define spi_enable  	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
#define spi_disable 	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);

#define Data_BuffSize   2048
#define MAX_DHCP_RET    3

uint8_t send_data = 32;
uint8_t UART1_TxBuffer[] = "I'm UART1\r\n";

UART_HandleTypeDef huart1;

void W5500_Network_Init(void);
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);

uint8_t socket_sn = 0;
uint8_t client_sn = 0;
uint8_t gDataBuff[Data_BuffSize];

wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc, 0x11, 0x11},
                            .ip = {192, 168, 2, 126},
                            .sn = {255, 255, 255, 0},
                            .gw = {192, 168, 2, 1},
                            .dns = {8, 8, 8, 8},
                            .dhcp = NETINFO_DHCP };

int __io_putchar (int ch) {
	uint8_t c = ch;
	HAL_UART_Transmit(&huart1, &c, 1, 100);
	//HAL_UART_Transmit_IT(&huart1, (uint8_t*)&UART1_TxBuffer, countof(UART1_TxBuffer)-1);
	return ch;
}

void W5500_Network_Init(void)
{
        uint8_t tmpstr[6] = {0,};
	ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);

	ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO);

	//Display Network Information use USART
	ctlwizchip(CW_GET_ID, (void*)tmpstr);
	printf("\r\n=== %s NET CONF ===\r\n", (char*)tmpstr);
	printf("MAC:%02X:%02X:%02X:%02X:%02X:%02X\r\n", gWIZNETINFO.mac[0], gWIZNETINFO.mac[1],
	gWIZNETINFO.mac[2],  gWIZNETINFO.mac[3], gWIZNETINFO.mac[4], gWIZNETINFO.mac[5]);
	printf("SIP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0], gWIZNETINFO.ip[1], gWIZNETINFO.ip[2], gWIZNETINFO.ip[3]);
	printf("GAR: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0], gWIZNETINFO.gw[1], gWIZNETINFO.gw[2], gWIZNETINFO.gw[3]);
	printf("SUB: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0], gWIZNETINFO.sn[1], gWIZNETINFO.sn[2], gWIZNETINFO.sn[3]);
	printf("SUB: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0], gWIZNETINFO.dns[1], gWIZNETINFO.dns[2], gWIZNETINFO.dns[3]);
	printf("==============================\r\n");

	wizchip_init(0, 0);

	wiz_NetTimeout w_NetTimeout;
	w_NetTimeout.retry_cnt = 50;
	w_NetTimeout.time_100us = 1000;
	wizchip_settimeout(&w_NetTimeout);
}

void my_ip_assign(void)
{
  getIPfromDHCP(gWIZNETINFO.ip);
  getGWfromDHCP(gWIZNETINFO.gw);
  getSNfromDHCP(gWIZNETINFO.sn);
  getDNSfromDHCP(gWIZNETINFO.dns);
  gWIZNETINFO.dhcp = NETINFO_DHCP;

  ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);
}

void my_ip_conflict(void)
{
  printf("CONFLICT IP from DHCP \r\n");
  while(1);
}

int main(void)
{
	HAL_Init();
	SystemClock_Config();

	MX_GPIO_Init();
	//MX_SPI1_Init();
	MX_USART1_UART_Init();

	uint8_t dhcp_ret = 0;

	printf("W5500_Init\n\r");

	W5500_Init();

	setSHAR(gWIZNETINFO.mac);

	DHCP_init(socket_sn, gDataBuff);

	reg_dhcp_cbfunc(my_ip_assign, my_ip_assign, my_ip_conflict);

	printf("W5500_OK\n\r");

	while (1)
	{
		//spi_enable;
		//HAL_SPI_Transmit_IT(&hspi1, &send_data, 1);
		//spi_disable;
		//send_data++;
		//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
		//printf("Verson: %d, %f\n\r",200,99.909);
		//HAL_Delay(1000);
	    switch (DHCP_run())
	    {
			case DHCP_IP_ASSIGN:
			case DHCP_IP_CHANGED:
			break;
			case DHCP_IP_LEASED:
			//Code cho chuong trinh thi viet vao day
			socket_sn = socket_sn;
			break;
			case DHCP_FAILED:
			dhcp_ret++;
			if (dhcp_ret > MAX_DHCP_RET)
			{
				dhcp_ret = 0;
				DHCP_stop(); //if restart, recall DHCP_init()
				W5500_Network_Init();
			}
			break;
			default:
			break;
	    }
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == huart1.Instance)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	*/
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the RCC Oscillators according to the specified parameters
	* in the RCC_OscInitTypeDef structure.
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
	RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
	Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
							  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
	Error_Handler();
	}
}

//static void MX_SPI1_Init(void)
//{
//	/* SPI1 parameter configuration*/
//	hspi1.Instance 						= SPI1;
//	hspi1.Init.Mode 					= SPI_MODE_MASTER;
//	hspi1.Init.Direction 				= SPI_DIRECTION_2LINES;
//	hspi1.Init.DataSize 				= SPI_DATASIZE_8BIT;
//	hspi1.Init.CLKPolarity 				= SPI_POLARITY_LOW;
//	hspi1.Init.CLKPhase 				= SPI_PHASE_1EDGE;
//	hspi1.Init.NSS 						= SPI_NSS_SOFT;
//	hspi1.Init.BaudRatePrescaler 		= SPI_BAUDRATEPRESCALER_8;
//	hspi1.Init.FirstBit 				= SPI_FIRSTBIT_MSB;
//	hspi1.Init.TIMode 					= SPI_TIMODE_DISABLE;
//	hspi1.Init.CRCCalculation 			= SPI_CRCCALCULATION_DISABLE;
//	hspi1.Init.CRCPolynomial 			= 10;
//
//	if (HAL_SPI_Init(&hspi1) != HAL_OK)
//	{
//	Error_Handler();
//	}
//}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{
	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK)
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

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
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
