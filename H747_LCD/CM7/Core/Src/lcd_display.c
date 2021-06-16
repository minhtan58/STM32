/*
 * lcd_display.c
 *
 *  Created on: May 5, 2021
 *      Author: Admin
 */
#include <stdlib.h>
#include <stdio.h>

#include "lcd_display.h"
#include "lcd_20x4.h"

#define MINIMUM_INDEX_OF_SCREEN     		1
#define MAXIMUM_LENGTH_OF_STRING    		255
#define MAXIMUM_NUMBER_OF_COLUMNS   		20
#define MAXIMUM_NUMBER_OF_ROWS      		18
#define MAXIMUM_LENGTH_OF_STRING            255
#define NUMBER_OF_CIE_MAIN_MENU_OPTION      6

teLCDState eLCDState = E_LCD_MAIN_MENU;

uint8_t u8MenuIndex = 0;

void vMoveScreenUp(void)
{
	if(u8MenuIndex < 3)u8MenuIndex++;
}

void vMoveScreenDown(void)
{
	if(u8MenuIndex > 0)u8MenuIndex--;
}

void vLCDDisplay(void)
{
	switch (eLCDState)
	{
		case E_LCD_MAIN_MENU:
		{
			lcd_clear();
			lcd_put_cur(0,0);
			lcd_send_string("Overview");
			lcd_put_cur(1,0);
			lcd_send_string("System setting");
			lcd_put_cur(2,0);
			lcd_send_string("I/O setting");
			lcd_put_cur(3,0);
			lcd_send_string("Sensor status");
			lcd_put_cur(u8MenuIndex,0);
			break;
		}
		case E_LCD_OVERVIEW:
		{
			lcd_clear();
			lcd_put_cur(0,0);
			lcd_send_string("Model name: D-SL21");
			lcd_put_cur(1,0);
			lcd_send_string("Station name: Hi");
			lcd_put_cur(2,0);
			lcd_send_string("Serial number: 202");
			lcd_put_cur(3,0);
			lcd_send_string("Voltage: 9.1V");
			lcd_put_cur(u8MenuIndex,0);
			break;
		}
		case E_LCD_SYSTEM_SETTING:
		{
			lcd_clear();
			lcd_put_cur(0,0);
			lcd_send_string("Name: _________");
			lcd_put_cur(1,0);
			lcd_send_string("ID: ___________");
			lcd_put_cur(2,0);
			lcd_send_string("IPAdd: 192.168");
			lcd_put_cur(3,0);
			lcd_send_string("Date: 5-5-2021");
			lcd_put_cur(u8MenuIndex,0);
			break;
		}
		case E_LCD_IO_SETTING:
		{
			lcd_clear();
			lcd_put_cur(0,0);
			lcd_send_string("I/O setting");
			lcd_put_cur(1,0);
			lcd_send_string("Setting Analog channel");
			lcd_put_cur(2,0);
			lcd_send_string("Setting Digital channel");
			lcd_put_cur(3,0);
			lcd_send_string("Setting Serial channel");
			lcd_put_cur(u8MenuIndex,1);
			break;
		}
		case E_LCD_IO_SETTING_ANALOG:
		{
			lcd_clear();
			lcd_put_cur(0,0);
			lcd_send_string("Setting analog");
			lcd_put_cur(1,0);
			lcd_send_string("AI1:      Enable");
			lcd_put_cur(2,0);
			lcd_send_string("AI2:      Enable");
			lcd_put_cur(3,0);
			lcd_send_string("AI3:      Enable");
			lcd_put_cur(u8MenuIndex,1);
			break;
		}
		case E_LCD_IO_SETTING_DIGITAL:
		{
			lcd_clear();
			lcd_put_cur(0,0);
			lcd_send_string("Setting digital");
			lcd_put_cur(1,0);
			lcd_send_string("AI1:      Enable");
			lcd_put_cur(2,0);
			lcd_send_string("AI2:      Enable");
			lcd_put_cur(3,0);
			lcd_send_string("AI3:      Enable");
			lcd_put_cur(u8MenuIndex,1);
			break;
		}
		case E_LCD_IO_SETTING_SERIAL:
		{
			lcd_clear();
			lcd_put_cur(0,0);
			lcd_send_string("Setting Serial");
			lcd_put_cur(1,0);
			lcd_send_string("COM4:    Enable");
			lcd_put_cur(2,0);
			lcd_send_string("SDI12:   Enable");
			lcd_put_cur(3,0);
			lcd_send_string("RS485:   Enable");
			lcd_put_cur(u8MenuIndex,1);
			break;
		}
		case E_LCD_SENSOR_STATUS:
		{
			lcd_clear();
			lcd_put_cur(0,0);
			lcd_send_string("Sensor status");
			lcd_put_cur(1,0);
			lcd_send_string("Temperature: 20C");
			lcd_put_cur(2,0);
			lcd_send_string("Humidity: 75%");
			lcd_put_cur(3,0);
			lcd_send_string("Date: 5-5-2021");
			lcd_put_cur(u8MenuIndex,1);
			break;
		}
		case E_LCD_TRANSMISSION_SETTING:
		{
			lcd_clear();
			lcd_put_cur(0,0);
			lcd_send_string("Transmission setting");
			lcd_put_cur(0,0);
			lcd_send_string("Cellular setting");
			lcd_put_cur(2,0);
			lcd_send_string("Ethernet setting");
			lcd_put_cur(3,0);
			lcd_send_string(" ");
			lcd_put_cur(u8MenuIndex,1);
			break;
		}
		case E_LCD_STORAGE_SETTING:
		{
			lcd_clear();
			lcd_put_cur(0,0);
			lcd_send_string("Storage setting");
			lcd_put_cur(1,0);
			lcd_send_string("Space free: 55%");
			lcd_put_cur(2,0);
			lcd_send_string("Storage: Forever");
			lcd_put_cur(3,0);
			lcd_send_string("USB: Enable");
			lcd_put_cur(u8MenuIndex,1);
			break;
		}
		case E_LCD_LOG_FILE:
		{
			lcd_clear();
			lcd_put_cur(1,1);
			lcd_send_string("Log file");
			lcd_put_cur(2,1);
			lcd_send_string("23/5 Setting change");
			lcd_put_cur(3,1);
			lcd_send_string("24/5 Setting change");
			lcd_put_cur(4,1);
			lcd_send_string("25/5 Setting change");
			lcd_put_cur(u8MenuIndex,1);
			break;
		}
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
						case 0:
						{
							eLCDState = E_LCD_OVERVIEW;
						}
						break;
						case 1:
						{
							eLCDState = E_LCD_SYSTEM_SETTING;
						}
						break;

						default:
							eLCDState = E_LCD_MAIN_MENU;
						break;
					}
					u8MenuIndex = 0;
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
                	u8MenuIndex = 0;
                    break;

                case GPIO_PIN_3:
                        vMoveScreenUp();
                    break;

                case GPIO_PIN_6:
                        vMoveScreenDown();
                    break;

                case GPIO_PIN_4:
                	u8MenuIndex = 0;
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
                	u8MenuIndex = 0;
                    break;

                case GPIO_PIN_3:
                        vMoveScreenUp();
                    break;

                case GPIO_PIN_6:
                        vMoveScreenDown();
                    break;

                case GPIO_PIN_4:
                	u8MenuIndex = 0;
					eLCDState = E_LCD_MAIN_MENU;
                    break;

                default:
                    break;
            }
            break;
        }
        case E_LCD_IO_SETTING:
        {
            switch(u8Key)
            {
                case GPIO_PIN_5:
                	u8MenuIndex = 0;
                    break;

                case GPIO_PIN_3:
                        vMoveScreenUp();
                    break;

                case GPIO_PIN_6:
                        vMoveScreenDown();
                    break;

                case GPIO_PIN_4:
                	u8MenuIndex = 0;
					eLCDState = E_LCD_MAIN_MENU;
                    break;

                default:
                    break;
            }
            break;
        }
        case E_LCD_IO_SETTING_ANALOG:
        {
            switch(u8Key)
            {
                case GPIO_PIN_5:
                	u8MenuIndex = 0;
                    break;

                case GPIO_PIN_3:
                        vMoveScreenUp();
                    break;

                case GPIO_PIN_6:
                        vMoveScreenDown();
                    break;

                case GPIO_PIN_4:
                	u8MenuIndex = 0;
					eLCDState = E_LCD_MAIN_MENU;
                    break;

                default:
                    break;
            }
            break;
        }
        case E_LCD_IO_SETTING_DIGITAL:
        {
            switch(u8Key)
            {
                case GPIO_PIN_5:
                	u8MenuIndex = 0;
                    break;

                case GPIO_PIN_3:
                        vMoveScreenUp();
                    break;

                case GPIO_PIN_6:
                        vMoveScreenDown();
                    break;

                case GPIO_PIN_4:
                	u8MenuIndex = 0;
					eLCDState = E_LCD_MAIN_MENU;
                    break;

                default:
                    break;
            }
            break;
        }
        case E_LCD_IO_SETTING_SERIAL:
        {
            switch(u8Key)
            {
                case GPIO_PIN_5:
                	u8MenuIndex = 0;
                    break;

                case GPIO_PIN_3:
                        vMoveScreenUp();
                    break;

                case GPIO_PIN_6:
                        vMoveScreenDown();
                    break;

                case GPIO_PIN_4:
                	u8MenuIndex = 0;
					eLCDState = E_LCD_MAIN_MENU;
                    break;

                default:
                    break;
            }
            break;
        }
        case E_LCD_SENSOR_STATUS:
        {
            switch(u8Key)
            {
                case GPIO_PIN_5:
                	u8MenuIndex = 0;
                    break;

                case GPIO_PIN_3:
                        vMoveScreenUp();
                    break;

                case GPIO_PIN_6:
                        vMoveScreenDown();
                    break;

                case GPIO_PIN_4:
                	u8MenuIndex = 0;
					eLCDState = E_LCD_MAIN_MENU;
                    break;

                default:
                    break;
            }
            break;
        }
        case E_LCD_TRANSMISSION_SETTING:
        {
            switch(u8Key)
            {
                case GPIO_PIN_5:
                	u8MenuIndex = 0;
                    break;

                case GPIO_PIN_3:
                        vMoveScreenUp();
                    break;

                case GPIO_PIN_6:
                        vMoveScreenDown();
                    break;

                case GPIO_PIN_4:
                	u8MenuIndex = 0;
					eLCDState = E_LCD_MAIN_MENU;
                    break;

                default:
                    break;
            }
            break;
        }
        case E_LCD_STORAGE_SETTING:
        {
            switch(u8Key)
            {
                case GPIO_PIN_5:
                	u8MenuIndex = 0;
                    break;

                case GPIO_PIN_3:
                        vMoveScreenUp();
                    break;

                case GPIO_PIN_6:
                        vMoveScreenDown();
                    break;

                case GPIO_PIN_4:
                	u8MenuIndex = 0;
					eLCDState = E_LCD_MAIN_MENU;
                    break;

                default:
                    break;
            }
            break;
        }
        case E_LCD_LOG_FILE:
        {
            switch(u8Key)
            {
                case GPIO_PIN_5:
                	u8MenuIndex = 0;
                    break;

                case GPIO_PIN_3:
                        vMoveScreenUp();
                    break;

                case GPIO_PIN_6:
                        vMoveScreenDown();
                    break;

                case GPIO_PIN_4:
                	u8MenuIndex = 0;
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
