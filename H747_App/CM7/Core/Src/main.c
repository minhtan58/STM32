/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "stdbool.h"
#include "main.h"

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif

#define SL_START_CHAR          0x01
#define SL_ESC_CHAR            0x02
#define SL_END_CHAR            0x03
#define MAX_PACKET_SIZE        256
#define COMMAND_BUF_SIZE   	   50
/* Private macro -------------------------------------------------------------*/
/* Macros take the value and put it into a buffer */
#define ZNC_BUF_U8_UPD( BUFFER, U8VALUE, LEN)    (  ( *( (uint8_t*)( ( BUFFER ) ) ) = ( ( ( ( uint8_t ) ( U8VALUE ) ) & 0xFF ) ) ) ,\
     ( ( LEN ) += sizeof( uint8_t ) ) )

#define ZNC_BUF_U64_UPD( BUFFER, U64VALUE, LEN)    ( ( *(uint8_t*) ( BUFFER ) = (uint8_t) ( ( ( ( uint64_t ) ( U64VALUE ) ) >> 56) & 0xFF)),\
    ( *(uint8_t*) ( ( BUFFER ) + 1) = (uint8_t) ( ( ( ( uint64_t ) ( U64VALUE ) ) >> 48) & 0xFF ) ),\
    ( *(uint8_t*) ( ( BUFFER ) + 2) = (uint8_t) ( ( ( ( uint64_t ) ( U64VALUE ) ) >> 40) & 0xFF ) ),\
    ( *(uint8_t*) ( ( BUFFER ) + 3) = (uint8_t) ( ( ( ( uint64_t ) ( U64VALUE ) ) >> 32) & 0xFF ) ),\
    ( *(uint8_t*) ( ( BUFFER ) + 4) = (uint8_t) ( ( ( ( uint64_t ) ( U64VALUE ) ) >> 24) & 0xFF ) ),\
    ( *(uint8_t*) ( ( BUFFER ) + 5) = (uint8_t) ( ( ( ( uint64_t ) ( U64VALUE ) ) >> 16) & 0xFF ) ),\
    ( *(uint8_t*) ( ( BUFFER ) + 6) = (uint8_t) ( ( ( ( uint64_t ) ( U64VALUE ) ) >>  8) & 0xFF ) ),\
    ( *(uint8_t*) ( ( BUFFER ) + 7) = (uint8_t) ( ( ( ( uint64_t ) ( U64VALUE ) ) & 0xFF ) ) ),\
    ( ( LEN ) += sizeof( uint64_t ) ) )

#define ZNC_BUF_U32_UPD( BUFFER, U32VALUE, LEN )     ( ( *( uint8_t* )( BUFFER )   =  ( uint8_t ) ( ( ( ( uint32_t ) ( U32VALUE ) ) >> 24 ) & 0xFF ) ),\
    ( *( uint8_t* )( ( BUFFER ) + 1) =  ( uint8_t ) ( ( ( ( uint32_t ) ( U32VALUE ) ) >> 16 ) & 0xFF ) ),\
    ( *( uint8_t* )( ( BUFFER ) + 2) =  ( uint8_t ) ( ( ( ( uint32_t ) ( U32VALUE ) ) >> 8 ) & 0xFF ) ),\
    ( *( uint8_t* )( ( BUFFER ) + 3) =  ( uint8_t ) ( ( ( ( uint32_t ) ( U32VALUE ) ) & 0xFF ) ) ) ,\
    ( ( LEN ) += sizeof ( uint32_t ) ) )


#define ZNC_BUF_U16_UPD( BUFFER, U16VALUE, LEN )     ( ( *( uint8_t* )( BUFFER )   =  ( uint8_t )  ( ( ( ( uint16_t)( U16VALUE ) ) >> 8 ) & 0xFF ) ),\
    ( *( uint8_t* ) ( ( BUFFER ) + 1 )  =  ( uint8_t )  ( ( ( ( uint16_t )( U16VALUE ) ) ) & 0xFF ) ) ,\
    ( ( LEN ) += sizeof( uint16_t ) ) )

#define ZNC_RTN_U16( BUFFER, i ) ( ( ( uint16_t ) (BUFFER)[ i ] << 8) |\
    ( ( uint16_t ) (BUFFER)[ i + 1 ] & 0xFF))\

#define countof(a) (sizeof(a) / sizeof(*(a)))

typedef struct
{
    uint8_t  au8Buffer[20];
    uint8_t  u8Pos;
}tsCommand;

typedef enum
{
    E_STATE_RX_WAIT_START,
    E_STATE_RX_WAIT_TYPEMSB,
    E_STATE_RX_WAIT_TYPELSB,
    E_STATE_RX_WAIT_LENMSB,
    E_STATE_RX_WAIT_LENLSB,
    E_STATE_RX_WAIT_CRC,
    E_STATE_RX_WAIT_DATA,
}teSL_RxState;

typedef enum
{
	E_SL_MSG_STATUS                				=   0x8000,
	E_SL_MSG_GET_PERMIT_JOIN   					=   0x0014,
	E_SL_MSG_START_NETWORK						=   0x0024,
	E_SL_MSG_PERMIT_JOINING_REQUEST    			=   0x0049,
	E_SL_MSG_AHI_GET_IEEE_ADD_RSP 				=   0x8404,
	E_SL_MSG_GET_PERMIT_JOIN_RESPONSE   		=   0x8014,
	E_SL_MSG_E_SL_MSG_SENSOR_STATUS_RESPONSE   	=   0x8111,
}teSL_MsgTypeRange;

tsCommand sCommand;

UART_HandleTypeDef huart8;
UART_HandleTypeDef huart1;

uint8_t UART8_RxBuffer;
uint8_t UART1_RxBuffer;

uint8_t indexd = 0;
//uint8_t buf[20];

uint16_t u16PacketType = 0;
uint16_t u16PacketLength = 0;
uint8_t au8LinkRxBuffer[256];
uint16_t u16TargetAddress;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART8_Init(void);
static void MX_USART1_UART_Init(void);

int __io_putchar (int ch) {
	uint8_t c = ch;
	HAL_UART_Transmit(&huart8, &c, 1, HAL_MAX_DELAY);
	return ch;
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
	/*Take HSEM */
	HAL_HSEM_FastTake(HSEM_ID_0);
	/*Release HSEM in order to notify the CPU2(CM4)*/
	HAL_HSEM_Release(HSEM_ID_0,0);
	/* wait until CPU2 wakes up from stop mode */
	timeout = 0xFFFF;
	while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0));
	if ( timeout < 0 )
	{
	Error_Handler();
	}

	MX_GPIO_Init();
	MX_UART8_Init();
	MX_USART1_UART_Init();

	HAL_UART_Receive_IT(&huart1, (uint8_t*)&UART1_RxBuffer, 1);

	while (1)
	{
		//printf("DHCP IP\r\n");
		//HAL_Delay(1000);
	}
}

uint8_t u8SL_CalculateCRC(uint16_t u16Type, uint16_t u16Length, uint8_t *pu8Data)
{
    int n;
    uint8_t u8CRC;

    u8CRC  = (u16Type   >> 0) & 0xff;
    u8CRC ^= (u16Type   >> 8) & 0xff;
    u8CRC ^= (u16Length >> 0) & 0xff;
    u8CRC ^= (u16Length >> 8) & 0xff;

    for(n = 0; n < u16Length; n++)
    {
        u8CRC ^= pu8Data[n];
    }

    return(u8CRC);
}

void vSL_TxByte(bool bSpecialCharacter, uint8_t u8Data)
{
    if(!bSpecialCharacter && u8Data < 0x10)
    {
        /* Send escape character and escape byte */
        u8Data ^= 0x10;
        HAL_UART_Transmit(&huart1, &u8Data, 1,HAL_MAX_DELAY);
    }
    HAL_UART_Transmit(&huart1, &u8Data, 1,HAL_MAX_DELAY);
}

void vSL_WriteMessage(uint16_t u16Type, uint16_t u16Length, uint8_t *pu8Data)
{
    int n;
    uint8_t u8CRC;

    u8CRC = u8SL_CalculateCRC(u16Type, u16Length, pu8Data);

    //printf("\nvSL_WriteMessage(%d, %d, %02x)", u16Type, u16Length, u8CRC);

    /* Send start character */
    vSL_TxByte(true, SL_START_CHAR);

    /* Send message type */
    vSL_TxByte(false, (u16Type >> 8) & 0xff);
    vSL_TxByte(false, (u16Type >> 0) & 0xff);

    /* Send message length */
    vSL_TxByte(false, (u16Length >> 8) & 0xff);
    vSL_TxByte(false, (u16Length >> 0) & 0xff);

    /* Send message checksum */
    vSL_TxByte(false, u8CRC);

    /* Send message payload */
    for(n = 0; n < u16Length; n++)
    {
        vSL_TxByte(false, pu8Data[n]);
    }

    /* Send end character */
    vSL_TxByte(true, SL_END_CHAR);
}

bool bSL_ReadMessage(uint16_t *pu16Type, uint16_t *pu16Length, uint16_t u16MaxLength, uint8_t *pu8Message, uint8_t u8Data)
{

    static teSL_RxState eRxState = E_STATE_RX_WAIT_START;
    static uint8_t u8CRC = 0;
    static uint16_t u16Bytes = 0;
    static bool bInEsc = false;

    switch(u8Data)
    {
        case SL_START_CHAR:
            // Reset state machine
            u16Bytes = 0;
            bInEsc = false;
            //printf("\nRX Start ");
            eRxState = E_STATE_RX_WAIT_TYPEMSB;
            break;

        case SL_ESC_CHAR:
            // Escape next character
            bInEsc = true;
            break;

        case SL_END_CHAR:
            // End message
            //printf("\nGot END");
            eRxState = E_STATE_RX_WAIT_START;
            if(*pu16Length < u16MaxLength)
            {
                if(u8CRC == u8SL_CalculateCRC(*pu16Type, *pu16Length, pu8Message))
                {
                    /* CRC matches - valid packet */
                    //printf("\nbSL_ReadMessage(%d, %d, %02x)", *pu16Type, *pu16Length, u8CRC);
                    return(true);
                }
            }
            //printf("\nCRC BAD");
            break;

        default:
            if(bInEsc)
            {
                /* Unescape the character */
                u8Data ^= 0x10;
                bInEsc = false;
            }
            //printf("\nData 0x%x", u8Data & 0xFF);

            switch(eRxState)
            {

            case E_STATE_RX_WAIT_START:
                break;

            case E_STATE_RX_WAIT_TYPEMSB:
                *pu16Type = (uint16_t)u8Data << 8;
                eRxState++;
                break;

            case E_STATE_RX_WAIT_TYPELSB:
                *pu16Type += (uint16_t)u8Data;
                //printf("\nType 0x%x", *pu16Type & 0xFFFF);
                eRxState++;
                break;

            case E_STATE_RX_WAIT_LENMSB:
                *pu16Length = (uint16_t)u8Data << 8;
                eRxState++;
                break;

            case E_STATE_RX_WAIT_LENLSB:
                *pu16Length += (uint16_t)u8Data;
                //printf("\nLength %d", *pu16Length);
                if(*pu16Length > u16MaxLength)
                {
                    //printf("\nLength > MaxLength");
                    eRxState = E_STATE_RX_WAIT_START;
                }
                else
                {
                    eRxState++;
                }
                break;

            case E_STATE_RX_WAIT_CRC:
                //printf("\nCRC %02x\n", u8Data);
                u8CRC = u8Data;
                eRxState++;
                break;

            case E_STATE_RX_WAIT_DATA:
                if(u16Bytes < *pu16Length)
                {
                    //printf( "%02x ", u8Data);
                    pu8Message[u16Bytes++] = u8Data;
                }
                break;
            }
            break;
    }
    return(false);
}

void Send_Uart (char *string)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)string, strlen (string), HAL_MAX_DELAY);
}


void APP_vProcessIncomingSerialCommands(uint8_t u8RxByte)
{
    if(true == bSL_ReadMessage(&u16PacketType, &u16PacketLength, MAX_PACKET_SIZE, au8LinkRxBuffer, u8RxByte))
    {
    	switch(u16PacketType)
		{
			case (E_SL_MSG_PERMIT_JOINING_REQUEST):
			{
				printf("E_SL_MSG_PERMIT_JOINING_REQUEST\n\r");
				//				u8Length    =  0;
				//				ZNC_BUF_U8_UPD  ( &au8values [ u8Length ], 8, u8Length );
				//				ZNC_BUF_U8_UPD  ( &au8values [ u8Length ], 8, u8Length );
				//				ZNC_BUF_U8_UPD  ( &au8values [ u8Length ], 8, u8Length );
				//				ZNC_BUF_U8_UPD  ( &au8values [ u8Length ], 8, u8Length );
				//				vSL_WriteMessage ( 0x8011,
				//								   u8Length,
				//								   au8values );
			}
			break;

			case (E_SL_MSG_START_NETWORK):
			{
				//printf("==E_SL_MSG_START_NETWORK==\n\r");
				//HAL_UART_Transmit(&huart1, (uint8_t*)&HeaderTxBuffer, countof(HeaderTxBuffer)-1, HAL_MAX_DELAY);
            	char *buf = malloc(30*sizeof(char));
            	sprintf (buf, "\r%d+%d+%d\n",0x8024,1,111);
            	Send_Uart(buf);
            	free(buf);
			}
			break;

			case (0x0092):
			{
				u16TargetAddress  =  ZNC_RTN_U16( au8LinkRxBuffer , 1);
				printf("==0x0092 : %d %d %d %d ==\n\r",au8LinkRxBuffer[0],au8LinkRxBuffer[3],au8LinkRxBuffer[4],u16TargetAddress);
				//HAL_UART_Transmit(&huart1, (uint8_t*)&HeaderTxBuffer, countof(HeaderTxBuffer)-1, HAL_MAX_DELAY);
//            	char *buf = malloc(30*sizeof(char));
//            	sprintf (buf, "\r%d+%d+%d\n",0x8024,3,444);
//            	Send_Uart(buf);
//            	free(buf);
			}
			break;

			case (E_SL_MSG_GET_PERMIT_JOIN):
			{
				//printf("==E_SL_MSG_GET_PERMIT_JOIN==\n\r");
            	char *buf = malloc(30*sizeof(char));
            	sprintf (buf, "\r%d+%d+%d\n",0x8024,2,123);
            	Send_Uart(buf);
            	free(buf);
			}
			break;
			default:
				printf("u16PacketType = %d \n\r",u16PacketType);
			break;
		}
    }
}

void vProcessCommand2(void)
{
	char str[] = "strtok needs to be called several times to split a string";
	char delim[] = " ";
	int i = 0;
	char *contro[20];
	char *ptr = strtok(str, delim);

	while(ptr != NULL)
	{
		printf("%s\n\r", ptr);
		contro[i] = ptr;
		i++;
		ptr = strtok(NULL, delim);
	}

	printf("%d\n\r", i);
	printf("%s\n\r", contro[5]);

    //memset(sCommand.au8Buffer, 0, COMMAND_BUF_SIZE);
    //sCommand.u8Pos = 0;
}

void vProcessCommand(void)
{
	char Delimiters[] = " ";
	//char *token = NULL;
	uint8_t i = 0;
	char *control[20];
	char *token = strtok((char *)sCommand.au8Buffer, Delimiters);

	while(token != NULL)
	{
		//printf("%s\n\r", token);
		control[i] = token;
		i++;
		token = strtok(NULL, Delimiters);
	}
	//printf("%s\n\r", control[0]);

    if (0 == stricmp(control[0], "1111"))
    {
    	printf("System Setting %s %s %s %s %s %s\n\r",control[1],control[2],control[3],control[4],control[5],control[6]);
    	char *buf = malloc(30*sizeof(char));
    	sprintf (buf, "\r%d+%d+%d\n",0x8024,1,123);
    	Send_Uart(buf);
    	free(buf);
    }

    if (0 == stricmp(control[0], "1000"))
    {
    	printf("u16PacketType = hello\n\r");
    	char *buf = malloc(30*sizeof(char));
    	sprintf (buf, "\r%d+%d+%d\n",0x8024,2,123);
    	Send_Uart(buf);
    	free(buf);
    }

    else if (0 == stricmp(control[0], "steer"))
    {
    	printf("u16PacketType = steer \n\r");
    }
    memset(sCommand.au8Buffer, 0, COMMAND_BUF_SIZE);
    sCommand.u8Pos = 0;
}

void vProcessRxChar(uint8_t u8Char)
{

//    if ((u8Char >= 'a' && u8Char <= 'z'))
//    {
//        u8Char -= ('a' - 'A');
//    }
    if ((sCommand.u8Pos < COMMAND_BUF_SIZE)  && (u8Char != 0x0d))
    {
        sCommand.au8Buffer[sCommand.u8Pos++] = u8Char;
    }
    else if (sCommand.u8Pos >= COMMAND_BUF_SIZE)
    {
        memset(sCommand.au8Buffer, 0, COMMAND_BUF_SIZE);
        sCommand.u8Pos = 0;
    }

    if (u8Char == 0x0d)
    {
    	//printf("Receive PacketType\n\r");
    	vProcessCommand();
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == huart1.Instance){
		//APP_vProcessIncomingSerialCommands(UART1_RxBuffer);
		vProcessRxChar(UART1_RxBuffer);
		HAL_UART_Receive_IT(&huart1, (uint8_t*)&UART1_RxBuffer, 1);
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
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
  * @brief UART8 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART8_Init(void)
{

  /* USER CODE BEGIN UART8_Init 0 */

  /* USER CODE END UART8_Init 0 */

  /* USER CODE BEGIN UART8_Init 1 */

  /* USER CODE END UART8_Init 1 */
  huart8.Instance = UART8;
  huart8.Init.BaudRate = 115200;
  huart8.Init.WordLength = UART_WORDLENGTH_8B;
  huart8.Init.StopBits = UART_STOPBITS_1;
  huart8.Init.Parity = UART_PARITY_NONE;
  huart8.Init.Mode = UART_MODE_TX_RX;
  huart8.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart8.Init.OverSampling = UART_OVERSAMPLING_16;
  huart8.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart8.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart8.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart8, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart8, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart8) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART8_Init 2 */

  /* USER CODE END UART8_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pins : PI12 PI13 PI14 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{

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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
