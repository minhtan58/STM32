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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "i2c-lcd.h"
#include "lcd_i2cModule.h"

I2C_HandleTypeDef hi2c1;

char buf[25];

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_I2C1_Init();

	/*
	LCD_i2cDeviceCheck();
	LCD_Init();
	LCD_BackLight(LCD_BL_ON);
	LCD_SetCursor(1,1);

	LCD_Send_String("string example str no slide", STR_SLIDE); //Example of sending string value.
	HAL_Delay(800);
	LCD_Clear();
	HAL_Delay(20);

	Using LCD_Write_Data() and LCD_Set_Command function
	uint8_t L = 0x4C; //Character L = upper bits:0100, lower bits:1100
	uint8_t C = 0x43; //Character C = upper bits:0100, lower bits:0011
	uint8_t D = 0x44; //Character D = upper bits:0100, lower bits:0100
	uint8_t Symbol = 0x23; //Symbol # = upper bits:0010, lower bits:0011

	LCD_Write_Data(Symbol);
	LCD_Write_Data(L);
	LCD_Write_Data(C);
	LCD_Write_Data(D);
	LCD_SetCursor(1,3);

	LCD_Send_String("string example str no slide", STR_NOSLIDE); //Example of sending string value.
	HAL_Delay(800);
	LCD_Clear();
	HAL_Delay(20);

	LCD_SetCursor(1,1);
	LCD_Send_String("1.line slide example", STR_SLIDE); //Example of sliding string value.
	HAL_Delay(800);
	LCD_Clear();
	HAL_Delay(20);
	LCD_SetCursor(2,1);
	LCD_Send_String("2.line slide example", STR_SLIDE); //Example of sliding string value.

	while (1)
	{

	}
	*/

	lcd_init ();

	lcd_send_cmd (0x80|0x00);
	sprintf (buf,"HELLO %d %d %d",0x8024,1,111);
	lcd_send_string(buf);
	HAL_Delay(1000);
	lcd_send_cmd (0x80|0x40);
	lcd_send_string("LCD 20x4 DEMO");
	HAL_Delay(1000);
	lcd_send_cmd (0x80|0x14);
	lcd_send_string("BY");
	HAL_Delay(1000);
	lcd_send_cmd (0x80|0x54);
	lcd_send_string("Tran Minh Tan");

	while (1)
	{
		for (int i=0; i<6; i++)
		{
		  lcd_send_cmd (0x1C);  // display shift right
		  HAL_Delay(1000);
		}

		for (int i=0; i<6; i++)
		{
		  lcd_send_cmd (0x18);  // display shift left
		  HAL_Delay(1000);
		}
	}
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_I2C1_Init(void)
{
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
	Error_Handler();
	}
}

static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

	/*Configure GPIO pin : PC13 */
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
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
