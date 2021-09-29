#include "myiic.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//IIC驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//初始化IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	//使能GPIOA时钟
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_11|GPIO_Pin_12); 	//PA11,PA12 输出高
}
//产生IIC起始信号
u8 IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	if(!SDA_read) return 0;
	delay_us(5);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	if(!SDA_read) return 0;
	delay_us(5);
	IIC_SCL=0;//钳住IIC总线，准备发送或接收数据 
	return 1;
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(5);
	IIC_SCL=1; 
	IIC_SDA=1;//发送IIC总线结束信号
	delay_us(5);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(5);
	IIC_SCL=1;
	delay_us(5);
	IIC_SCL=0;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(5);
	IIC_SCL=1;
	delay_us(5);
	IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1; 	  
		delay_us(50);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(50); 
		IIC_SCL=0;	
		delay_us(50); 
		
    }	 

} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
		{
        IIC_SCL=0; 
        delay_us(50);
				IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
				delay_us(50); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}
u8 Single_WriteIIC_byte(u8 Slave_Address,u8 REG_Address,u8 data)
{
	if(IIC_Start()==0)  //起始信号
		{IIC_Start(); return RESET;}           

    IIC_Send_Byte(Slave_Address);   //发送设备地址+写信号
 	  if(IIC_Wait_Ack()){IIC_Stop(); return RESET;}
   
		IIC_Send_Byte(REG_Address);    //内部寄存器地址，
 	  if(IIC_Wait_Ack()){IIC_Stop(); return RESET;}
   
		IIC_Send_Byte(data);       //内部寄存器数据，
	  if(IIC_Wait_Ack()){IIC_Stop(); return RESET;}
		
		IIC_Stop();   //发送停止信号
		
		return SET;
}

u8 Single_MWriteIIC_byte(u8 Slave_Address,u8 REG_Address,u8 const *data,u8 length)
{
	  IIC_Start();         

	IIC_Send_Byte(Slave_Address);   //发送设备地址+写信号
 	  if(IIC_Wait_Ack()){IIC_Stop(); return RESET;}
   
		IIC_Send_Byte(REG_Address);    //内部寄存器地址，
 	  if(IIC_Wait_Ack()){IIC_Stop(); return RESET;}
	 
	while(length)
	{
		IIC_Send_Byte(*data++);       //内部寄存器数据，
	   if(IIC_Wait_Ack()){IIC_Stop(); return RESET;}           //应答
		length--;
	}
		IIC_Stop();   //发送停止信号		
		return SET;
}

u8 Single_ReadIIC(u8 Slave_Address,u8 REG_Address,u8 *REG_data,u8 length)
{
	
	if(IIC_Start()==0)  //起始信号
		{IIC_Stop(); return RESET;}          
	 
	IIC_Send_Byte(Slave_Address);    //发送设备地址+写信号
 	if(IIC_Wait_Ack()){IIC_Stop(); return RESET;} 
	
	IIC_Send_Byte(REG_Address);     //发送存储单元地址
 	if(IIC_Wait_Ack()){IIC_Stop(); return RESET;} 
	
	if(IIC_Start()==0)  //起始信号
			{IIC_Stop(); return RESET;}            

	IIC_Send_Byte(Slave_Address+1);  //发送设备地址+读信号
 	if(IIC_Wait_Ack()){IIC_Stop(); return RESET;}
	
	while(length-1)
	{
		*REG_data++=IIC_Read_Byte(1);       //读出寄存器数据
		length--;
	}
	*REG_data=IIC_Read_Byte(0);  

	IIC_Stop();                    //停止信号
	return SET;
}























