/*
 * lcd_display.c
 *
 *  Created on: May 5, 2021
 *      Author: Admin
 */
#include <stdlib.h>
#include <stdio.h>
#include "lcd_display.h"
#include "lcd_i2cModule.h"

#define MINIMUM_INDEX_OF_SCREEN     		1
#define MAXIMUM_LENGTH_OF_STRING    		255
#define MAXIMUM_NUMBER_OF_COLUMNS   		20
#define MAXIMUM_NUMBER_OF_ROWS      		18
#define MAXIMUM_LENGTH_OF_STRING            255
#define NUMBER_OF_CIE_MAIN_MENU_OPTION      6

teLCDState eLCDState = E_LCD_MAIN_MENU;

uint8_t u8MenuIndex = 1;

void vMoveScreenUp(void)
{
	if(u8MenuIndex < 4)
		u8MenuIndex++;
    //LCD_SetCursor(u8MenuIndex,1);
}

void vMoveScreenDown(void)
{
	if(u8MenuIndex >1)
		u8MenuIndex--;
    //LCD_SetCursor(u8MenuIndex,1);
}


void vLCDDisplay(void)
{
	switch (eLCDState)
	{
		case E_LCD_MAIN_MENU:
		{
			LCD_SetCursor(1,1);
			LCD_Send_String("Overview", STR_NOSLIDE);
			LCD_SetCursor(2,1);
			LCD_Send_String("System setting", STR_NOSLIDE);
			LCD_SetCursor(3,1);
			LCD_Send_String("I/O setting", STR_NOSLIDE);
			LCD_SetCursor(4,1);
			LCD_Send_String("Sensor status", STR_NOSLIDE);
			LCD_SetCursor(u8MenuIndex,1);
			break;
		}
		case E_LCD_OVERVIEW:
		{
			LCD_SetCursor(1,1);
			LCD_Send_String("Model name: D-SL21", STR_NOSLIDE);
			LCD_SetCursor(2,1);
			LCD_Send_String("Station name: Hi", STR_NOSLIDE);
			LCD_SetCursor(3,1);
			LCD_Send_String("Serial number: 202", STR_NOSLIDE);
			LCD_SetCursor(4,1);
			LCD_Send_String("Voltage: 9.1V", STR_NOSLIDE);
			LCD_SetCursor(u8MenuIndex,1);
			break;
		}
		case E_LCD_SYSTEM_SETTING:
		{
			//LCD_Clear();
			LCD_SetCursor(1,1);
			LCD_Send_String("Name: _________", STR_NOSLIDE);
			LCD_SetCursor(2,1);
			LCD_Send_String("ID: ___________", STR_NOSLIDE);
			LCD_SetCursor(3,1);
			LCD_Send_String("IPAdd: 192.168", STR_NOSLIDE);
			LCD_SetCursor(4,1);
			LCD_Send_String("Date: 5-5-2021", STR_NOSLIDE);
			LCD_SetCursor(u8MenuIndex,1);
			break;
		}
//		case E_LCD_IO_SETTING:
//		{
//			LCD_SetCursor(1,1);
//			LCD_Send_String("I/O setting", STR_NOSLIDE);
//			LCD_SetCursor(2,1);
//			LCD_Send_String("Setting Analog channel", STR_NOSLIDE);
//			LCD_SetCursor(3,1);
//			LCD_Send_String("Setting Digital channel", STR_NOSLIDE);
//			LCD_SetCursor(4,1);
//			LCD_Send_String("Setting Serial channel", STR_NOSLIDE);
//			LCD_SetCursor(u8MenuIndex,1);
//			break;
//		}
//
//		case E_LCD_IO_SETTING_ANALOG:
//		{
//			LCD_SetCursor(1,1);
//			LCD_Send_String("Setting analog", STR_NOSLIDE);
//			LCD_SetCursor(2,1);
//			LCD_Send_String("AI1:      Enable", STR_NOSLIDE);
//			LCD_SetCursor(3,1);
//			LCD_Send_String("AI2:      Enable", STR_NOSLIDE);
//			LCD_SetCursor(4,1);
//			LCD_Send_String("AI3:      Enable", STR_NOSLIDE);
//			LCD_SetCursor(u8MenuIndex,1);
//			break;
//		}
//		case E_LCD_IO_SETTING_DIGITAL:
//		{
//			LCD_SetCursor(1,1);
//			LCD_Send_String("Setting digital", STR_NOSLIDE);
//			LCD_SetCursor(2,1);
//			LCD_Send_String("AI1:      Enable", STR_NOSLIDE);
//			LCD_SetCursor(3,1);
//			LCD_Send_String("AI2:      Enable", STR_NOSLIDE);
//			LCD_SetCursor(4,1);
//			LCD_Send_String("AI3:      Enable", STR_NOSLIDE);
//			LCD_SetCursor(u8MenuIndex,1);
//			break;
//		}
//		case E_LCD_IO_SETTING_SERIAL:
//		{
//			LCD_SetCursor(1,1);
//			LCD_Send_String("Setting Serial", STR_NOSLIDE);
//			LCD_SetCursor(2,1);
//			LCD_Send_String("COM4:    Enable", STR_NOSLIDE);
//			LCD_SetCursor(3,1);
//			LCD_Send_String("SDI12:   Enable", STR_NOSLIDE);
//			LCD_SetCursor(4,1);
//			LCD_Send_String("RS485:   Enable", STR_NOSLIDE);
//			LCD_SetCursor(u8MenuIndex,1);
//			break;
//		}
//		case E_LCD_SENSOR_STATUS:
//		{
//			LCD_SetCursor(1,1);
//			LCD_Send_String("Name: _________", STR_NOSLIDE);
//			LCD_SetCursor(2,1);
//			LCD_Send_String("ID: ___________", STR_NOSLIDE);
//			LCD_SetCursor(3,1);
//			LCD_Send_String("IPAdd: 192.168", STR_NOSLIDE);
//			LCD_SetCursor(4,1);
//			LCD_Send_String("Date: 5-5-2021", STR_NOSLIDE);
//			LCD_SetCursor(u8MenuIndex,1);
//			break;
//		}
//		case E_LCD_TRANSMISSION_SETTING:
//		{
//			LCD_SetCursor(1,1);
//			LCD_Send_String("Name: _________", STR_NOSLIDE);
//			LCD_SetCursor(2,1);
//			LCD_Send_String("ID: ___________", STR_NOSLIDE);
//			LCD_SetCursor(3,1);
//			LCD_Send_String("IPAdd: 192.168", STR_NOSLIDE);
//			LCD_SetCursor(4,1);
//			LCD_Send_String("Date: 5-5-2021", STR_NOSLIDE);
//			LCD_SetCursor(u8MenuIndex,1);
//			break;
//		}
//		case E_LCD_STORAGE_SETTING:
//		{
//			LCD_SetCursor(1,1);
//			LCD_Send_String("Name: _________", STR_NOSLIDE);
//			LCD_SetCursor(2,1);
//			LCD_Send_String("ID: ___________", STR_NOSLIDE);
//			LCD_SetCursor(3,1);
//			LCD_Send_String("IPAdd: 192.168", STR_NOSLIDE);
//			LCD_SetCursor(4,1);
//			LCD_Send_String("Date: 5-5-2021", STR_NOSLIDE);
//			LCD_SetCursor(u8MenuIndex,1);
//			break;
//		}
//		case E_LCD_LOG_FILE:
//		{
//			LCD_SetCursor(1,1);
//			LCD_Send_String("Name: _________", STR_NOSLIDE);
//			LCD_SetCursor(2,1);
//			LCD_Send_String("ID: ___________", STR_NOSLIDE);
//			LCD_SetCursor(3,1);
//			LCD_Send_String("IPAdd: 192.168", STR_NOSLIDE);
//			LCD_SetCursor(4,1);
//			LCD_Send_String("Date: 5-5-2021", STR_NOSLIDE);
//			LCD_SetCursor(u8MenuIndex,1);
//			break;
//		}
		default:
		break;
	}
}

void vChangeLCDState(uint8_t u8Key)
{
    switch (eLCDState)
    {
        case E_LCD_MAIN_MENU:
        {
            switch(u8Key)
            {
                case GPIO_PIN_5:
					switch(u8MenuIndex)
					{
						case E_LCD_OVERVIEW:
						{
							eLCDState = E_LCD_OVERVIEW;
						}
						break;
						case E_LCD_SYSTEM_SETTING:
						{
							eLCDState = E_LCD_SYSTEM_SETTING;
						}
						break;

						default:
							eLCDState = E_LCD_MAIN_MENU;
						break;
					}
					u8MenuIndex = 1;
                    break;

                case GPIO_PIN_3:
                        vMoveScreenUp();
                    break;

                case GPIO_PIN_6:
                        vMoveScreenDown();
                    break;

                case GPIO_PIN_4:
                    break;
                default:
                    break;
            }
            break;
        }
        case E_LCD_OVERVIEW:
        {
            switch(u8Key)
            {
                case GPIO_PIN_5:
                	u8MenuIndex = 1;
                    break;

                case GPIO_PIN_6:
                	vMoveScreenUp();
                    break;

                case GPIO_PIN_3:
                	vMoveScreenUp();
                	break;

                case GPIO_PIN_4:
					eLCDState = E_LCD_MAIN_MENU;
                    break;

                default:
                    break;
            }
            break;
        }
        case E_LCD_SYSTEM_SETTING:
        {
            switch(u8Key)
            {
                case GPIO_PIN_5:
                	u8MenuIndex = 1;
                    break;

                case GPIO_PIN_6:
                	vMoveScreenUp();
                    break;

                case GPIO_PIN_3:
                	vMoveScreenUp();
                	break;

                case GPIO_PIN_4:
					eLCDState = E_LCD_MAIN_MENU;
                    break;

                default:
                    break;
            }
            break;
        }
    }
    vLCDDisplay();
}

