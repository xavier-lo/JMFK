////**********************************////
//=================//
//本程序适用于
//用于JMFK-3 刷卡器
//MCU：stc89c55rd+
//晶振：29.4912MHz 震荡周期0.4069us 双倍速下载
//文件：operate_card.C
//此文件使用单片机外设串口1
//================//
#include <operate_card.h>

#define card_delay 12000

sbit BEEP	=	P3^7;//蜂鸣器

sbit TEST = P1^4;

sbit CardHead_ERR_Lamp = P4^2;

/**********************************/
/*name:make_verr									*/
/*discription:产生校验						*/
/*var in:da0 需要生成的数据列			*/
/*result out:	da0 生成后的数据列	*/
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
/*discription:校验接收数据			*/
/*var in:da0 需要校验的数据			*/
/*result out:bool 状态					*/
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
				ver_da ^= da0[i];//异或校验数据
				i++;
			}
			if(ver_da == 0)//结果为0校验成功
				states = 1;
		}		
	}
	return states;
}


/**********************************************/
/*name:seek_card															*/
/*discription:寻卡														*/
/*var in:																			*/
/*result out:recive_da 返回数据	传入必须9字节	*/
/**********************************************/
char seek_card(unsigned char *recive_da)
{
	char state	=	0;
	unsigned char i	=	0;
	unsigned int recive_count	=	0;
	/*
	发送寻卡数据到卡头
	6个字节
	*/

	recive_da[0]=0xaa;
	recive_da[1]=0x55;
	recive_da[2]=0x03;
	recive_da[3]=0x20;
	recive_da[4]=0x00;	
	
	make_verr(recive_da);//生成校验字
	i = 0;
	while(i < 6)//发送
	{
		TI = 0;
		SBUF = recive_da[i];
		i++;
		while(!TI);	
	}
	RI = 0;
	REN = 1;
	i = 0;	
	
	while(i < 9)//接收数据最多接收9个字节
	{	
		recive_count	=	0;			
		while(!RI)
		{
			recive_count++;
			if(recive_count>=card_delay)
			{
				REN	=	0;
				return state;//超时退出
			}
		}
		recive_da[i] = SBUF;
		RI	=	0;		
		i++;		
	}
	
	if((verrify(recive_da) == 1)&&(recive_da[2] == 0x06))//校验接收数据
	{
			state	=	1;//置位成功状态
			CardHead_ERR_Lamp = 0;
	}
	else CardHead_ERR_Lamp = 1;
	REN	=	0;
	return state;
}

/******************************************/
/*name:	write_card												*/
/*discription:写数据到块使用卡头密码			*/
/*var in:block 块号												*/
/*var in:da 需要写的数据16字节传入25字节	*/
/*result out:state 写卡状态								*/
/******************************************/
bit write_card(unsigned char block,unsigned char *da)
{
	unsigned char i	=	0, seek_data[9];
	unsigned int recive_count	=	0 ;
	bit state	=	0 ;

	/*寻卡*/
	if(seek_card(seek_data)	==	0)
	{
		return state;//无卡退出
	}
	
	/*
	填充数据到发送区
	*/
	da[0]	=	0xaa;
	da[1]	=	0x55;
	da[2]	=	0x16;
	da[3]	=	0x22;
	da[4]	=	0x02;
	da[5]	=	block;
	da[6]	=	0x80;
	da[7]	=	0x00;

	make_verr(da);//生成校验
	
	/* 发送 */
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
			if(recive_count >= card_delay/6 ) return 0;//超时退出
		}	
	}	
	/*
	接收返回状态
	*/
	RI = 0;
	i = 0;
	REN = 1;
	while(i < 5)//接收数据最多接收5个字节
	{	
		recive_count	=	0;			
		while(!RI)
		{
			recive_count++;
			if(recive_count>=card_delay) 
			{
				REN=0;
				return state;//超时退出
			}
		}
		da[i] = SBUF;
		RI	=	0;		
		i++;
		
	}
	if((verrify(da) == 1)&&(da[3] == 0x22))//校验接收数据
	{
			state	=	1;//置位成功状态
	}
	REN	=	0;	
	return state;
}



/********************************/
/*name:read_card_1block					*/
/*discription:读出1块数据				*/
/*var in:读出16字节 传入21字节	*/
/*result out:										*/
/********************************/
bit read_card_1block(unsigned char block,unsigned char *out)
{
	unsigned char i	=	0;
	unsigned int recive_count1 = 0;
	unsigned char seek_data[9];
	bit state	=	0	;
	/*寻卡*/
	if(seek_card(seek_data)	==	0)
	{
		return state;//无卡退出
	}
	/*
	填充数据到发送区
	*/
	out[0]	=	0xaa;
	out[1]	=	0x55;
	out[2]	=	0x06;
	out[3]	=	0x21;
	out[4]	=	0x02;
	out[5]	=	block;
	out[6]	=	0x80;
	out[7]	=	0x00;	
	make_verr(out);//生成校验
	
	/*发送*/
	i = 0;
	while(i < 9)
	{
		TI = 0;
		SBUF = out[i];
		i++;
		while(!TI);	
	}	
	/*
	接收返回状态
	*/
	RI = 0;
	i = 0;
	REN = 1;
	while(i < 21)//接收数据最多接收21个字节
	{	
		recive_count1	=	0;			
		while(!RI)
		{
			recive_count1++;
			if(recive_count1>=card_delay)
			{
				REN=0;
				return state;//超时退出
			}
		}
		out[i] = SBUF;
		RI	=	0;		
		i++;
		
	}		
	
	if((verrify(out) == 1)&&(out[3] == 0x21))//校验接收数据
	{
			state	=	1;//置位成功状态
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
/*discription:读某一块数据									*/
/*var in:	sector 扇区号											*/
/*result out:	out 读出数据48字节传入53字节	*/
/*result out:	state 状态										*/
/********************************************/
bit read_card(unsigned char sector, unsigned char *out)
{
	unsigned char i	=	0;
	unsigned int recive_count1 = 0;
	unsigned char seek_data[9];
	bit r_state	=	0	;
	/*寻卡*/
	if(seek_card(seek_data)	==	0)
	{
		return r_state;//无卡退出
	}
	
	/*
	填充数据到发送区
	*/
	out[0]	=	0xaa;
	out[1]	=	0x55;
	out[2]	=	0x06;
	out[3]	=	0x21;
	out[4]	=	0x02;
	out[5]	=	sector*4+2;
	out[6]	=	0x80;
	out[7]	=	0x00;	
	make_verr(out);//生成校验
	
	/*发送*/
	i = 0;
	while(i < 9)
	{
		TI = 0;
		SBUF = out[i];
		i++;
		while(!TI);	
	}	
	/*
	接收返回状态
	*/
	RI = 0;
	i = 0;
	REN = 1;
	while(i < 21)//接收数据最多接收21个字节
	{	
		recive_count1	=	0;			
		while(!RI)
		{
			recive_count1++;
			if(recive_count1>=card_delay)
			{
				REN=0;
				return r_state;//超时退出
			}
		}
		out[i] = SBUF;
		RI	=	0;		
		i++;
		
	}
	if((verrify(out) != 1)	||	(out[3] != 0x21))//校验接收数据
	{
		r_state	=	0;
		return r_state;
	}
	/*映射到最后16字节*/
	i	=	0;
	while(i < 16)
	{
		out[37 + i]	= out[4 + i];
		i++;
	}
	
	/*
	填充数据到发送区
	*/
	out[0]	=	0xaa;
	out[1]	=	0x55;
	out[2]	=	0x06;
	out[3]	=	0x21;
	out[4]	=	0x02;
	out[5]	=	sector*4+1;
	out[6]	=	0x80;
	out[7]	=	0x00;	
	make_verr(out);//生成校验
	
	/*发送*/
	i = 0;
	while(i < 9)
	{
		TI = 0;
		SBUF = out[i];
		i++;
		while(!TI);	
	}	
	/*
	接收返回状态
	*/
	RI = 0;
	i = 0;
	REN = 1;
	while(i < 21)//接收数据最多接收21个字节
	{	
		recive_count1	=	0;			
		while(!RI)
		{
			recive_count1++;
			if(recive_count1 >= card_delay)
			{
				REN=0;
				return r_state;//超时退出
			}
		}
		out[i] = SBUF;
		RI	=	0;		
		i++;
		
	}

	if((verrify(out) != 1)	||	(out[3] != 0x21))//校验接收数据
	{
		r_state	=	0;
		return r_state;
	}
	/*映射到中间16字节*/
	i	=	0;
	while(i < 16)
	{
		out[21 + i]	= out[4 + i];
		i++;
	}
	
	/*
	填充数据到发送区
	*/
	out[0]	=	0xaa;
	out[1]	=	0x55;
	out[2]	=	0x06;
	out[3]	=	0x21;
	out[4]	=	0x02;
	out[5]	=	sector*4+0;
	out[6]	=	0x80;
	out[7]	=	0x00;	
	make_verr(out);//生成校验
	
	/*发送*/
	i = 0;
	while(i < 9)
	{
		TI = 0;
		SBUF = out[i];
		i++;
		while(!TI);	
	}	
	/*
	接收返回状态
	*/
	RI = 0;
	i = 0;
	REN = 1;
	while(i < 21)//接收数据最多接收21个字节
	{	
		recive_count1	=	0;			
		while(!RI)
		{
			recive_count1++;
			if(recive_count1>=card_delay)
			{
				REN=0;
				r_state = 0;
				return r_state;//超时退出
			}
		}
		out[i] = SBUF;
		RI	=	0;		
		i++;
		
	}		
	
	TEST = 1;
	

	if((verrify(out) == 1)&&(out[3] == 0x21))//校验接收数据
	{
			r_state	=	1;//置位成功状态
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
/*discription:读用户卡					*/
/*var in:	in 扇区号							*/
/*var in:	out 传入69个字节			*/
/*result out:	读出状态					*/
/********************************/
bit read_user_card(unsigned char sector, unsigned char *out)
{
	unsigned char i	=	0;
	unsigned int recive_count1 = 0;
	unsigned char seek_data[9];
	bit r_state	=	0	;
	/*寻卡*/
	if(seek_card(seek_data)	==	0)
	{
		return r_state;//无卡退出
	}
	
	/*
	填充数据到发送区
	*/
	out[0]	=	0xaa;
	out[1]	=	0x55;
	out[2]	=	0x06;
	out[3]	=	0x21;
	out[4]	=	0x02;
	out[5]	=	(sector + 1) * 4 + 1;
	out[6]	=	0x80;
	out[7]	=	0x00;	
	make_verr(out);//生成校验
	
	/*发送*/
	i = 0;
	while(i < 9)
	{
		TI = 0;
		SBUF = out[i];
		i++;
		while(!TI);	
	}	
	/*
	接收返回状态
	*/
	RI = 0;
	i = 0;
	REN = 1;
	while(i < 21)//接收数据最多接收21个字节
	{	
		recive_count1	=	0;			
		while(!RI)
		{
			recive_count1++;
			if(recive_count1>=card_delay)
			{
				REN=0;
				return r_state;//超时退出
			}
		}
		out[i] = SBUF;
		RI	=	0;		
		i++;
		
	}
	if((verrify(out) != 1)	||	(out[3] != 0x21))//校验接收数据
	{
		r_state	=	0;
		return r_state;
	}
	/*映射到最后16字节*/
	i	=	0;
	while(i < 16)
	{
		out[53 + i]	= out[4 + i];
		i++;
	}
	
	/*
	填充数据到发送区
	*/
	out[0]	=	0xaa;
	out[1]	=	0x55;
	out[2]	=	0x06;
	out[3]	=	0x21;
	out[4]	=	0x02;
	out[5]	=	(sector + 1) * 4 + 0;
	out[6]	=	0x80;
	out[7]	=	0x00;	
	make_verr(out);//生成校验
	
	/*发送*/
	i = 0;
	while(i < 9)
	{
		TI = 0;
		SBUF = out[i];
		i++;
		while(!TI);	
	}	
	/*
	接收返回状态
	*/
	RI = 0;
	i = 0;
	REN = 1;
	while(i < 21)//接收数据最多接收21个字节
	{	
		recive_count1	=	0;			
		while(!RI)
		{
			recive_count1++;
			if(recive_count1>=card_delay)
			{
				REN=0;
				return r_state;//超时退出
			}
		}
		out[i] = SBUF;
		RI	=	0;		
		i++;
		
	}
	if((verrify(out) != 1)	||	(out[3] != 0x21))//校验接收数据
	{
		r_state	=	0;
		return r_state;
	}
	/*映射到后16字节*/
	i	=	0;
	while(i < 16)
	{
		out[37 + i]	= out[4 + i];
		i++;
	}	

	/*
	填充数据到发送区
	*/
	out[0]	=	0xaa;
	out[1]	=	0x55;
	out[2]	=	0x06;
	out[3]	=	0x21;
	out[4]	=	0x02;
	out[5]	=	sector * 4 + 2;
	out[6]	=	0x80;
	out[7]	=	0x00;	
	make_verr(out);//生成校验
	
	/*发送*/
	i = 0;
	while(i < 9)
	{
		TI = 0;
		SBUF = out[i];
		i++;
		while(!TI);	
	}	
	/*
	接收返回状态
	*/
	RI = 0;
	i = 0;
	REN = 1;
	while(i < 21)//接收数据最多接收21个字节
	{	
		recive_count1	=	0;			
		while(!RI)
		{
			recive_count1++;
			if(recive_count1>=card_delay)
			{
				REN=0;
				return r_state;//超时退出
			}
		}
		out[i] = SBUF;
		RI	=	0;		
		i++;
		
	}
	if((verrify(out) != 1)	||	(out[3] != 0x21))//校验接收数据
	{
		r_state	=	0;
		return r_state;
	}
	/*映射到最后16字节*/
	i	=	0;
	while(i < 16)
	{
		out[21 + i]	= out[4 + i];
		i++;
	}	

	/*
	填充数据到发送区
	*/
	out[0]	=	0xaa;
	out[1]	=	0x55;
	out[2]	=	0x06;
	out[3]	=	0x21;
	out[4]	=	0x02;
	out[5]	=	sector*4+1;
	out[6]	=	0x80;
	out[7]	=	0x00;	
	make_verr(out);//生成校验
	
	/*发送*/
	i = 0;
	while(i < 9)
	{
		TI = 0;
		SBUF = out[i];
		i++;
		while(!TI);	
	}	
	/*
	接收返回状态
	*/
	RI = 0;
	i = 0;
	REN = 1;
	while(i < 21)//接收数据最多接收21个字节
	{	
		recive_count1	=	0;			
		while(!RI)
		{
			recive_count1++;
			if(recive_count1>=card_delay)
			{
				REN=0;
				r_state = 0;
				return r_state;//超时退出
			}
		}
		out[i] = SBUF;
		RI	=	0;		
		i++;
		
	}		
	
	TEST = 1;
	

	if((verrify(out) == 1)&&(out[3] == 0x21))//校验接收数据
	{
			r_state	=	1;//置位成功状态
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


