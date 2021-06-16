/*
 * app_common.h
 *
 *  Created on: Mar 10, 2021
 *      Author: Admin
 */

#ifndef INC_APP_COMMON_H_
#define INC_APP_COMMON_H_

#define SL_START_CHAR          0x01
#define SL_ESC_CHAR            0x02
#define SL_END_CHAR            0x03
#define MAX_PACKET_SIZE        256

#include "stm32h7xx_hal.h"
#include <stdio.h>

UART_HandleTypeDef huart8;
UART_HandleTypeDef huart1;

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


#endif /* INC_APP_COMMON_H_ */
