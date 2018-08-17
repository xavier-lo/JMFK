
#include<stc_init.h>
#include<stc_isr.h>
#include<stc_user.h>
#include<operate_card.h>
#include<time_sd2200.h>
#include<stc_eeprom.h>



sbit BEEP	=	P3^7;//蜂鸣器
sbit SET_eeprom_baseaddr = P0^1;
//volatile unsigned int EEPROM_BASE_ADDR;
extern bit beep_start;


/********************************/
/*name:mcu_init									*/
/*discription:单片机初始化			*/
/*var in:												*/
/*result out:										*/
/********************************/
void mcu_init(void);


void delay(unsigned int i)
{
	while(i--);
}


/********************************/
/*name:													*/
/*discription:									*/
/*var in:												*/
/*result out:										*/
/********************************/
void main(void)
{
//	unsigned char xdata rd_da[21];
	mcu_init();

	BEEP=0;
	delay(10000);
//	if(!SET_eeprom_baseaddr)
//	{//读卡设置eeprom基地址
//		while(1)
//		{
//			if(read_card_1block(0x01,rd_da) == 1)//读取块1
//			{
//				if((rd_da[4] & 0x40) == 0x40)
//				{
//					EEPROM_BASE_ADDR = ((unsigned int)rd_da[6] << 8)&0xf000 ;
//					sectorerase(EEPROM_BASE_ADDR);
//					rd_da[0] = rd_da[6] & 0xf0;
//					rd_da[1] = 0;
//					if(arrywrite_overwrite(EEPROM_BASE_ADDR+10,2,rd_da) == 1)
//					{
//						beep_start = 1;
//						while(1);
//					}		
//				}
//			}
//		}
//	}else{
		read_eeprom();
		while(1)
		{
			action();
		}	
//	}

}

/********************************/
/*name:mcu_init									*/
/*discription:单片机初始化			*/
/*var in:												*/
/*result out:										*/
/********************************/
void mcu_init(void)
{
	init_uart(UART_1,MOD1,BTR_TM1,ISSMOD,btr_smod_19200);
	init_time(TIME_0,WIDE16,t_20ms);
	ES = 0;
	EA = 1;
}





