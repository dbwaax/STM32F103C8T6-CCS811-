#include "GPIO.h"
void GPIO_Initialization(u8 Type,u8 Pin,u8 mode)
{
	GPIO_InitTypeDef temp;
	/*GPIO Type*/
	/*0x01 GPIOA*/
	/*0x02 GPIOB*/
	/*0x03 GPIOC*/
	switch(Type)
	{
		case 0x01:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);break;
		case 0x02:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);break;
		case 0x03:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);break;
	}
	switch(Pin)
	{
		case 0:temp.GPIO_Pin = GPIO_Pin_0;break;
		case 0x01:temp.GPIO_Pin = GPIO_Pin_1;break;
		case 0x02:temp.GPIO_Pin = GPIO_Pin_2;break;
		case 0x03:temp.GPIO_Pin = GPIO_Pin_3;break;
		case 0x04:temp.GPIO_Pin = GPIO_Pin_4;break;
		case 0x05:temp.GPIO_Pin = GPIO_Pin_5;break;
		case 0x06:temp.GPIO_Pin = GPIO_Pin_6;break;
		case 0x07:temp.GPIO_Pin = GPIO_Pin_7;break;
		case 0x08:temp.GPIO_Pin = GPIO_Pin_8;break;
		case 0x09:temp.GPIO_Pin = GPIO_Pin_9;break;
		case 0x0a:temp.GPIO_Pin = GPIO_Pin_10;break;
		case 0x0b:temp.GPIO_Pin = GPIO_Pin_11;break;
		case 0x0c:temp.GPIO_Pin = GPIO_Pin_12;break;
		case 0x0d:temp.GPIO_Pin = GPIO_Pin_13;break;
		case 0x0e:temp.GPIO_Pin = GPIO_Pin_14;break;
		case 0x0f:temp.GPIO_Pin = GPIO_Pin_15;break;
	}
	/*0x01:GPIO_Mode_AIN;break;
		0x02:GPIO_Mode_IN_FLOATING;break;
		0x03:GPIO_Mode_IPD;break;
		0x04:GPIO_Mode_IPU;break;
		0x05:GPIO_Mode_Out_OD;break;
		0x06:GPIO_Mode_Out_PP;break;
		0x07:GPIO_Mode_AF_OD;break;
		0x08:GPIO_Mode_AF_PP;break;*/
	switch(mode)
	{
		case 0x01:temp.GPIO_Mode = GPIO_Mode_AIN;break;
		case 0x02:temp.GPIO_Mode = GPIO_Mode_IN_FLOATING;break;
		case 0x03:temp.GPIO_Mode = GPIO_Mode_IPD;break;
		case 0x04:temp.GPIO_Mode = GPIO_Mode_IPU;break;
		case 0x05:temp.GPIO_Mode = GPIO_Mode_Out_OD;break;
		case 0x06:temp.GPIO_Mode = GPIO_Mode_Out_PP;break;
		case 0x07:temp.GPIO_Mode = GPIO_Mode_AF_OD;break;
		case 0x08:temp.GPIO_Mode = GPIO_Mode_AF_PP;break;
	}
	temp.GPIO_Speed = GPIO_Speed_50MHz;
	switch(Type)
	{
		case 0x01:GPIO_Init(GPIOA,&temp);break;
		case 0x02:GPIO_Init(GPIOB,&temp);break;
		case 0x03:GPIO_Init(GPIOC,&temp);break;
	}
}
