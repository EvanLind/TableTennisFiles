#ifndef _USART_H_
#define _USART_H_
#include "stdafx.h"
void Serial_Port_Init(char *_portname, int _rate);//Serial_Port_Init("\\\\.\\COM14", 57600);
void ReceiveChar_api(char com[], char *RevBuff);
int Wait4StrIs(char com[], char string[]);
void WriteChar_api(char com[], char string[]);
#define IS_WAIT4CALIB  0
#define IS_STARTCALIB  1
#endif