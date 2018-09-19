////**********************************////
//=================//
//������������
//����JMFK-3 ˢ����
//MCU��stc89c55rd+
//����29.4912MHz ������0.4069us ˫��������
//�ļ���operate_card.C
//���ļ�ʹ�õ�Ƭ�����贮��1
//================//
#include <operate_card.h>

#define card_delay 12000

sbit BEEP	=	P3^7;//������

sbit TEST = P1^4;

sbit CardHead_ERR_Lamp = P4^2;

/**********************************/
/*name:make_verr									*/
/*discription:����У��						*/
/*var in:da0 ��Ҫ���ɵ�������			*/
/*result out:	da0 ���ɺ��������	*/
/**********************************/
void make_verr(unsigned char *da0)
{
	unsigned char i = 0, verr = 0;
	verr = da0[2];
	i = 3;
	while(i <= da0[2]+1)
	{
		verr ^= da0[i];
		i++;
	}
	da0[i] = verr;
}

/********************************/
/*name:	verrify									*/
/*discription:У���������			*/
/*var in:da0 ��ҪУ�������			*/
/*result out:bool ״̬					*/
/********************************/
bit verrify(unsigned char *da0)
{
	bit states = 0;
	unsigned char ver_da = 0, i = 0;
	if(da0[0] == 0xaa)
	{
		if(da0[1] == 0x55)
		{			
			i = 3;
			ver_da = da0[2];
			while(i < da0[2]+3)
			{
				ver_da ^= da0[i];//���У������
				i++;
			}
			if(ver_da == 0)//���Ϊ0У��ɹ�
				states = 1;
		}		
	}
	return states;
}


/**********************************************/
/*name:seek_card															*/
/*discription:Ѱ��														*/
/*var in:																			*/
/*result out:recive_da ��������	�������9�ֽ�	*/
/**********************************************/
char seek_card(unsigned char *recive_da)
{
	char state	=	0;
	unsigned char i	=	0;
	unsigned int recive_count	=	0;
	/*
	����Ѱ�����ݵ���ͷ
	6���ֽ�
	*/

	recive_da[0]=0xaa;
	recive_da[1]=0x55;
	recive_da[2]=0x03;
	recive_da[3]=0x20;
	recive_da[4]=0x00;	
	
	make_verr(recive_da);//����У����
	i = 0;
	while(i < 6)//����
	{
		TI = 0;
		SBUF = recive_da[i];
		i++;
		while(!TI);	
	}
	RI = 0;
	REN = 1;
	i = 0;	
	
	while(i < 9)//��������������9���ֽ�
	{	
		recive_count	=	0;			
		while(!RI)
		{
			recive_count++;
			if(recive_count>=card_delay)
			{
				REN	=	0;
				return state;//��ʱ�˳�
			}
		}
		recive_da[i] = SBUF;
		RI	=	0;		
		i++;		
	}
	
	if((verrify(recive_da) == 1)&&(recive_da[2] == 0x06))//У���������
	{
			state	=	1;//��λ�ɹ�״̬
			CardHead_ERR_Lamp = 0;
	}
	else CardHead_ERR_Lamp = 1;
	REN	=	0;
	return state;
}

/******************************************/
/*name:	write_card												*/
/*discription:д���ݵ���ʹ�ÿ�ͷ����			*/
/*var in:block ���												*/
/*var in:da ��Ҫд������16�ֽڴ���25�ֽ�	*/
/*result out:state д��״̬								*/
/******************************************/
bit write_card(unsigned char block,unsigned char *da)
{
	unsigned char i	=	0, seek_data[9];
	unsigned int recive_count	=	0 ;
	bit state	=	0 ;

	/*Ѱ��*/
	if(seek_card(seek_data)	==	0)
	{
		return state;//�޿��˳�
	}
	
	/*
	������ݵ�������
	*/
	da[0]	=	0xaa;
	da[1]	=	0x55;
	da[2]	=	0x16;
	da[3]	=	0x22;
	da[4]	=	0x02;
	da[5]	=	block;
	da[6]	=	0x80;
	da[7]	=	0x00;

	make_verr(da);//����У��
	
	/* ���� */
	i = 0;
	while(i < 25)
	{
		recive_count	=	0;
		TI = 0;
		SBUF = da[i];
		i++;
		while(!TI)
		{
			recive_count++;
			if(recive_count >= card_delay/6 ) return 0;//��ʱ�˳�
		}	
	}	
	/*
	���շ���״̬
	*/
	RI = 0;
	i = 0;
	REN = 1;
	while(i < 5)//��������������5���ֽ�
	{	
		recive_count	=	0;			
		while(!RI)
		{
			recive_count++;
			if(recive_count>=card_delay) 
			{
				REN=0;
				return state;//��ʱ�˳�
			}
		}
		da[i] = SBUF;
		RI	=	0;		
		i++;
		
	}
	if((verrify(da) == 1)&&(da[3] == 0x22))//У���������
	{
			state	=	1;//��λ�ɹ�״̬
	}
	REN	=	0;	
	return state;
}



/********************************/
/*name:read_card_1block					*/
/*discription:����1������				*/
/*var in:����16�ֽ� ����21�ֽ�	*/
/*result out:										*/
/********************************/
bit read_card_1block(unsigned char block,unsigned char *out)
{
	unsigned char i	=	0;
	unsigned int recive_count1 = 0;
	unsigned char seek_data[9];
	bit state	=	0	;
	/*Ѱ��*/
	if(seek_card(seek_data)	==	0)
	{
		return state;//�޿��˳�
	}
	/*
	������ݵ�������
	*/
	out[0]	=	0xaa;
	out[1]	=	0x55;
	out[2]	=	0x06;
	out[3]	=	0x21;
	out[4]	=	0x02;
	out[5]	=	block;
	out[6]	=	0x80;
	out[7]	=	0x00;	
	make_verr(out);//����У��
	
	/*����*/
	i = 0;
	while(i < 9)
	{
		TI = 0;
		SBUF = out[i];
		i++;
		while(!TI);	
	}	
	/*
	���շ���״̬
	*/
	RI = 0;
	i = 0;
	REN = 1;
	while(i < 21)//��������������21���ֽ�
	{	
		recive_count1	=	0;			
		while(!RI)
		{
			recive_count1++;
			if(recive_count1>=card_delay)
			{
				REN=0;
				return state;//��ʱ�˳�
			}
		}
		out[i] = SBUF;
		RI	=	0;		
		i++;
		
	}		
	
	if((verrify(out) == 1)&&(out[3] == 0x21))//У���������
	{
			state	=	1;//��λ�ɹ�״̬
	}	
	
	REN	=	0;
	out[0]	=	seek_data[4];
	out[1]	=	seek_data[5];
	out[2]	=	seek_data[6];
	out[3]	=	seek_data[7];
	return state;
}



/********************************************/
/*name:	read_card														*/
/*discription:��ĳһ������									*/
/*var in:	sector ������											*/
/*result out:	out ��������48�ֽڴ���53�ֽ�	*/
/*result out:	state ״̬										*/
/********************************************/
bit read_card(unsigned char sector, unsigned char *out)
{
	unsigned char i	=	0;
	unsigned int recive_count1 = 0;
	unsigned char seek_data[9];
	bit r_state	=	0	;
	/*Ѱ��*/
	if(seek_card(seek_data)	==	0)
	{
		return r_state;//�޿��˳�
	}
	
	/*
	������ݵ�������
	*/
	out[0]	=	0xaa;
	out[1]	=	0x55;
	out[2]	=	0x06;
	out[3]	=	0x21;
	out[4]	=	0x02;
	out[5]	=	sector*4+2;
	out[6]	=	0x80;
	out[7]	=	0x00;	
	make_verr(out);//����У��
	
	/*����*/
	i = 0;
	while(i < 9)
	{
		TI = 0;
		SBUF = out[i];
		i++;
		while(!TI);	
	}	
	/*
	���շ���״̬
	*/
	RI = 0;
	i = 0;
	REN = 1;
	while(i < 21)//��������������21���ֽ�
	{	
		recive_count1	=	0;			
		while(!RI)
		{
			recive_count1++;
			if(recive_count1>=card_delay)
			{
				REN=0;
				return r_state;//��ʱ�˳�
			}
		}
		out[i] = SBUF;
		RI	=	0;		
		i++;
		
	}
	if((verrify(out) != 1)	||	(out[3] != 0x21))//У���������
	{
		r_state	=	0;
		return r_state;
	}
	/*ӳ�䵽���16�ֽ�*/
	i	=	0;
	while(i < 16)
	{
		out[37 + i]	= out[4 + i];
		i++;
	}
	
	/*
	������ݵ�������
	*/
	out[0]	=	0xaa;
	out[1]	=	0x55;
	out[2]	=	0x06;
	out[3]	=	0x21;
	out[4]	=	0x02;
	out[5]	=	sector*4+1;
	out[6]	=	0x80;
	out[7]	=	0x00;	
	make_verr(out);//����У��
	
	/*����*/
	i = 0;
	while(i < 9)
	{
		TI = 0;
		SBUF = out[i];
		i++;
		while(!TI);	
	}	
	/*
	���շ���״̬
	*/
	RI = 0;
	i = 0;
	REN = 1;
	while(i < 21)//��������������21���ֽ�
	{	
		recive_count1	=	0;			
		while(!RI)
		{
			recive_count1++;
			if(recive_count1 >= card_delay)
			{
				REN=0;
				return r_state;//��ʱ�˳�
			}
		}
		out[i] = SBUF;
		RI	=	0;		
		i++;
		
	}

	if((verrify(out) != 1)	||	(out[3] != 0x21))//У���������
	{
		r_state	=	0;
		return r_state;
	}
	/*ӳ�䵽�м�16�ֽ�*/
	i	=	0;
	while(i < 16)
	{
		out[21 + i]	= out[4 + i];
		i++;
	}
	
	/*
	������ݵ�������
	*/
	out[0]	=	0xaa;
	out[1]	=	0x55;
	out[2]	=	0x06;
	out[3]	=	0x21;
	out[4]	=	0x02;
	out[5]	=	sector*4+0;
	out[6]	=	0x80;
	out[7]	=	0x00;	
	make_verr(out);//����У��
	
	/*����*/
	i = 0;
	while(i < 9)
	{
		TI = 0;
		SBUF = out[i];
		i++;
		while(!TI);	
	}	
	/*
	���շ���״̬
	*/
	RI = 0;
	i = 0;
	REN = 1;
	while(i < 21)//��������������21���ֽ�
	{	
		recive_count1	=	0;			
		while(!RI)
		{
			recive_count1++;
			if(recive_count1>=card_delay)
			{
				REN=0;
				r_state = 0;
				return r_state;//��ʱ�˳�
			}
		}
		out[i] = SBUF;
		RI	=	0;		
		i++;
		
	}		
	
	TEST = 1;
	

	if((verrify(out) == 1)&&(out[3] == 0x21))//У���������
	{
			r_state	=	1;//��λ�ɹ�״̬
	}	
	
	REN	=	0;
	out[0]	=	seek_data[4];
	out[1]	=	seek_data[5];
	out[2]	=	seek_data[6];
	out[3]	=	seek_data[7];
	i = 0;
	while(i < 32)
	{
		out[20 + i] =out[21 + i];	
		i++;
	}	
	
	TEST = 0;
	return r_state;
}

/********************************/
/*name:	read_user_card					*/
/*discription:���û���					*/
/*var in:	in ������							*/
/*var in:	out ����69���ֽ�			*/
/*result out:	����״̬					*/
/********************************/
bit read_user_card(unsigned char sector, unsigned char *out)
{
	unsigned char i	=	0;
	unsigned int recive_count1 = 0;
	unsigned char seek_data[9];
	bit r_state	=	0	;
	/*Ѱ��*/
	if(seek_card(seek_data)	==	0)
	{
		return r_state;//�޿��˳�
	}
	
	/*
	������ݵ�������
	*/
	out[0]	=	0xaa;
	out[1]	=	0x55;
	out[2]	=	0x06;
	out[3]	=	0x21;
	out[4]	=	0x02;
	out[5]	=	(sector + 1) * 4 + 1;
	out[6]	=	0x80;
	out[7]	=	0x00;	
	make_verr(out);//����У��
	
	/*����*/
	i = 0;
	while(i < 9)
	{
		TI = 0;
		SBUF = out[i];
		i++;
		while(!TI);	
	}	
	/*
	���շ���״̬
	*/
	RI = 0;
	i = 0;
	REN = 1;
	while(i < 21)//��������������21���ֽ�
	{	
		recive_count1	=	0;			
		while(!RI)
		{
			recive_count1++;
			if(recive_count1>=card_delay)
			{
				REN=0;
				return r_state;//��ʱ�˳�
			}
		}
		out[i] = SBUF;
		RI	=	0;		
		i++;
		
	}
	if((verrify(out) != 1)	||	(out[3] != 0x21))//У���������
	{
		r_state	=	0;
		return r_state;
	}
	/*ӳ�䵽���16�ֽ�*/
	i	=	0;
	while(i < 16)
	{
		out[53 + i]	= out[4 + i];
		i++;
	}
	
	/*
	������ݵ�������
	*/
	out[0]	=	0xaa;
	out[1]	=	0x55;
	out[2]	=	0x06;
	out[3]	=	0x21;
	out[4]	=	0x02;
	out[5]	=	(sector + 1) * 4 + 0;
	out[6]	=	0x80;
	out[7]	=	0x00;	
	make_verr(out);//����У��
	
	/*����*/
	i = 0;
	while(i < 9)
	{
		TI = 0;
		SBUF = out[i];
		i++;
		while(!TI);	
	}	
	/*
	���շ���״̬
	*/
	RI = 0;
	i = 0;
	REN = 1;
	while(i < 21)//��������������21���ֽ�
	{	
		recive_count1	=	0;			
		while(!RI)
		{
			recive_count1++;
			if(recive_count1>=card_delay)
			{
				REN=0;
				return r_state;//��ʱ�˳�
			}
		}
		out[i] = SBUF;
		RI	=	0;		
		i++;
		
	}
	if((verrify(out) != 1)	||	(out[3] != 0x21))//У���������
	{
		r_state	=	0;
		return r_state;
	}
	/*ӳ�䵽��16�ֽ�*/
	i	=	0;
	while(i < 16)
	{
		out[37 + i]	= out[4 + i];
		i++;
	}	

	/*
	������ݵ�������
	*/
	out[0]	=	0xaa;
	out[1]	=	0x55;
	out[2]	=	0x06;
	out[3]	=	0x21;
	out[4]	=	0x02;
	out[5]	=	sector * 4 + 2;
	out[6]	=	0x80;
	out[7]	=	0x00;	
	make_verr(out);//����У��
	
	/*����*/
	i = 0;
	while(i < 9)
	{
		TI = 0;
		SBUF = out[i];
		i++;
		while(!TI);	
	}	
	/*
	���շ���״̬
	*/
	RI = 0;
	i = 0;
	REN = 1;
	while(i < 21)//��������������21���ֽ�
	{	
		recive_count1	=	0;			
		while(!RI)
		{
			recive_count1++;
			if(recive_count1>=card_delay)
			{
				REN=0;
				return r_state;//��ʱ�˳�
			}
		}
		out[i] = SBUF;
		RI	=	0;		
		i++;
		
	}
	if((verrify(out) != 1)	||	(out[3] != 0x21))//У���������
	{
		r_state	=	0;
		return r_state;
	}
	/*ӳ�䵽���16�ֽ�*/
	i	=	0;
	while(i < 16)
	{
		out[21 + i]	= out[4 + i];
		i++;
	}	

	/*
	������ݵ�������
	*/
	out[0]	=	0xaa;
	out[1]	=	0x55;
	out[2]	=	0x06;
	out[3]	=	0x21;
	out[4]	=	0x02;
	out[5]	=	sector*4+1;
	out[6]	=	0x80;
	out[7]	=	0x00;	
	make_verr(out);//����У��
	
	/*����*/
	i = 0;
	while(i < 9)
	{
		TI = 0;
		SBUF = out[i];
		i++;
		while(!TI);	
	}	
	/*
	���շ���״̬
	*/
	RI = 0;
	i = 0;
	REN = 1;
	while(i < 21)//��������������21���ֽ�
	{	
		recive_count1	=	0;			
		while(!RI)
		{
			recive_count1++;
			if(recive_count1>=card_delay)
			{
				REN=0;
				r_state = 0;
				return r_state;//��ʱ�˳�
			}
		}
		out[i] = SBUF;
		RI	=	0;		
		i++;
		
	}		
	
	TEST = 1;
	

	if((verrify(out) == 1)&&(out[3] == 0x21))//У���������
	{
			r_state	=	1;//��λ�ɹ�״̬
	}	
	
	REN	=	0;
	out[0]	=	seek_data[4];
	out[1]	=	seek_data[5];
	out[2]	=	seek_data[6];
	out[3]	=	seek_data[7];
	i = 0;
	while(i < 48)
	{
		out[20 + i] =out[21 + i];	
		i++;
	}	
	
	TEST = 0;
	return r_state;	
	
}


