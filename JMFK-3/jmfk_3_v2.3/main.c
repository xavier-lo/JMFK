
#include<stc_init.h>
#include<stc_isr.h>
#include<stc_user.h>
#include<operate_card.h>
#include<time_sd2200.h>
#include<stc_eeprom.h>



sbit BEEP	=	P3^7;//蜂鸣器
sbit SET_eeprom_baseaddr = P0^1;
sbit lamp_green	=	P3^5;
sbit lamp_red		=	P3^6;
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
	read_eeprom();
	BEEP=0;
	lamp_green = 0;
	lamp_red	=	0;
	delay(1000);
	WDT_CONTR	=	0x3f;//启动看门狗 29MHz 6T模式 预分频101  427ms 111 1.7s

	while(1)
	{
		action();
	}	
}

/********************************/
/*name:mcu_init									*/
/*discription:单片机初始化			*/
/*var in:												*/
/*result out:										*/
/********************************/
void mcu_init(void)
{
	init_uart(UART_1,MOD1,BTR_TM1,NOSMOD,btr_19200);
	init_time(TIME_0,WIDE16,t_20ms);
	ES = 0;
	EA = 1;
}





