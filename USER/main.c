#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stdio.h"
#include "CCS811.h"
extern 	ccs811_measurement_t CCS;
int main(void)
{
	u8 st1 = 0;
	delay_init();
	Uart1_init(115200);
	CCS811Init();
	app_Start();
	delay_ms(150);
	while(1)
	{
		st1 = CCS811_ReadOneByte(0x00);
		switch(st1)
		{
			case 0x90:
			{
				setMode(MODE1);
			}break;
			case 0x98:
			{
				getData();
				delay_ms(10);
				if(CCS.eco2<10000)
				{
					printf("co2 = %d tvoc = %d\r\n",CCS.eco2,CCS.tvoc);
				}
				delay_ms(300);
			}break;
			case 0x10:
			{
				app_Start();
				printf("CCS811 Tries to enter App Mode!\r\n");
				delay_ms(150);
			}
			case 0x99:
			{
				st1 = CCS811_ReadOneByte(0xE0);
				printf("ERROR = %d\r\n",st1);
			}
			default:
			{
				reset();
				printf("CCS811 Tries to reset\r\n");
				delay_ms(150);
			}break;
		}
		delay_ms(1000);
	}
}
