/*
 * lcd_display.h
 *
 *  Created on: May 17, 2021
 *      Author: Admin
 */

#ifndef INC_LCD_DISPLAY_H_
#define INC_LCD_DISPLAY_H_

#include "stm32h7xx_hal.h"

typedef enum
{
	E_LCD_MAIN_MENU,
    E_LCD_OVERVIEW,
    E_LCD_SYSTEM_SETTING,
    E_LCD_IO_SETTING,
	E_LCD_IO_SETTING_ANALOG,
	E_LCD_IO_SETTING_DIGITAL,
	E_LCD_IO_SETTING_SERIAL,
    E_LCD_SENSOR_STATUS,
    E_LCD_TRANSMISSION_SETTING,
    E_LCD_STORAGE_SETTING,
    E_LCD_LOG_FILE
}teLCDState;

void vMoveScreenUp(void);
void vMoveScreenDown(void);
void vLCDDisplay(void);
void vLCDDisplay(void);
void vChangeLCDState(uint8_t u8Key);

#endif /* INC_LCD_DISPLAY_H_ */
