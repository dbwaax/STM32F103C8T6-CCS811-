#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	  
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//myiic ����	   
//by lty
//������̳:ltyworking@outlook.com
//�޸�����:2018/08/02
//�汾��V1.0
//��Ȩ���У�����ؾ���
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//IO��������
 
#define SDA_IN()  {GPIOA->CRH&=0XFFF0FFFF;GPIOA->CRH|=0X00080000;}
#define SDA_OUT() {GPIOA->CRH&=0XFFF0FFFF;GPIOA->CRH|=0X00030000;}

//IO��������	 
#define IIC_SCL    PAout(11) //SCL
#define IIC_SDA    PAout(12) //SDA	 
#define READ_SDA   PAin(12)  //����SDA 

#define SCL_read      GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)
#define SDA_read      GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)
//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
u8 IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

u8 Single_WriteIIC_byte(u8 Slave_Address,u8 REG_Address,u8 data);
u8 Single_MWriteIIC_byte(u8 Slave_Address,u8 REG_Address,u8 const *data,u8 length);
u8 Single_ReadIIC(u8 Slave_Address,u8 REG_Address,u8 *REG_data,u8 length);
#endif
















