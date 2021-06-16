/*
 * SerailLink.h
 *
 *  Created on: Mar 10, 2021
 *      Author: Admin
 */

#ifndef INC_SERIALLINK_H_
#define INC_SERIALLINK_H_

#include "stdbool.h"
#include "stm32h7xx_hal.h"
#include "app_common.h"

#define SL_ESC_CHAR		0x02

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

uint8_t u8SL_CalculateCRC(uint16_t u16Type, uint16_t u16Length, uint8_t *pu8Data);
void vSL_TxByte(bool bSpecialCharacter, uint8_t u8Data);
void vSL_WriteMessage(uint16_t u16Type, uint16_t u16Length, uint8_t *pu8Data);
bool bSL_ReadMessage(uint16_t *pu16Type, uint16_t *pu16Length, uint16_t u16MaxLength, uint8_t *pu8Message, uint8_t u8Data);
void APP_vProcessIncomingSerialCommands(uint8_t u8RxByte);

#endif /* INC_SERIALLINK_H_ */
