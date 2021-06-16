/*
 * lcd.c
 *
 *  Created on: May 10, 2021
 *      Author: Admin
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "lcd.h"

#define CONVERT_POLISH

#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define fontbyte(x) pgm_read_byte(&cfont.font[x])

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#define LCD_CLS         0x01
#define LCD_HOME        0x02
#define LCD_ADDRINC     0x06
#define LCD_DISPLAYON   0x0C
#define LCD_DISPLAYOFF  0x08
#define LCD_CURSORON    0x0E
#define LCD_CURSORBLINK 0x0F
#define LCD_BASIC       0x30
#define LCD_EXTEND      0x34
#define LCD_GFXMODE     0x36
#define LCD_TXTMODE     0x34
#define LCD_STANDBY     0x01
#define LCD_SCROLL      0x03
#define LCD_SCROLLADDR  0x40
#define LCD_ADDR        0x80
#define LCD_LINE0       0x80
#define LCD_LINE1       0x90
#define LCD_LINE2       0x88
#define LCD_LINE3       0x98

#define SPI_SPEED (1000000UL)

#ifndef SCR_WD
#define SCR_WD  128
#define SCR_HT  64
#endif

//#define spi_enable  	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
//#define spi_disable 	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
//#define ALIGN_LEFT    0
//#define ALIGN_RIGHT  -1
//#define ALIGN_CENTER -2

struct _propFont
{
  const uint8_t* font;
  int8_t xSize;
  uint8_t ySize;
  uint8_t firstCh;
  uint8_t lastCh;
  uint8_t minCharWd;
  uint8_t minDigitWd;
};

extern SPI_HandleTypeDef hspi1;

uint8_t scr[SCR_WD*SCR_HT/8];
uint8_t scrWd = 128/8;
uint8_t scrHt = 64;
uint8_t csPin  = 10;

static uint8_t xstab[8] = {0xff,0x7f,0x3f,0x1f,0x0f,0x07,0x03,0x01};
static uint8_t xetab[8] = {0x80,0xc0,0xe0,0xf0,0xf8,0xfc,0xfe,0xff};
static uint8_t pattern[4] = {0xaa,0x55,0xaa,0x55};

const uint8_t ditherTab[4*17] = {
  0x00,0x00,0x00,0x00, // 0

  0x88,0x00,0x00,0x00, // 1
  0x88,0x00,0x22,0x00, // 2
  0x88,0x00,0xaa,0x00, // 3
  0xaa,0x00,0xaa,0x00, // 4
  0xaa,0x44,0xaa,0x00, // 5
  0xaa,0x44,0xaa,0x11, // 6
  0xaa,0x44,0xaa,0x55, // 7

  0xaa,0x55,0xaa,0x55, // 8

  0xaa,0xdd,0xaa,0x55, // 9
  0xaa,0xdd,0xaa,0x77, // 10
  0xaa,0xdd,0xaa,0xff, // 11
  0xaa,0xff,0xaa,0xff, // 12
  0xbb,0xff,0xaa,0xff, // 13
  0xbb,0xff,0xee,0xff, // 14
  0xbb,0xff,0xff,0xff, // 15

  0xff,0xff,0xff,0xff  // 16
};

bool (*isNumberFun)(uint8_t ch);

struct _propFont  cfont;
/*
uint8_t xSize;
uint8_t ySize;
uint8_t ySize8;
uint8_t firstCh;
uint8_t lastCh;
uint8_t minCharWd;
uint8_t minDigitWd;
*/
uint8_t cr;  // carriage return mode for printStr
uint8_t dualChar;
uint8_t invertCh;
uint8_t spacing = 1;

// ----------------------------------------------------------------
void spi_wb(uint8_t b)
{
	HAL_SPI_Transmit(&hspi1, &b, 1, 0xFFFFFFFF);
}

// ----------------------------------------------------------------
void init()
{
	scrWd = SCR_WD/8;
	scrHt = SCR_HT;

	isNumberFun = &isNumber;
	cr = 0;
	cfont.font = NULL;
	dualChar = 0;

	//spi_disable;

	//pinMode(csPin, OUTPUT);
	//digitalWrite(csPin, LOW);
	//SPI.begin();

	sendCmd(LCD_BASIC);
	HAL_Delay(2);
	sendCmd(LCD_BASIC);
	HAL_Delay(2);
	sendCmd(LCD_CLS);
	HAL_Delay(2);
	sendCmd(LCD_ADDRINC);
	HAL_Delay(2);
	sendCmd(LCD_DISPLAYON);
	HAL_Delay(2);
	setGfxMode(true);
}
// ----------------------------------------------------------------
void sendCmd(uint8_t b)
{
	//SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE3));
	//digitalWrite(csPin, HIGH);

	spi_enable;
	spi_wb(0xF8);
	spi_wb(b & 0xF0);
	spi_wb(b << 4);
	spi_disable;

	//digitalWrite(csPin, LOW);
	//SPI.endTransaction();
}
// ----------------------------------------------------------------

void sendData(uint8_t b)
{
	//SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE3));
	//digitalWrite(csPin, HIGH);

	spi_enable;
	spi_wb(0xFA);
	spi_wb(b & 0xF0);
	spi_wb(b << 4);
	spi_disable;

	//digitalWrite(csPin, LOW);
	//SPI.endTransaction();
}

void setCR(uint8_t _cr) { cr = _cr; }
void setInvert(uint8_t _inv) { invertCh = _inv; }
void setFontMinWd(uint8_t wd) { cfont.minCharWd = wd; }
void setCharMinWd(uint8_t wd) { cfont.minCharWd = wd; }
void setDigitMinWd(uint8_t wd) { cfont.minDigitWd = wd; }
void setIsNumberFun(bool (*fun)(uint8_t)) { isNumberFun=fun; }
// ----------------------------------------------------------------
// y =  0..63  -> buffer #0
// y = 64..127 -> buffer #1
void gotoXY(uint8_t x, uint8_t y)
{
  if(y>=32 && y<64) { y-=32; x+=8; } else
  if(y>=64 && y<64+32) { y-=32; x+=0; } else
  if(y>=64+32 && y<64+64) { y-=64; x+=8; }
  sendCmd(LCD_ADDR | y); // 6-bit (0..63)
  sendCmd(LCD_ADDR | x); // 4-bit (0..15)
}
// ----------------------------------------------------------------
// true - graphics mode
// false - text mode
void setGfxMode(bool mode)
{
  if(mode) {
    sendCmd(LCD_EXTEND);
    HAL_Delay(2);
    sendCmd(LCD_GFXMODE);
  } else {
    sendCmd(LCD_EXTEND);
    HAL_Delay(2);
    sendCmd(LCD_TXTMODE);
  }
}
// ----------------------------------------------------------------
void sleep(bool mode)
{
  //sendCmd(mode?LCD_STANDBY:LCD_EXTEND);
  if(mode) sendCmd(LCD_STANDBY);
  else { sendCmd(LCD_BASIC); sendCmd(LCD_EXTEND); sendCmd(LCD_GFXMODE); }
}
// ----------------------------------------------------------------
void switchBuf(uint8_t b)
{
  sendCmd(LCD_SCROLL);
  sendCmd(LCD_SCROLLADDR+32*b);
}
// ----------------------------------------------------------------
// Arduino Uno: 33-34ms -> 29fps
// optimized for full 128x64 frame
// 0 - buffer #0
// 1 - buffer #1
void display(int buf)
{
	uint8_t i,j,b;

	for(j = 0; j < 64/2; j++) {
		gotoXY(0, j+buf*64);

		//SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE3));
		//digitalWrite(csPin, HIGH);

		spi_enable;

		spi_wb(0xFA); // data

		for(i=0;i<16;i++) {  // 16 bytes from line #0+
			b=scr[i+j*16];
			spi_wb(b & 0xF0);  spi_wb(b << 4);
		}
		for(i=0;i<16;i++) {  // 16 bytes from line #32+
			b=scr[i+(j+32)*16];
			spi_wb(b & 0xF0);  spi_wb(b << 4);
		}

		spi_disable;
		//digitalWrite(csPin, LOW);
		//SPI.endTransaction();
	}
}
// ----------------------------------------------------------------
// copy only part of framebuffer
void copy(uint8_t x16, uint8_t y, uint8_t w16, uint8_t h, uint8_t buf)
{
	uint8_t i,j,b;
	for(j=0;j<h;j++) {
		gotoXY(x16,y+j+buf*64);

		//SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE3));
		//digitalWrite(csPin, HIGH);
		spi_enable;

		spi_wb(0xFA); // data

		for(i=0;i<w16;i++) {
			b=scr[(i+j*w16)*2+0];
			spi_wb(b & 0xF0);  spi_wb(b << 4);
			b=scr[(i+j*w16)*2+1];
			spi_wb(b & 0xF0);  spi_wb(b << 4);
		}

		spi_disable;
		//digitalWrite(csPin, LOW);
		//SPI.endTransaction();
	}
}
// ----------------------------------------------------------------
void cls()
{
	//for(int i=0;i<scrWd*scrHt;i++) scr[i]=0;
	//memset(scr,0,scrWd*scrHt);
	memset(scr,0,SCR_WD*SCR_HT/8);
}
// ----------------------------------------------------------------
void drawPixel(uint8_t x, uint8_t y, uint8_t col)
{
	if(x>=SCR_WD || y>=SCR_HT) return;
	switch(col) {
		case 1: scr[y*scrWd+x/8] |=   (0x80 >> (x&7)); break;
		case 0: scr[y*scrWd+x/8] &=  ~(0x80 >> (x&7)); break;
		case 2: scr[y*scrWd+x/8] ^=   (0x80 >> (x&7)); break;
	}
}
// ----------------------------------------------------------------
void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t col)
{
	int dx = abs(x1-x0);
	int dy = abs(y1-y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;

	while(1) {
		drawPixel(x0, y0, col);
		if(x0==x1 && y0==y1) return;
		int err2 = err+err;
		if(err2>-dy) { err-=dy; x0+=sx; }
		if(err2< dx) { err+=dx; y0+=sy; }
	}
}
// ----------------------------------------------------------------
void drawLineH(uint8_t x0, uint8_t x1, uint8_t y, uint8_t col)
{
	if(x1>x0) for(uint8_t x=x0; x<=x1; x++) drawPixel(x,y,col);
	else      for(uint8_t x=x1; x<=x0; x++) drawPixel(x,y,col);
}
// ----------------------------------------------------------------
void drawLineV(uint8_t x, uint8_t y0, uint8_t y1, uint8_t col)
{
	if(y1>y0) for(uint8_t y=y0; y<=y1; y++) drawPixel(x,y,col);
	else      for(uint8_t y=y1; y<=y0; y++) drawPixel(x,y,col);
}
// ----------------------------------------------------------------
// about 4x faster than regular drawLineV
void drawLineVfast(uint8_t x, uint8_t y0, uint8_t y1, uint8_t col)
{
	uint8_t mask = 0x80 >> (x&7);
	if(y1<y0) {
		mask=y0; y0=y1; y1=mask; // swap
	}
	mask = 0x80 >> (x&7);
	switch(col) {
		case 1: for(int y=y0; y<=y1; y++) scr[y*scrWd+x/8] |= mask;   break;
		case 0: for(int y=y0; y<=y1; y++) scr[y*scrWd+x/8] &= ~mask;  break;
		case 2: for(int y=y0; y<=y1; y++) scr[y*scrWd+x/8] ^= mask;   break;
	}
}
// ----------------------------------------------------------------
// limited to pattern #8
void drawLineVfastD(uint8_t x, uint8_t y0, uint8_t y1, uint8_t col)
{
	uint8_t mask = 0x80 >> (x&7);
	if(y1<y0) {
		mask=y0; y0=y1; y1=mask; // swap
	}
	if(((x&1)==1 && (y0&1)==0) ||( (x&1)==0 && (y0&1)==1)) y0++;
		mask = 0x80 >> (x&7);
	switch(col) {
		case 1: for(int y=y0; y<=y1; y+=2) scr[y*scrWd+x/8] |= mask;   break;
		case 0: for(int y=y0; y<=y1; y+=2) scr[y*scrWd+x/8] &= ~mask;  break;
		case 2: for(int y=y0; y<=y1; y+=2) scr[y*scrWd+x/8] ^= mask;   break;
	}
}
// ----------------------------------------------------------------
// about 40x faster than regular drawLineH
void drawLineHfast(uint8_t x0, uint8_t x1, uint8_t y, uint8_t col)
{
  int yadd=y*scrWd;
  int x8s,x8e;
  if(x1>=x0) {
    x8s=x0/8;
    x8e=x1/8;
  } else {
    x8s=x1; x1=x0; x0=x8s; // swap
    x8s=x1/8;
    x8e=x0/8;
  }
  switch(col) {
    case 1:
      if(x8s==x8e) scr[yadd+x8s]|=(xstab[x0&7] & xetab[x1&7]);
      else { scr[yadd+x8s]|=xstab[x0&7]; scr[yadd+x8e]|=xetab[x1&7]; }
      for(int x=x8s+1; x<x8e; x++) scr[yadd+x]=0xff;
      break;
    case 0:
      if(x8s==x8e) scr[yadd+x8s]&=~(xstab[x0&7] & xetab[x1&7]);
      else { scr[yadd+x8s]&=~xstab[x0&7]; scr[yadd+x8e]&=~xetab[x1&7]; }
      for(int x=x8s+1; x<x8e; x++) scr[yadd+x]=0x00;
      break;
    case 2:
      if(x8s==x8e) scr[yadd+x8s]^=(xstab[x0&7] & xetab[x1&7]);
      else { scr[yadd+x8s]^=xstab[x0&7]; scr[yadd+x8e]^=xetab[x1&7]; }
      for(int x=x8s+1; x<x8e; x++) scr[yadd+x]^=0xff;
      break;
  }
}
// ----------------------------------------------------------------
// dithered version
void drawLineHfastD(uint8_t x0, uint8_t x1, uint8_t y, uint8_t col)
{
  int yadd=y*scrWd;
  int x8s,x8e;
  if(x1>=x0) {
    x8s=x0/8;
    x8e=x1/8;
  } else {
    x8s=x1; x1=x0; x0=x8s; // swap
    x8s=x1/8;
    x8e=x0/8;
  }
  switch(col) {
    case 1:
      if(x8s==x8e) scr[yadd+x8s]|=(xstab[x0&7] & xetab[x1&7] & pattern[y&3]);
      else { scr[yadd+x8s]|=(xstab[x0&7] & pattern[y&3]); scr[yadd+x8e]|=(xetab[x1&7] & pattern[y&3]); }
      for(int x=x8s+1; x<x8e; x++) scr[yadd+x]=pattern[y&3];
      break;
    case 0:
      if(x8s==x8e) scr[yadd+x8s]&=~(xstab[x0&7] & xetab[x1&7] & pattern[y&3]);
      else { scr[yadd+x8s]&=~(xstab[x0&7] & pattern[y&3]); scr[yadd+x8e]&=~(xetab[x1&7] & pattern[y&3]); }
      for(int x=x8s+1; x<x8e; x++) scr[yadd+x]=~pattern[y&3];
      break;
    case 2:
      if(x8s==x8e) scr[yadd+x8s]^=(xstab[x0&7] & xetab[x1&7] & pattern[y&3]);
      else { scr[yadd+x8s]^=(xstab[x0&7] & pattern[y&3]); scr[yadd+x8e]^=(xetab[x1&7] & pattern[y&3]); }
      for(int x=x8s+1; x<x8e; x++) scr[yadd+x]^=pattern[y&3];
      break;
  }
}
// ----------------------------------------------------------------
void drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t col)
{
  if(x>=SCR_WD || y>=SCR_HT) return;
  uint8_t drawVright=1;
  if(x+w>SCR_WD) { w=SCR_WD-x; drawVright=0; }
  if(y+h>SCR_HT) h=SCR_HT-y; else drawLineHfast(x, x+w-1, y+h-1,col);
  drawLineHfast(x, x+w-1, y,col);
  drawLineVfast(x,    y+1, y+h-2,col);
  if(drawVright) drawLineVfast(x+w-1,y+1, y+h-2,col);
}
// ----------------------------------------------------------------
// dithered version (50% of brightness)
void drawRectD(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t col)
{
  if(x>=SCR_WD || y>=SCR_HT) return;
  uint8_t drawVright=1;
  if(x+w>SCR_WD) { w=SCR_WD-x; drawVright=0; }
  if(y+h>SCR_HT) h=SCR_HT-y; else drawLineHfastD(x, x+w-1, y+h-1,col);
  drawLineHfastD(x, x+w-1, y,col);
  drawLineVfastD(x,    y+1, y+h-2,col);
  if(drawVright) drawLineVfastD(x+w-1,y+1, y+h-2,col);
}
// ----------------------------------------------------------------
void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t col)
{
  if(x>=SCR_WD || y>=SCR_HT) return;
  if(x+w>SCR_WD) w=SCR_WD-x;
  if(y+h>SCR_HT) h=SCR_HT-y;
  for(int i=y;i<y+h;i++) drawLineHfast(x,x+w-1,i,col);
}
// ----------------------------------------------------------------
// dithered version (50% of brightness)
void fillRectD(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t col)
{
  if(x>=SCR_WD || y>=SCR_HT) return;
  if(x+w>=SCR_WD) w=SCR_WD-x;
  if(y+h>=SCR_HT) h=SCR_HT-y;
  for(int i=y;i<y+h;i++) drawLineHfastD(x,x+w-1,i,col);
}
// ----------------------------------------------------------------
// circle
void drawCircle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t col)
{
  int f = 1 - (int)radius;
  int ddF_x = 1;
  int ddF_y = -2 * (int)radius;
  int x = 0;
  int y = radius;

  drawPixel(x0, y0 + radius, col);
  drawPixel(x0, y0 - radius, col);
  drawPixel(x0 + radius, y0, col);
  drawPixel(x0 - radius, y0, col);

  while(x < y) {
    if(f >= 0) {
      y--; ddF_y += 2; f += ddF_y;
    }
    x++; ddF_x += 2; f += ddF_x;
    drawPixel(x0 + x, y0 + y, col);
    drawPixel(x0 - x, y0 + y, col);
    drawPixel(x0 + x, y0 - y, col);
    drawPixel(x0 - x, y0 - y, col);
    drawPixel(x0 + y, y0 + x, col);
    drawPixel(x0 - y, y0 + x, col);
    drawPixel(x0 + y, y0 - x, col);
    drawPixel(x0 - y, y0 - x, col);
  }
}
// ----------------------------------------------------------------
void fillCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t col)
{
  drawLineHfast(x0-r, x0-r+2*r+1, y0, col);
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    drawLineHfast(x0-x, x0-x+2*x+1, y0+y, col);
    drawLineHfast(x0-y, x0-y+2*y+1, y0+x, col);
    drawLineHfast(x0-x, x0-x+2*x+1, y0-y, col);
    drawLineHfast(x0-y, x0-y+2*y+1, y0-x, col);
  }
}
// ----------------------------------------------------------------
// dithered version (50% of brightness)
void fillCircleD(uint8_t x0, uint8_t y0, uint8_t r, uint8_t col)
{
  drawLineHfastD(x0-r, x0-r+2*r+1, y0, col);
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    drawLineHfastD(x0-x, x0-x+2*x+1, y0+y, col);
    drawLineHfastD(x0-y, x0-y+2*y+1, y0+x, col);
    drawLineHfastD(x0-x, x0-x+2*x+1, y0-y, col);
    drawLineHfastD(x0-y, x0-y+2*y+1, y0-x, col);
  }
}
// ----------------------------------------------------------------

void setDither(uint8_t s)
{
  if(s>16) return;
  pattern[0] = pgm_read_byte(ditherTab+s*4+0);
  pattern[1] = pgm_read_byte(ditherTab+s*4+1);
  pattern[2] = pgm_read_byte(ditherTab+s*4+2);
  pattern[3] = pgm_read_byte(ditherTab+s*4+3);
}
// ----------------------------------------------------------------
#define ALIGNMENT \
  if(x==-1) x = SCR_WD-w; \
  else if(x<0) x = (SCR_WD-w)/2; \
  if(x<0) x=0; \
  if(x>=SCR_WD || y>=SCR_HT) return 0; \
  if(x+w>SCR_WD) w = SCR_WD-x; \
  if(y+h>SCR_HT) h = SCR_HT-y
// ----------------------------------------------------------------

int drawBitmaps(const uint8_t *bmp, int x, uint8_t y, uint8_t w, uint8_t h)
{
  uint8_t wdb = w;
  ALIGNMENT;
  uint8_t i,j8,d,b,ht8=(h+7)/8;
  for(j8=0; j8<ht8; j8++) {
    for(i=0; i<w; i++) {
    	uint8_t mask = 0x80 >> ((x+i)&7);
      d = pgm_read_byte(bmp+wdb*j8+i);
      int lastbit = h - j8 * 8;
      if (lastbit > 8) lastbit = 8;
      for(b=0; b<lastbit; b++) {
         if(d & 1) scr[(y+j8*8+b)*scrWd+(x+i)/8] |= mask;
         d>>=1;
      }
    }
  }
  return x+w;
}
// ----------------------------------------------------------------
int drawBitmap(const uint8_t *bmp, int x, uint8_t y)
{
  uint8_t w = pgm_read_byte(bmp+0);
  uint8_t h = pgm_read_byte(bmp+1);
  return drawBitmaps(bmp+2, x, y, w, h);
}
// ----------------------------------------------------------------
// text rendering
// ----------------------------------------------------------------
void setFont(const uint8_t* font)
{
  cfont.font = font;
  cfont.xSize = fontbyte(0);
  cfont.ySize = fontbyte(1);
  cfont.firstCh = fontbyte(2);
  cfont.lastCh  = fontbyte(3);
  cfont.minDigitWd = 0;
  cfont.minCharWd = 0;
  isNumberFun = &isNumber;
  spacing = 1;
  cr = 0;
  invertCh = 0;
}
// ----------------------------------------------------------------
int charWidth(uint8_t c, bool last)
{
  c = convertPolish(c);
  if (c < cfont.firstCh || c > cfont.lastCh)
      return c==' ' ?  1 + cfont.xSize/2 : 0;
  if (cfont.xSize > 0) return cfont.xSize;
  int ys8=(cfont.ySize+7)/8;
  int idx = 4 + (c-cfont.firstCh)*(-cfont.xSize*ys8+1);
  int wd = pgm_read_byte(cfont.font + idx);
  int wdL = 0, wdR = spacing; // default spacing before and behind char
  if((*isNumberFun)(c)) {
    if(cfont.minDigitWd>wd) {
      wdL = (cfont.minDigitWd-wd)/2;
      wdR += (cfont.minDigitWd-wd-wdL);
    }
  } else
  if(cfont.minCharWd>wd) {
    wdL = (cfont.minCharWd-wd)/2;
    wdR += (cfont.minCharWd-wd-wdL);
  }
  return last ? wd+wdL+wdR : wd+wdL+wdR-spacing;  // last!=0 -> get rid of last empty columns
}
// ----------------------------------------------------------------
int strWidth(char *str)
{
  int wd = 0;
  while (*str) wd += charWidth(*str++,true);
  return wd;
}
// ----------------------------------------------------------------
int printChar(int xpos, int ypos, unsigned char c)
{
  if(xpos >= SCR_WD || ypos >= SCR_HT)  return 0;
  int fht8 = (cfont.ySize + 7) / 8, wd, fwd = cfont.xSize;
  if(fwd < 0)  fwd = -fwd;

  c = convertPolish(c);
  if(c < cfont.firstCh || c > cfont.lastCh)  return c==' ' ?  1 + fwd/2 : 0;

  int x,y8,b,cdata = (c - cfont.firstCh) * (fwd*fht8+1) + 4;
  uint8_t d;
  wd = fontbyte(cdata++);
  int wdL = 0, wdR = spacing;
  if((*isNumberFun)(c)) {
    if(cfont.minDigitWd>wd) {
      wdL = (cfont.minDigitWd-wd)/2;
      wdR += (cfont.minDigitWd-wd-wdL);
    }
  } else
  if(cfont.minCharWd>wd) {
    wdL = (cfont.minCharWd-wd)/2;
    wdR += (cfont.minCharWd-wd-wdL);
  }
  if(xpos+wd+wdL+wdR>SCR_WD) wdR = MAX(SCR_WD-xpos-wdL-wd, 0);
  if(xpos+wd+wdL+wdR>SCR_WD) wd  = MAX(SCR_WD-xpos-wdL, 0);
  if(xpos+wd+wdL+wdR>SCR_WD) wdL = MAX(SCR_WD-xpos, 0);

  for(x=0; x<wd; x++) {
	  uint8_t mask = 0x80 >> ((xpos+x+wdL)&7);
	  for(y8=0; y8<fht8; y8++) {
      d = fontbyte(cdata+x*fht8+y8);
      int lastbit = cfont.ySize - y8 * 8;
      if (lastbit > 8) lastbit = 8;
      for(b=0; b<lastbit; b++) {
         if(d & 1) scr[(ypos+y8*8+b)*scrWd+(xpos+x+wdL)/8] |= mask;  //drawPixel(xpos+x, ypos+y8*8+b, 1);
         d>>=1;
      }
    }
  }
  return wd+wdR+wdL;
}
// ----------------------------------------------------------------
int printStr(int xpos, int ypos, char *str)
{
  //unsigned char ch;
  //int stl, row;
  int x = xpos;
  int y = ypos;
  int wd = strWidth(str);

  if(x==-1) // right = -1
    x = SCR_WD - wd;
  else if(x<0) // center = -2
    x = (SCR_WD - wd) / 2;
  if(x<0) x = 0; // left

  while(*str) {
    int wd = printChar(x,y,*str++);
    x+=wd;
    if(cr && x>=SCR_WD) {
      x=0;
      y+=cfont.ySize;
      if(y>SCR_HT) y = 0;
    }
  }
  if(invertCh) fillRect(xpos,x-1,y,y+cfont.ySize+1,2);
  return x;
}
// ----------------------------------------------------------------
void printTxt(uint8_t pos, char *str)
{
  sendCmd(LCD_BASIC);
  sendCmd(pos);
  while(*str) sendData(*str++);
}
// ----------------------------------------------------------------
void printTxt_16(uint8_t pos, uint16_t *signs)
{
  sendCmd(LCD_BASIC);
  sendCmd(pos);
  while(*signs) {  sendData(*signs>>8); sendData(*signs&0xff); signs++; }
}
// ----------------------------------------------------------------
bool isNumber(uint8_t ch)
{
  return isdigit(ch) || ch==' ';
}
// ---------------------------------
bool isNumberExt(uint8_t ch)
{
  return isdigit(ch) || ch=='-' || ch=='+' || ch=='.' || ch==' ';
}
// ----------------------------------------------------------------
unsigned char convertPolish(unsigned char _c)
{
  unsigned char pl, c = _c;
  if(c==196 || c==197 || c==195) {
	  dualChar = c;
    return 0;
  }
  if(dualChar) { // UTF8 coding
    switch(_c) {
      case 133: pl = 1+9; break; // '�'
      case 135: pl = 2+9; break; // '�'
      case 153: pl = 3+9; break; // '�'
      case 130: pl = 4+9; break; // '�'
      case 132: pl = dualChar==197 ? 5+9 : 1; break; // '�' and '�'
      case 179: pl = 6+9; break; // '�'
      case 155: pl = 7+9; break; // '�'
      case 186: pl = 8+9; break; // '�'
      case 188: pl = 9+9; break; // '�'
      //case 132: pl = 1; break; // '�'
      case 134: pl = 2; break; // '�'
      case 152: pl = 3; break; // '�'
      case 129: pl = 4; break; // '�'
      case 131: pl = 5; break; // '�'
      case 147: pl = 6; break; // '�'
      case 154: pl = 7; break; // '�'
      case 185: pl = 8; break; // '�'
      case 187: pl = 9; break; // '�'
      default:  return c; break;
    }
    dualChar = 0;
  } else
  switch(_c) {  // Windows coding
    case 165: pl = 1; break; // �
    case 198: pl = 2; break; // �
    case 202: pl = 3; break; // �
    case 163: pl = 4; break; // �
    case 209: pl = 5; break; // �
    case 211: pl = 6; break; // �
    case 140: pl = 7; break; // �
    case 143: pl = 8; break; // �
    case 175: pl = 9; break; // �
    case 185: pl = 10; break; // �
    case 230: pl = 11; break; // �
    case 234: pl = 12; break; // �
    case 179: pl = 13; break; // �
    case 241: pl = 14; break; // �
    case 243: pl = 15; break; // �
    case 156: pl = 16; break; // �
    case 159: pl = 17; break; // �
    case 191: pl = 18; break; // �
    default:  return c; break;
  }
  return pl+'~'+1;
}
