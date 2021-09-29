
#include "CCS811.h"
//typedef struct {
//u16 eco2;
//u16 tvoc;
//u8 status;
//u8 error_id;
//u8 hw_id;
//u16 raw_data;
//} ccs811_measurement_t;

ccs811_measurement_t CCS;
void CCS811Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_8;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_8);						 //PB.5 输出高

	/********************************************************/

	IIC_Init();
}
//查看是否有新数据 返回值 1：有
//						  0：没有
u8 CheckData_Ready(void)
{
		readStatus();
	if((CCS.status&8)==1) return 1;
	else return 0;
}
//读取一个bit数据
u8 CCS811_ReadOneByte(u8 ReadAddr)
{
		u8 temp=0;	
		nWAKE=0;
		delay_us(50);//必须等待50us
		IIC_Start();  						
		//addr接地，7位地址为0x5A，读地址为AB,写地址AA
		IIC_Send_Byte(WRIT);	   //发送写命令
		if(IIC_Wait_Ack())return 254;
		//else nINT=0;
		IIC_Send_Byte(ReadAddr);
		if(IIC_Wait_Ack())return 253;
		//if(!IIC_Wait_Ack()){nINT=1;delay_ms(200);nINT=0;delay_ms(200);}
		delay_ms(1);
		IIC_Start();
		IIC_Send_Byte(READ);
		if(IIC_Wait_Ack())return 252;
		temp=IIC_Read_Byte(0); 
		IIC_Stop();
		nWAKE=1;
		delay_us(50);
	return temp;


}
void CCS811_WriteOneByte(u8 RegAddr,u8 DataToWrite)
{
	nWAKE=0;
	delay_us(50);
	IIC_Start(); 
	IIC_Send_Byte(WRIT);	   //发送写命令
	IIC_Wait_Ack();
	IIC_Send_Byte(RegAddr);
	IIC_Wait_Ack();
	IIC_Send_Byte(DataToWrite);
	IIC_Wait_Ack();
	IIC_Stop();
	nWAKE=1;
}
//测量数据前必须开启
void app_Start(void)
{
	nWAKE=0;
	delay_us(50);
	IIC_Start(); 
	IIC_Send_Byte(WRIT);	   //发送写命令
	if(IIC_Wait_Ack()){
	IIC_Stop();return;}
	IIC_Send_Byte(0xF4);
	delay_us(200);			//必须等待180us以上才能够收到应答
	if(IIC_Wait_Ack()){IIC_Stop();return;}
	IIC_Stop();
	nWAKE=1;

}

void readStatus(void)
{
  
  CCS.status=CCS811_ReadOneByte(STATUS);
  
}


void readHW_ID(void)
{

	CCS.hw_id=CCS811_ReadOneByte(HW_ID);
}


void readErrorID(void)
{
	
	CCS.error_id=CCS811_ReadOneByte(ERROR_ID);
  
  
}

void setMode(u8 mode)
{
//	nWAKE=0;
//	delay_us(50);
	CCS811_WriteOneByte(MEAS_MODE,mode);
//	nWAKE=1;
}
	
void readALG_RESULT_DATA(u8 *data)
{
	
	nWAKE=0;
	delay_us(50);
	Single_ReadIIC(WRIT,ALG_RESULT_DATA,data,8);
	nWAKE=1;
  

}
//一次读8bit数据，前2bit为eco2数据，接着2bit为tvoc数据
//当status第五位为1时有新数据。
void getData(void)
{
	u8 i,buffer[8];
	
    //readALG_RESULT_DATA(buffer);
	nWAKE=0;
	delay_us(50);
	IIC_Start(); 
	IIC_Send_Byte(WRIT);	   //发送写命令
	IIC_Wait_Ack();
	IIC_Send_Byte(ALG_RESULT_DATA);
	IIC_Wait_Ack();
	IIC_Stop();
	delay_us(50);
	IIC_Start(); 
	IIC_Send_Byte(READ);	   //发送写命令
	IIC_Wait_Ack();
	for(i=0;i<8-1;i++)
	buffer[i]=IIC_Read_Byte(1);
	buffer[i]=IIC_Read_Byte(0);
	IIC_Stop();
	nWAKE=1;
	
	CCS.eco2 = ((u16)buffer[0] << 8) + buffer[1];
	CCS.tvoc = ((u16)buffer[2] << 8) + buffer[3];
	CCS.status=buffer[4];
	
}
//睡眠模式
void sleep(void)
{

		CCS811_WriteOneByte(MEAS_MODE,0x00000000);
}
//软重置，回到boot模式
void reset(void)
{
	
//	u8 ResetData[4]={0X11,0XE5,0X72,0X8A};
	nWAKE=0;
	delay_us(50);
	IIC_Start(); 
	IIC_Send_Byte(WRIT);	   //发送写命令
	IIC_Wait_Ack();
	IIC_Send_Byte(SW_RESET);
	IIC_Wait_Ack();
	IIC_Send_Byte(0X11);
	IIC_Wait_Ack();
	IIC_Send_Byte(0XE5);
	IIC_Wait_Ack();
	IIC_Send_Byte(0X72);
	IIC_Wait_Ack();
	IIC_Send_Byte(0X8A);
	IIC_Wait_Ack();
	IIC_Stop();
	nWAKE=1;
}



//void compensate(float t, float rh)    // compensate for temperature and relative humidity
//{
  
	
//  int _t, _rh;
//  if(t>0)
//    _t = (int)t + 0.5;  // this will round off the floating point to the nearest integer value
//  else if(t<0)
//   _t = (int)t - 0.5;
//  _t = _t + 25;  // temperature high byte is stored as T+25°C so the value of byte is positive
//  _rh = (int)rh + 0.5;  // this will round off the floating point to the nearest integer value


	//CCS811_WriteOneByte(ENV_DATA,_t);
	
//}
