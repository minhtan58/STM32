/*
 * rtc.h
 *
 *  Created on: Jun 8, 2021
 *      Author: Admin
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#include "stm32h7xx_hal.h"

extern RTC_HandleTypeDef hrtc;

void Error_Handler_rtc(void);

void MX_RTC_Init(void);

#endif /* INC_RTC_H_ */
