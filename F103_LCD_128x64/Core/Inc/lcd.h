/*
 * lcd.h
 *
 *  Created on: May 10, 2021
 *      Author: Admin
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_


/*
 128x64 ST7920 connections (SPI)

 #01 GND  -> GND
 #02 VCC  -> VCC (5V)
 #04 RS   -> D10/CS or any pin
 #05 R/W  -> D11/MOSI
 #06 E    -> D13/SCK
 #15 PSB  -> GND (for SPI mode)
 #19 BLA  -> VCC or any pin via 300ohm
 #20 BLK  -> GND

*/
#include <stdbool.h>
#include "stm32f1xx_hal.h"

#define spi_enable  	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
#define spi_disable 	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

void init();
void sendCmd(uint8_t b);
void sendData(uint8_t b);
void switchBuf(uint8_t b);
void display(int buf);
void copy(uint8_t x16, uint8_t y, uint8_t w16, uint8_t h, uint8_t buf);
void gotoXY(uint8_t x, uint8_t y);
void sleep(bool mode);
void setGfxMode(bool mode);
void printTxt(uint8_t pos, char *str);
void printTxt_16(uint8_t pos, uint16_t *signs);

void cls();
void drawPixel(uint8_t x, uint8_t y, uint8_t col);
void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t col);
void drawLineH(uint8_t x0, uint8_t x1, uint8_t y, uint8_t col);
void drawLineV(uint8_t x,  uint8_t y0, uint8_t y1, uint8_t col);
void drawLineVfast(uint8_t x, uint8_t y0, uint8_t y1, uint8_t col);
void drawLineVfastD(uint8_t x, uint8_t y0, uint8_t y1, uint8_t col);
void drawLineHfast(uint8_t x0, uint8_t x1, uint8_t y, uint8_t col);
void drawLineHfastD(uint8_t x0, uint8_t x1, uint8_t y, uint8_t col);
void drawRect(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t col);
void drawRectD(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t col);
void fillRect(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t col);
void fillRectD(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t col);
void drawCircle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t col);
void fillCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t col);
void fillCircleD(uint8_t x0, uint8_t y0, uint8_t r, uint8_t col);
void setDither(uint8_t s);
int drawBitmaps(const uint8_t *bmp, int x, uint8_t y, uint8_t w, uint8_t h);
int drawBitmap(const uint8_t *bmp, int x, uint8_t y);

void setFont(const uint8_t* f);
void setCR(uint8_t _cr);
void setInvert(uint8_t _inv);
void setFontMinWd(uint8_t wd);
void setCharMinWd(uint8_t wd);
void setDigitMinWd(uint8_t wd);
int printChar(int xpos, int ypos, unsigned char c);
int printStr(int xpos, int ypos, char *str);
int charWidth(uint8_t _ch, bool last);
int strWidth(char *txt);
unsigned char convertPolish(unsigned char _c);
bool isNumber(uint8_t ch);
bool isNumberExt(uint8_t ch);
void setIsNumberFun(bool (*fun)(uint8_t));


#endif /* INC_LCD_H_ */
