/*
 * sd_card.h
 *
 *  Created on: Jun 11, 2021
 *      Author: Admin
 */

#ifndef INC_SD_CARD_H_
#define INC_SD_CARD_H_

#include "stm32h7xx_hal.h"

extern SD_HandleTypeDef hsd1;

void Sd_Card_Manager(void);
void MX_SDMMC1_SD_Init(void);

#endif /* INC_SD_CARD_H_ */
