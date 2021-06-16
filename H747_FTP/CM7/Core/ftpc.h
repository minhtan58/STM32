/*
 * ftpc.h
 *
 *  Created on: Nov 21, 2020
 *      Author: Admin
 */

#ifndef FTPC_H_
#define FTPC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <stdarg.h>
//#include "stdio_private.h"
//#include "socket.h"
//#include "w5500.h"
#include "uart.h"

#define F_APP_FTPC

#define AUTOLOGON_ANONYMOUS

#define FTP_DIR_LIST_PAUSE 500 /*500ms..3000ms*/

/* If your target support a file system, you have to activate this feature and implement. */
#define F_FILESYSTEM

/* Change to your Chipset Uart function, you have to activate this feature and implement.
 * Change!! -> Board_UARTGetCharBlocking()
 * Below is an example of a function of lpc_chip library. */
//#define ftp_getc()	Board_UARTGetCharBlocking()

#ifdef F_FILESYSTEM
#include "ff.h"
#endif

//#ifndef	ftp_getc()
//#define Need_UARTGetCharBlocking_func
//#define ftp_getc() uart0_receive()
//#else
/* Change library
 * Change!! -> board_api.h,
 * Below is an example of a function of lpc_chip library. */
//#include "board_api.h"
//#endif


#define LINELEN		100

#define _MAX_SS_FTP		2048

#ifndef F_FILESYSTEM
#define _MAX_SS_FTP		2048
#endif

#define CTRL_SOCK	2
#define DATA_SOCK	3

/* FTP Responses */
#define R_150	150		/* File status ok; opening data conn */
#define R_200	200		/* 'Generic' command ok */
#define R_220	220		/* Service ready for new user. */
#define R_226	226		/* Closing data connection.  File transfer/abort successful */
#define R_227	227		/* Entering passive mode (h1,h2,h3,h4,p1,p2) */
#define R_230	230		/* User logged in, proceed */
#define R_250	250		/* Deleted file success */
#define R_331	331		/* User name okay, need password. */
#define R_530	530		/* 530 Permission denied */
#define R_550	550		/* Can't delete file */

#define TransferAscii		'A'
#define TransferBinary		'I'

enum ftpc_type {
	ASCII_TYPE,
	IMAGE_TYPE,
};

enum ftpc_datasock_state{
	DATASOCK_IDLE,
	DATASOCK_READY,
	DATASOCK_START
};

enum ftpc_datasock_mode{
	PASSIVE_MODE,
	ACTIVE_MODE
};
enum CommandFirst {
	f_nocmd,
	f_dir,
	f_put,
	f_get,
};
enum CommandSecond {
	s_nocmd,
	s_dir,
	s_put,
	s_get,
};
struct Command {
	enum CommandFirst First;
	enum CommandSecond Second;
};
struct ftpc {
	uint8_t control;			/* Control stream */
	uint8_t data;				/* Data stream */

	enum ftpc_type type;		/* Transfer type */

	enum ftpc_datasock_state dsock_state;
	enum ftpc_datasock_mode dsock_mode;

	char workingdir[LINELEN];
	char filename[LINELEN];

#ifdef F_FILESYSTEM
	FIL fil;	// FatFs File objects
	FRESULT fr;	// FatFs function common result code
#endif
};

#ifndef un_I2cval
typedef union _un_l2cval {
	uint32_t	lVal;
	uint8_t		cVal[4];
}un_l2cval;
#endif

void ftpc_init(uint8_t * src_ip);
uint8_t ftpc_run(uint8_t * dbuf);
char proc_ftpc(char * buf);
int pportc(char * arg);
uint8_t* User_Keyboard_MSG();

FRESULT scan_files(char* path, char* buf, int * buf_len);

#ifdef __cplusplus
}
#endif

#endif /* FTPC_H_ */
