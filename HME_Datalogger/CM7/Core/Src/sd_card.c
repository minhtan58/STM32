/*
 * sd_card.c
 *
 *  Created on: Jun 11, 2021
 *      Author: Admin
 */

#include <stdio.h>
#include "sd_card.h"
#include "fatfs.h"

SD_HandleTypeDef hsd1;

FATFS   	fatfs; 		/* FAT File System */
FRESULT 	fresult;  	/* FAT File Result */
FIL			myfile;		/* FILE Instance */

uint8_t write_data[] = "Hello! This is my friend.";

void MX_SDMMC1_SD_Init(void)
{
	hsd1.Instance = SDMMC1;
	hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
	hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
	hsd1.Init.BusWide = SDMMC_BUS_WIDE_1B;
	hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
	hsd1.Init.ClockDiv = 0;
	hsd1.Init.TranceiverPresent = SDMMC_TRANSCEIVER_NOT_PRESENT;
	if (HAL_SD_Init(&hsd1) != HAL_OK)
	{
	Error_Handler();
	}
}

void Sd_Card_Manager(void)
{
	printf("Sd_Card_Manager\r\n");
	if(BSP_SD_Init() == MSD_OK)
	{
		printf("===ok===\r\n");
		fresult = f_mount(&fatfs,"",1);

		fresult = f_open(&myfile,"stm.txt",FA_CREATE_ALWAYS|FA_WRITE);

		f_printf(&myfile,"%s",write_data);

		fresult = f_close(&myfile);
	}
}
