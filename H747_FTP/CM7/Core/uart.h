/*
 * uart.h
 *
 *  Created on: Nov 21, 2020
 *      Author: Admin
 */

#ifndef UART_H_
#define UART_H_

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "stm32h7xx_hal.h"

extern UART_HandleTypeDef huart8;

uint8_t headerUART8;

void MX_UART8_Init(void);

uint8_t uart8_receive();

#endif /* UART_H_ */
