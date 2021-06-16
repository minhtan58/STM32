/*
 * sim_fibocom.c
 *
 *  Created on: Jun 11, 2021
 *      Author: Admin
 */
#include <stdio.h>
#include <string.h>
#include "sim_fibocom.h"

void Sim_handler(void)
{
	printf("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");

	printf("0123456789\r\n");

	printf("AT+MIPCALL=1,\"internet\",\"User1\",\"Pswd\"\r\n");

	printf("AT+FTPMKD=\"Fibocom\"\r\n");

	printf("AT+FTPDEL=\"test1.txt\"\r\n");

	printf("AT+FTPPUT=\"test2.txt\",10,1\r\n");

	printf("AT+FTPPUT=\"test2.txt\",10,0\r\n");

	printf("AT+FTPOPEN=\"27.72.60.201\",\"tantm\",\"Tan1234\"\r\n");

	printf("AT+FTPOPEN=\"27.72.60.201\",\"u1\",\"13020371\",\"acount\",2224,2224,2224\r\n");

	printf("AT+FTPCLOSE\r\n");

	printf("AT\r\n");
}
