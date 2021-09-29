#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	  
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//myiic 代码	   
//by lty
//技术论坛:ltyworking@outlook.com
//修改日期:2018/08/02
//版本：V1.0
//版权所有，盗版必究。
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//IO方向设置
 
#define SDA_IN()  {GPIOA->CRH&=0XFFF0FFFF;GPIOA->CRH|=0X00080000;}
#define SDA_OUT() {GPIOA->CRH&=0XFFF0FFFF;GPIOA->CRH|=0X00030000;}

//IO操作函数	 
#define IIC_SCL    PAout(11) //SCL
#define IIC_SDA    PAout(12) //SDA	 
#define READ_SDA   PAin(12)  //输入SDA 

#define SCL_read      GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)
#define SDA_read      GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)
//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
u8 IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

u8 Single_WriteIIC_byte(u8 Slave_Address,u8 REG_Address,u8 data);
u8 Single_MWriteIIC_byte(u8 Slave_Address,u8 REG_Address,u8 const *data,u8 length);
u8 Single_ReadIIC(u8 Slave_Address,u8 REG_Address,u8 *REG_data,u8 length);
#endif
















