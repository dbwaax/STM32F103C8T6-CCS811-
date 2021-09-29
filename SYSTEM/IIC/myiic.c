#include "myiic.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//IIC���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//��ʼ��IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	//ʹ��GPIOAʱ��
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_11|GPIO_Pin_12); 	//PA11,PA12 �����
}
//����IIC��ʼ�ź�
u8 IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	if(!SDA_read) return 0;
	delay_us(5);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	if(!SDA_read) return 0;
	delay_us(5);
	IIC_SCL=0;//ǯסIIC���ߣ�׼�����ͻ�������� 
	return 1;
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(5);
	IIC_SCL=1; 
	IIC_SDA=1;//����IIC���߽����ź�
	delay_us(5);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1; 	  
		delay_us(50);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(50); 
		IIC_SCL=0;	
		delay_us(50); 
		
    }	 

} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}
u8 Single_WriteIIC_byte(u8 Slave_Address,u8 REG_Address,u8 data)
{
	if(IIC_Start()==0)  //��ʼ�ź�
		{IIC_Start(); return RESET;}           

    IIC_Send_Byte(Slave_Address);   //�����豸��ַ+д�ź�
 	  if(IIC_Wait_Ack()){IIC_Stop(); return RESET;}
   
		IIC_Send_Byte(REG_Address);    //�ڲ��Ĵ�����ַ��
 	  if(IIC_Wait_Ack()){IIC_Stop(); return RESET;}
   
		IIC_Send_Byte(data);       //�ڲ��Ĵ������ݣ�
	  if(IIC_Wait_Ack()){IIC_Stop(); return RESET;}
		
		IIC_Stop();   //����ֹͣ�ź�
		
		return SET;
}

u8 Single_MWriteIIC_byte(u8 Slave_Address,u8 REG_Address,u8 const *data,u8 length)
{
	  IIC_Start();         

	IIC_Send_Byte(Slave_Address);   //�����豸��ַ+д�ź�
 	  if(IIC_Wait_Ack()){IIC_Stop(); return RESET;}
   
		IIC_Send_Byte(REG_Address);    //�ڲ��Ĵ�����ַ��
 	  if(IIC_Wait_Ack()){IIC_Stop(); return RESET;}
	 
	while(length)
	{
		IIC_Send_Byte(*data++);       //�ڲ��Ĵ������ݣ�
	   if(IIC_Wait_Ack()){IIC_Stop(); return RESET;}           //Ӧ��
		length--;
	}
		IIC_Stop();   //����ֹͣ�ź�		
		return SET;
}

u8 Single_ReadIIC(u8 Slave_Address,u8 REG_Address,u8 *REG_data,u8 length)
{
	
	if(IIC_Start()==0)  //��ʼ�ź�
		{IIC_Stop(); return RESET;}          
	 
	IIC_Send_Byte(Slave_Address);    //�����豸��ַ+д�ź�
 	if(IIC_Wait_Ack()){IIC_Stop(); return RESET;} 
	
	IIC_Send_Byte(REG_Address);     //���ʹ洢��Ԫ��ַ
 	if(IIC_Wait_Ack()){IIC_Stop(); return RESET;} 
	
	if(IIC_Start()==0)  //��ʼ�ź�
			{IIC_Stop(); return RESET;}            

	IIC_Send_Byte(Slave_Address+1);  //�����豸��ַ+���ź�
 	if(IIC_Wait_Ack()){IIC_Stop(); return RESET;}
	
	while(length-1)
	{
		*REG_data++=IIC_Read_Byte(1);       //�����Ĵ�������
		length--;
	}
	*REG_data=IIC_Read_Byte(0);  

	IIC_Stop();                    //ֹͣ�ź�
	return SET;
}























