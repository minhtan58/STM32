/*
 * SerialLink.c
 *
 *  Created on: Mar 10, 2021
 *      Author: Admin
 */

#include "SerialLink.h"
#include "app_common.h"

uint16_t u16PacketType = 0;
uint16_t u16PacketLength = 0;
uint8_t au8LinkRxBuffer[256];

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
        HAL_UART_Transmit(&huart1, &u8Data, 1, 1000);
    }
    HAL_UART_Transmit(&huart1, &u8Data, 1, 1000);
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

void APP_vProcessIncomingSerialCommands(uint8_t u8RxByte)
{
	uint8_t u8Length = 0;
	uint8_t au8values[4];

    if(true == bSL_ReadMessage(&u16PacketType, &u16PacketLength, MAX_PACKET_SIZE, au8LinkRxBuffer, u8RxByte))
    {
    	switch(u16PacketType)
		{
			case (E_SL_MSG_PERMIT_JOINING_REQUEST):
			{
				printf("E_SL_MSG_PERMIT_JOINING_REQUEST\n\r");
			}
			break;

			case (E_SL_MSG_START_NETWORK):
			{
				printf("E_SL_MSG_START_NETWORK\n\r");
				u8Length = 0;
                ZNC_BUF_U8_UPD ( &au8values[ u8Length ], 9, u8Length );
                ZNC_BUF_U8_UPD ( &au8values[ u8Length ], 9, u8Length );
                ZNC_BUF_U8_UPD ( &au8values[ u8Length ], 9, u8Length );
                ZNC_BUF_U8_UPD ( &au8values[ u8Length ], 9, u8Length );
                vSL_WriteMessage ( 0x8011,
                                   u8Length,
                                   au8values );
			}
			break;

			case (E_SL_MSG_GET_PERMIT_JOIN):
			{
				printf("E_SL_MSG_GET_PERMIT_JOIN\n\r");
				u8Length    =  0;
				ZNC_BUF_U8_UPD  ( &au8values [ 0 ], 1, u8Length );
				ZNC_BUF_U8_UPD  ( &au8values [ 1 ], 2, u8Length );
				ZNC_BUF_U16_UPD ( &au8values [ 2 ], 3, u8Length );
				vSL_WriteMessage ( E_SL_MSG_STATUS,
								   u8Length,
								   au8values );
			}
			break;
			default:
				printf("u16PacketType = %d \n\r",u16PacketType);
			break;
		}
    }
}
