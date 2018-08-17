////**********************************////
//=================//
//������������
//����JMFK-3 ˢ����
//MCU��stc89c55rd+
//����11.0592MHz ������1.085us
//�ļ���STC_ISR.C
//================//
////**********************************////
#include <STC_ISR.H>
#include <STC_USER.H>
#include <stc_eeprom.h>


#define repeat_card_count 80 //�ظ�ˢ��ʱ��

sbit BEEP = P3^7;

/*�ֲ���������*/
unsigned char t0_count,t0_count1,t0_count5;	

/**/
/*ȫ�ֱ�������*/
volatile bit delay40ms_ready = 0;
volatile bit beep_start = 0;

extern bit delay_5s_start;//�û�����Ч5s��ʼ 
extern bit delay_5s_stop;//�û�����Ч5s����
extern bit repeat_bit;//�ظ�����������ʼ
extern bit repeat_bit1;//�ظ�����������ʼ
extern bit delay1s_ready;//��ȡʱ������1s
extern unsigned char pre_IC_ISN[4];//��ֹ��ʱ���ظ�����
extern unsigned char pre_IC_ISN1[4];//��ֹ��ʱ���ظ�����
volatile unsigned char t0_count2;
volatile unsigned char t0_count4;
volatile unsigned char t0_count3;
/**/


//*************************//
//name:init_time0
//discription:��ʱ��0�жϳ�ʼ��
//in:
//out:
void TIME0_ISR()interrupt 1 using 1
{
		TH0=(65535-t_20ms)/256;
		TL0=(65535-t_20ms)%256;	
	  /*Ѱ�����*/
		t0_count++;
		if(t0_count > 2)
		{
			t0_count = 0;
			delay40ms_ready = 1;
			P14=!P14;
		}
		/*��ʱ����*/
		t0_count5++;
		if(t0_count5 > 50)
		{
			t0_count5 = 0;
			delay1s_ready = 1;		
		}	
		
		/*�������п���Ӧ*/
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
		/*�ظ�����ֹͣ�������1*/
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
		/*�ظ�����ֹͣ�������2*/		
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
		/*�û�����Ч5s����*/
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