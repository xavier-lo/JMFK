////**********************************////
//=================//
//本程序适用于
//用于JMFK-3 刷卡器
//MCU：stc89c55rd+
//晶振：11.0592MHz 震荡周期1.085us
//文件：STC_ISR.C
//================//
////**********************************////
#include <STC_ISR.H>
#include <STC_USER.H>
#include <stc_eeprom.h>


#define repeat_card_count 80 //重复刷卡时长

sbit BEEP = P3^7;

/*局部变量定义*/
unsigned char t0_count,t0_count1,t0_count5;	

/**/
/*全局变量定义*/
volatile bit delay40ms_ready = 0;
volatile bit beep_start = 0;

extern bit delay_5s_start;//用户卡有效5s开始 
extern bit delay_5s_stop;//用户卡有效5s结束
extern bit repeat_bit;//重复读卡计数开始
extern bit repeat_bit1;//重复读卡计数开始
extern bit delay1s_ready;//读取时间周期1s
extern unsigned char pre_IC_ISN[4];//防止短时间重复读卡
extern unsigned char pre_IC_ISN1[4];//防止短时间重复读卡
volatile unsigned char t0_count2;
volatile unsigned char t0_count4;
volatile unsigned char t0_count3;
/**/


//*************************//
//name:init_time0
//discription:定时器0中断初始化
//in:
//out:
void TIME0_ISR()interrupt 1 using 1
{
		TH0=(65535-t_20ms)/256;
		TL0=(65535-t_20ms)%256;	
	  /*寻卡间隔*/
		t0_count++;
		if(t0_count > 2)
		{
			t0_count = 0;
			delay40ms_ready = 1;
			P14=!P14;
		}
		/*读时间间隔*/
		t0_count5++;
		if(t0_count5 > 50)
		{
			t0_count5 = 0;
			delay1s_ready = 1;		
		}	
		
		/*蜂鸣器有卡响应*/
		if(beep_start)
		{
			t0_count1++;
			if(t0_count1 == 1)
			{
				BEEP = 1;
			}
			if(t0_count1 > 10)
			{
				BEEP = 0;
				t0_count1 = 0;
				beep_start = 0;
			}
		}
		/*重复读卡停止读卡间隔1*/
		if(repeat_bit)
		{
			t0_count2++;
			if(t0_count2 > repeat_card_count)
			{
				t0_count2 = 0;
				repeat_bit = 0;
				pre_IC_ISN[0] = 0; 
				pre_IC_ISN[1] = 0; 
				pre_IC_ISN[2] = 0; 
				pre_IC_ISN[3] = 0; 
			}			
		}
		/*重复读卡停止读卡间隔2*/		
		if(repeat_bit1)
		{
			t0_count4++;
			if(t0_count4 > repeat_card_count)
			{
				t0_count4 = 0;
				repeat_bit1 = 0;
				pre_IC_ISN1[0] = 0; 
				pre_IC_ISN1[1] = 0; 
				pre_IC_ISN1[2] = 0; 
				pre_IC_ISN1[3] = 0; 
			}			
		}
		/*用户卡有效5s计数*/
		if(delay_5s_start)
		{
			t0_count3++;
			if(t0_count3 > 250)
			{
				t0_count3 = 0;
				delay_5s_start = 0;
				delay_5s_stop = 1;
			}		
		}
}

//******//

//*************************//
//name:USART1_ISR
//discription:
//in:
//out:
void USART1_ISR(void)interrupt 4 using 2
{
	int i=0;
	if(RI)
	{
		RI=0;
		
		
	}
	else if(TI)
	{
		TI=0;
	}
}

//******//
//*************************//
//name:USART1_ISR
//discription:
//in:
//out:
void USART2_ISR(void)interrupt 8 using 3
{
			if(S2CON&0x01)
			{
				
				S2CON&=0xfe;
			}

}

//******//
