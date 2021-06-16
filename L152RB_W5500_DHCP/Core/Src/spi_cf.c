#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spi_cf.h"
#include "dhcp.h"
#include "stm32l1xx_hal.h"

static uint8_t W5500_ReadByte(void);
static void W5500_WriteByte(uint8_t byte);
static void W5500_SPI_Init(void);
static void W5500_CS_Select(void);
static void W5500_CS_Deselect(void);
static void W5500_Mem_Init(void);

uint8_t receive_data;

SPI_HandleTypeDef hspi1;
uint8_t chipid[6];
/*
wiz_NetInfo gWIZNETINFO;
uint8_t mac[6] = {0x00, 0x08, 0xdc, 0x11, 0x11}; //<Source MAC Address>
uint8_t ip[4]  = {192, 168, 32, 123}; //<Source IP Address>
uint8_t sn[4]  = {255, 255, 255, 0}; //<Subnet Mask>
uint8_t gw[4]  = {192, 168, 32, 1}; //<Gateway IP Address>
uint8_t dns[4] = {8, 8, 8, 8}; //<DNS server IP Address>
*/


/****************SPI1-Master*******************
		 Connect W5500 to SPI1 follow:
                  PA4 --> W5500_NSS
		  PA5 --> W5500_SCK
		  PA6 --> W5500_MISO
		  PA7 --> W5500_MOSI
**********************************************/
static void W5500_SPI_Init(void)
{
	/* SPI1 parameter configuration*/
	hspi1.Instance 						= SPI1;
	hspi1.Init.Mode 					= SPI_MODE_MASTER;
	hspi1.Init.Direction 				= SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize 				= SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity 				= SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase 				= SPI_PHASE_1EDGE;
	hspi1.Init.NSS 						= SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler 		= SPI_BAUDRATEPRESCALER_8;
	hspi1.Init.FirstBit 				= SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode 					= SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation 			= SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial 			= 10;

	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		//Error_Handler();
	}
}

//Đăng kí một số chức năng trong file wizchip_conf.h
void reg_wizchip_cris_cbfunc(void(*cris_en)(void), void(*cris_ex)(void));
void reg_wizchip_cs_cbfunc(void(*cs_sel)(void), void(*cs_decel)(void));
void reg_wizchip_bus_cbfunc(iodata_t(*bus_rb)(uint32_t addr), void(*bus_wb)(uint32_t addr, iodata_t wb));
void reg_wizchip_spi_cbfunc(uint8_t(*spi_rb)(void), void(*spi_wb)(uint8_t wb));
void reg_wizchip_spiburst_cbfunc(void(*spi_rb)(uint8_t* pBuf, uint16_t len), void(*spi_wb)(uint8_t* pBuf, uint16_t len));

static void W5500_CS_Select(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET); //Chân select cần được đẩy về 0
}

static void W5500_CS_Deselect(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);  //Khi đẩy select lên 1 thì sẽ deactive W5500
}

static void W5500_WriteByte(uint8_t byte)
{
	 while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
	 HAL_SPI_Transmit(&hspi1, (uint8_t *) &byte, 1, 10);
}

static uint8_t W5500_ReadByte(void)
{
	uint8_t Dummy = 0xFF;
	uint8_t RxData;
	while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
	HAL_SPI_TransmitReceive(&hspi1,&Dummy,&RxData,1,10);
	return RxData;
}


static void W5500_Mem_Init(void)
{
	uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};

	if (ctlwizchip(CW_INIT_WIZCHIP, (void*)memsize) == -1)
	{
		printf("WIZCHIP Initialized fail.\r\n");
		return;
	}
}

//Khoi tao cai dat lien quan toi W5500
void W5500_Init(void)
{
	W5500_SPI_Init();
	
	for (int i=0x5FFF; i>0; i--); //Delay mot khoang thoi gian de khoi tao
	W5500_Mem_Init();
	reg_wizchip_cris_cbfunc(0, 0);
	reg_wizchip_cs_cbfunc(W5500_CS_Select, W5500_CS_Deselect);
	reg_wizchip_spi_cbfunc(W5500_ReadByte, W5500_WriteByte);
}

void delay_us(uint16_t time)
{
	uint16_t i,j;
	for(i = 0; i<time; i++)
	{
		j=32;
		while(j>1)j--;
	}
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if(hspi->Instance == hspi1.Instance)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
	}
}
