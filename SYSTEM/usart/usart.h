#ifndef __USART_H
#define __USART_H
#include "sys.h"
#include "usart.h"	 
#include "stdio.h"
extern char buff[100];	  	
extern char USART_RX_BUF[200];
extern unsigned char USART_RX_STA;
extern unsigned char flag;
void Uart1_init(u32 bound);
void Uart2_init(u32 bound);
void PutString(u8 mode,char * array);
#endif


