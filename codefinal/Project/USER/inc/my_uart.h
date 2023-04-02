#ifndef __MY_UART__H
#define __MY_UART__H

#include "headfile.h"

#define FRAME_HEADER1 0xfe
#define FRAME_HEADER2 0x01
#define FRAME_TAIL1 0x01
#define FRAME_TAIL2 0xfe


void Uart_Init(void);



#endif