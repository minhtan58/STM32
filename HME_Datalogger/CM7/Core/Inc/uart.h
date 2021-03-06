/*
 * uart.h
 *
 *  Created on: Jun 7, 2021
 *      Author: Admin
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "stm32h7xx_hal.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart8;

void Error_Handler_Uart(void);
void MX_UART8_Init(void);
void MX_USART1_UART_Init(void);

#endif /* INC_UART_H_ */
