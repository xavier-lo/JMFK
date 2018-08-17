////**********************************////
//=================//
//������������
//����JMFK-3 ˢ����
//MCU��stc89c55rd+
//����11.0592MHz ������1.085us
//�ļ���STC_MCU.H
//================//
////**********************************////


#ifndef __STC_MCU_H__
#define __STC_MCU_H__

#include <STC12C5A60S2.h>
//******�ж϶���***********//
typedef enum IRQn
{
/***������STC12C60S2**/
	EXTI0	=	0,//�ⲿ�ж�0
	TIME0	=	1,//��ʱ���ж�0
	EXTI1	=	2,//�ⲿ�ж�1
	TIME1	=	3,//��ʱ���ж�1
	UART	=	4,//�����ж�1
	ADC		=	5,//ģ��ת���ж�
	LVD		=	6,
	PCA		=	7,
	UART2	=	8,//�����ж�2
	SPI		=	9 //SPI�ж�
} IRQn_Type;
//**********//
//******��ʱ������***********//
typedef enum TIM_MOD
{
/***������STC12C60S2**/
	WIDE13	=	0,//13λ��ʱ����
	WIDE16	=	1,//16λ��ʱ����
	WIDE8	=	2,//8λ��ʱ����
	WIDE8D	=	3,//˫8λ��ʱ���� ��ʱ��0
} TIM_MOD_Type;
//*********//

//******���ڶ���***********//
typedef enum UART_MOD
{
/***������STC12C60S2**/	  
	MOD0	=	0,//ͬ����λ�Ĵ淽ʽ
	MOD1	=	1,//����8λ�������ʿɱ�
	MOD2	=	2,//����9λ
	MOD3	=	3,//����9λ�������ʿɱ�	   
	BTR_BTR	=	4,//������ʹ�ö��������ʷ�����
	BTR_TM1	=	5,//������ʹ��TIME1Ϊ�����ʷ�����
	ISSMOD	=	6,//�����ʼӱ�
	NOSMOD	=	7 //�����ʲ��ӱ�
} UART_MOD_Type;
//*********//
//******���ڶ���***********//
typedef enum UART_NAME
{
/***������STC12C60S2**/	  
	UART_1	=	1,
	UART_2	=	2
} UART_NAME_Type;
//*********//
//******��ʱ������***********//
typedef enum TIME_NAME
{
/***������STC12C60S2**/	  
	TIME_0	=	1,
	TIME_1	=	2
} TIME_NAME_Type;
//*********//


//******��ʱ������***********//
typedef enum TIM_VALUE
{
/***������STC12C60S2**/

			t_1ms 	=	2457*2,
			t_2ms		= t_1ms*2,
			t_10ms	= t_1ms*10,
			t_15ms	=	t_1ms*15,
			t_20ms	=	t_2ms*10,
			t_40ms	=	t_20ms*2,
			btr_smod_19200	=	256-3, //11.0592MHz/3/(32*12)*2=19200
		  btr_19200 =	256-8
/*
			btr_38400	=	 256-2,
			BTR_9600	=	256-3,
			BTR_19200	=	256-4,
			BTR_10k		=	256-13			//29.4912MHz/13/(32*12)=0.005008mbit/s =5.008kbit/s	
*/
} TIM_VALUE_Type;
//*********//


//*****IOģʽ����***********//
typedef enum GPIO_MOD
{
	GPIO_STANDERD				=	0,//��׼˫���
	GPIO_PUSHPULL				=	1,//������� ǿ�������
	GPIO_HIGHIMPEDENCE	=	2,//��������
	GPIO_OPENDRAIN			=	3//��©��� �ڲ������Ͽ� �����			
}GPIO_MOD_Type;
//*******//
//*****IO���Ŷ���***********//
typedef enum GPIO_PIN
{
	GPIO_PIN_0				=	1,
	GPIO_PIN_1				=	2,
	GPIO_PIN_2				=	4,
	GPIO_PIN_3				=	8,
	GPIO_PIN_4				=	16,
	GPIO_PIN_5				=	32,
	GPIO_PIN_6				=	64,
	GPIO_PIN_7				=	128,
	GPIO_PIN_all			=	255	
}GPIO_PIN_Type;
//*******//
//*****IO������***********//
typedef enum GPIO_NAME
{
	GPIO_0				=	1,//P0
	GPIO_1				=	2,//P1
	GPIO_2				=	4,//P2
	GPIO_3				=	8,//P3
	GPIO_4				=	16//P4
}GPIO_NAME_Type;
//*******//

////******��ʱ������***********//
//struct TIME_InitTypeDef
//{
//	unsigned char MOD;
//	unsigned int VALUE;	
//};
////*****//	



#endif