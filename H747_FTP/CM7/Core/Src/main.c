/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "main.h"
#include "wizchip_conf.h"
//#include "socket.h"
//#include "loopback.h"
#include "fatfs.h"
#include "ftpc.h"
//#include "ff.h"

#define HSEM_ID_0 			(0U) /* HW semaphore 0*/
#define spi_enable  		HAL_GPIO_WritePin(GPIOK, GPIO_PIN_1, GPIO_PIN_RESET);
#define spi_disable 		HAL_GPIO_WritePin(GPIOK, GPIO_PIN_1, GPIO_PIN_SET);
#define HAL_TIMEOUT_VALUE 	0xFFFFFFFF
#define countof(a) (sizeof(a) / sizeof(*(a)))

FATFS   	fatfs; 		/* FAT File System */
FRESULT 	fresult;  	/* FAT File Result */
FIL			myfile;		/* FILE Instance */

uint8_t write_data[] = "Hello! This is my friend.", byte_written = 0;
uint8_t read_data[100], byte_read;
uint8_t send_data = 32;
uint8_t headerUART8;
uint8_t msg_c;
uint8_t dat[50]={0,};

long ret = 0;

volatile unsigned long _millis;

SD_HandleTypeDef hsd1;

SPI_HandleTypeDef hspi5;

UART_HandleTypeDef huart8;

wiz_NetInfo netInfo = { .mac  =  {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef}, 	// Mac address
						.ip   =  {192, 168, 2, 198},         			// IP address
						.sn   =  {255, 255, 255, 0},         			// Subnet mask
						.dns  =  {8, 8, 8, 8},			  				// DNS address (google dns)
						.gw   =  {192, 168, 2, 1}, 						// Gateway address
						.dhcp =  NETINFO_STATIC};    					//Static IP configuration

uint8_t gFTPBUF[_MAX_SS_FTP];

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SDMMC1_SD_Init(void);
static void MX_SPI5_Init(void);

int __io_putchar (int ch) {
	uint8_t c = ch;
	HAL_UART_Transmit(&huart8, &c, 1, 1000);
	return ch;
}

void cs_sel()
{
	HAL_GPIO_WritePin(GPIOK, GPIO_PIN_1, GPIO_PIN_RESET);
}

void cs_desel()
{
	HAL_GPIO_WritePin(GPIOK, GPIO_PIN_1, GPIO_PIN_SET);
}

uint8_t spi_rb(void)
{
	uint8_t rbuf;
	HAL_SPI_Receive(&hspi5, &rbuf, 1, 0xFFFFFFFF);
	return rbuf;
}

void spi_wb(uint8_t b)
{
	HAL_SPI_Transmit(&hspi5, &b, 1, 0xFFFFFFFF);
}

void Sd_Card_Manager()
{
	if(BSP_SD_Init() == MSD_OK)
	{
		fresult = f_mount(&fatfs,"",1);

		/* Create File with name "stm32.txt" and file access is write */
		fresult = f_open(&myfile,"stm.txt",FA_CREATE_ALWAYS|FA_WRITE);

		byte_written = f_printf(&myfile,"%s",write_data);

		fresult = f_close(&myfile);
	}
}

void print_network_information(void)
{
	uint8_t tmpstr[6] = {0,};
	ctlwizchip(CW_GET_ID,(void*)tmpstr); // Get WIZCHIP name
	printf("\r\n=======================================\r\n");
	printf(" WIZnet chip:  %s \r\n", tmpstr);
	printf("=======================================\r\n");

	wiz_NetInfo gWIZNETINFO;
	wizchip_getnetinfo(&gWIZNETINFO);
	if (gWIZNETINFO.dhcp == NETINFO_STATIC)
		printf("STATIC IP\r\n");
	else
		printf("DHCP IP\r\n");
	printf("Mac address: %02x:%02x:%02x:%02x:%02x:%02x\n\r",gWIZNETINFO.mac[0],gWIZNETINFO.mac[1],gWIZNETINFO.mac[2],gWIZNETINFO.mac[3],gWIZNETINFO.mac[4],gWIZNETINFO.mac[5]);
	printf("IP address : %d.%d.%d.%d\n\r",gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
	printf("SM Mask	   : %d.%d.%d.%d\n\r",gWIZNETINFO.sn[0],gWIZNETINFO.sn[1],gWIZNETINFO.sn[2],gWIZNETINFO.sn[3]);
	printf("Gate way   : %d.%d.%d.%d\n\r",gWIZNETINFO.gw[0],gWIZNETINFO.gw[1],gWIZNETINFO.gw[2],gWIZNETINFO.gw[3]);
	printf("DNS Server : %d.%d.%d.%d\n\r",gWIZNETINFO.dns[0],gWIZNETINFO.dns[1],gWIZNETINFO.dns[2],gWIZNETINFO.dns[3]);
}

void Network_init(void) {
	uint8_t bufSize[] = {2, 2, 2, 2, 2, 2, 2, 2};

	reg_wizchip_cs_cbfunc(cs_sel, cs_desel);
	reg_wizchip_spi_cbfunc(spi_rb, spi_wb);
	//reg_wizchip_spiburst_cbfunc(spi_rb_burst, spi_wb_burst);

	wizchip_init(bufSize, bufSize);
	wizchip_setnetinfo(&netInfo);
}

//****************************FAT FS initialize: BEGIN
//static void put_rc (FRESULT rc)
//{
//	const char *p;
//	static const char str[] =
//		"OK\0" "DISK_ERR\0" "INT_ERR\0" "NOT_READY\0" "NO_FILE\0" "NO_PATH\0"
//		"INVALID_NAME\0" "DENIED\0" "EXIST\0" "INVALID_OBJECT\0" "WRITE_PROTECTED\0"
//		"INVALID_DRIVE\0" "NOT_ENABLED\0" "NO_FILE_SYSTEM\0" "MKFS_ABORTED\0" "TIMEOUT\0"
//		"LOCKED\0" "NOT_ENOUGH_CORE\0" "TOO_MANY_OPEN_FILES\0";
//	FRESULT i;
//
//	for (p = str, i = 0; i != rc && pgm_read_byte_near(p); i++) {
//		while(pgm_read_byte_near(p++));
//	}
//	printf("rc=%u FR_%s\r\n", rc, p);
//}

void ls_dir(char* path)
{
	DIR Dir;
	FILINFO _Finfo;
	BYTE res;
	long p1, p2;
	UINT s1, s2;
	//while (*ptr == ' ') ptr++;
	res = f_opendir(&Dir, path);
	if (res)
	{
		//put_rc(res);
		return;
	}
	p1 = s1 = s2 = 0;
#if _USE_LFN
	//Init buffer for LFN NAME (Without this LFN NAME not visible!!)
	//Also look here:
	/*
	 * http://microsin.net/programming/file-systems/fatfs-read-dir.html
	 * https://electronix.ru/forum/index.php?app=forums&module=forums&controller=topic&id=122267
	 */
    _Finfo.lfname = Lfname;
    _Finfo.lfsize = sizeof(Lfname);
#endif

	for(;;) {
		res = f_readdir(&Dir, &_Finfo);
		if ((res != FR_OK) || !_Finfo.fname[0]) break;
		if (_Finfo.fattrib & AM_DIR) {
			s2++;
		} else {
			s1++; p1 += _Finfo.fsize;
		}
		printf("%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  %s",
					(_Finfo.fattrib & AM_DIR) ? 'D' : '-',
					(_Finfo.fattrib & AM_RDO) ? 'R' : '-',
					(_Finfo.fattrib & AM_HID) ? 'H' : '-',
					(_Finfo.fattrib & AM_SYS) ? 'S' : '-',
					(_Finfo.fattrib & AM_ARC) ? 'A' : '-',
					(_Finfo.fdate >> 9) + 1980, (_Finfo.fdate >> 5) & 15, _Finfo.fdate & 31,
					(_Finfo.ftime >> 11), (_Finfo.ftime >> 5) & 63,
					_Finfo.fsize, &(_Finfo.fname[0]));
#if _USE_LFN
		for (p2 = strlen(_Finfo.fname); p2 < 14; p2++)
			xputc(' ');
		xprintf(PSTR("%s\r\n"), Lfname);
#else
		printf("\r\n");
#endif
	}
	f_closedir(&Dir);
}

void fatfs_head_file(const char * fn)
{
	FRESULT f_err_code;
	FIL fil_obj;
	//trying to open and read file..
	f_chdir("/");
	f_err_code = f_open(&fil_obj, fn,FA_READ);	//Open *fn - <index.htm> for reading
	if(f_err_code==0)
	{
		DWORD file_len = fil_obj.fptr;
		UINT br;
		uint8_t _buf[128] = {0, };
		printf("++Content <%s> = %lu bytes found on SDCARD\r\n", fn, file_len);
		printf("++Trying to read head file..\r\n");
		f_err_code = f_read(&fil_obj,&_buf[0], 128, &br);
		if(f_err_code == 0)
		{
			if(br < 128)
				_buf[br] = 0x0;
			else
				_buf[127] = 0x0;
			printf ("OK\r\n");
			printf("text contents reading %u bytes:\r\n", br);
			printf("%s", _buf);
		}
		else
		{
			printf ("ERROR ");
			//put_rc(f_err_code);
			printf("But anyway text contents:\r\n");
			printf("%s", _buf);
		}
		f_close(&fil_obj);
	}
	else
	{
		printf ("ERROR opening file <%s> ", fn);
		//put_rc(f_err_code);
	}
}

void fatfs_init(void)
{
//	if( disk_status (0) == STA_NOINIT )	// Initialise the SD Card here, before we do anything else.
//	{
//		if( disk_initialize (0) )		// If it didn't initialise, or the card is write protected, try again.
//		{
//			if( disk_initialize (0) )		// If it didn't initialise, or the card is write protected, then call it out.
//			{
//				printf("\r\nSDCard initialization failed..!\r\nPlease power cycle the SDCard.\r\nCheck write protect.\r\n");
//				printf("\r\nReboot the Board");
//				while(1)
//				{
//					HAL_Delay(1000);
//					printf(".");
//				}
//			}
//			else
//			{
//				printf("\r\nSDCard initialization OK\r\n");
//			}
//		}
//		else
//		{
//			printf("\r\nSDCard initialization OK\r\n");
//		}
		printf(">>FS MOUNT ");
		f_mount(&fatfs, (const TCHAR *)"", 1);
		//put_rc(f_mount(&fatfs, (const TCHAR *)"", 1));
		printf(">>GO ROOT DIRECTORY ");
		f_chdir((const TCHAR *)"/");
		//put_rc(f_chdir((const TCHAR *)"/") );

		printf ("\r\n\r\nSD-Card root file list:\r\n");
		printf ("===============================================\r\n");
		ls_dir("/");
		printf ("===============================================\r\n\r\n");

	//}
}

int main(void)
{
	int32_t timeout;

	timeout = 0xFFFF;
	while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
	if ( timeout < 0 )
	{
	Error_Handler();
	}

	HAL_Init();

	SystemClock_Config();

	__HAL_RCC_HSEM_CLK_ENABLE();

	HAL_HSEM_FastTake(HSEM_ID_0);

	HAL_HSEM_Release(HSEM_ID_0,0);

	timeout = 0xFFFF;
	while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0));
	if ( timeout < 0 )
	{
	Error_Handler();
	}

	MX_GPIO_Init();
	MX_SDMMC1_SD_Init();
	MX_SPI5_Init();
	MX_UART8_Init();
	MX_FATFS_Init();

	HAL_Delay(1000);

	Sd_Card_Manager();

	//fatfs_init();
	HAL_Delay(200);
	//fatfs_head_file("stm.txt");

	printf("DHCP IP \n\r");

	Network_init();

	HAL_Delay(200);

	print_network_information();

	HAL_Delay(5000);

	printf("FTP Init\r\n");

	ftpc_init(netInfo.ip);

	HAL_Delay(200);

	while (1)
	{
		ftpc_run(gFTPBUF);
	}
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if(hspi->Instance == hspi5.Instance)
	{
		HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_14);
		HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_15);
	}
}

void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /*!< Supply configuration update enable */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

/* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
                                 RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }
    /*
  Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
          (GPIO, SPI, FMC, QSPI ...)  when  operating at  high frequencies(please refer to product datasheet)
          The I/O Compensation Cell activation  procedure requires :
        - The activation of the CSI clock
        - The activation of the SYSCFG clock
        - Enabling the I/O Compensation Cell : setting bit[0] of register SYSCFG_CCCSR

          To do this please uncomment the following code
  */

  /*
  __HAL_RCC_CSI_ENABLE() ;

  __HAL_RCC_SYSCFG_CLK_ENABLE() ;

  HAL_EnableCompensationCell();
  */
}
/**
  * @brief SDMMC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDMMC1_SD_Init(void)
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

/**
  * @brief SPI5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI5_Init(void)
{
	/* SPI5 parameter configuration*/
//	hspi5.Instance = SPI5;
//	hspi5.Init.Mode = SPI_MODE_MASTER;
//	hspi5.Init.Direction = SPI_DIRECTION_2LINES;
//	hspi5.Init.DataSize = SPI_DATASIZE_4BIT;
//	hspi5.Init.CLKPolarity = SPI_POLARITY_LOW;
//	hspi5.Init.CLKPhase = SPI_PHASE_1EDGE;
//	hspi5.Init.NSS = SPI_NSS_SOFT;
//	hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
//	hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
//	hspi5.Init.TIMode = SPI_TIMODE_DISABLE;
//	hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//	hspi5.Init.CRCPolynomial = 0x0;
//	hspi5.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
//	hspi5.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
//	hspi5.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
//	hspi5.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
//	hspi5.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
//	hspi5.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
//	hspi5.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
//	hspi5.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
//	hspi5.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
//	hspi5.Init.IOSwap = SPI_IO_SWAP_DISABLE;
//	if (HAL_SPI_Init(&hspi5) != HAL_OK)
//	{
//	Error_Handler();
//	}

	hspi5.Instance               = SPI5;
	hspi5.Init.Mode              = SPI_MODE_MASTER;
	hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	hspi5.Init.Direction         = SPI_DIRECTION_2LINES;
	hspi5.Init.CLKPhase          = SPI_PHASE_1EDGE;
	hspi5.Init.CLKPolarity       = SPI_POLARITY_LOW;
	hspi5.Init.DataSize          = SPI_DATASIZE_8BIT;
	hspi5.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	hspi5.Init.TIMode            = SPI_TIMODE_DISABLE;
	hspi5.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	hspi5.Init.CRCPolynomial     = 7;
	hspi5.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;
	hspi5.Init.NSS               = SPI_NSS_SOFT;
	hspi5.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;
	hspi5.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;
	if (HAL_SPI_Init(&hspi5) != HAL_OK)
	{
	Error_Handler();
	}
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOK, GPIO_PIN_1, GPIO_PIN_SET);

  /*Configure GPIO pins : PK5 PK4 PK6 PK3 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

  /*Configure GPIO pins : PI12 PI13 PI14 PI15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pin : PJ5 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
