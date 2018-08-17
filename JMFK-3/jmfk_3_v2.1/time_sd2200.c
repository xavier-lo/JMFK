////**********************************////
//=================//
//本程序适用于
//用于JMFK-3 刷卡器
//MCU：stc89c55rd+
//晶振：11.0592MHz 震荡周期1.085us
//文件：time_sd2200.C
//================//
#include<time_sd2200.h>
#include<SHJY_IIC.h>


/********************************/
/*name:	read_time								*/
/*discription:读取时间					*/
/*var in:												*/
/*result out:out 存放读取数据		*/
/********************************/
/*年 月 日 周 时 分 秒					*/
bit read_time(unsigned char *out)
{
	unsigned char i=0;
	bit state = 0;
	if(!IIC_START())
	{
		if(!IIC_send_byte(0x65))//读年 
		{
			while(i<7)
			{
				out[i]=IIC_read_Byte();	
				IIC_send_bit(0);//应答0
				i++;
			}
			IIC_send_bit(1);//应答1
			state = 1;
		}
	}
	
	IIC_STOP();
	return state;
}

/********************************/
/*name:		set_time							*/
/*discription:	设置打电话			*/
/*var in:in 输入日期时间				*/
/*result out:状态								*/
/********************************/
bit set_time(unsigned char *in)
{
	bit state = 0;
	unsigned char xdata i = 0;
	if((in[4] >= 13) &&(in[4] <= 23)) in[4] |= 0x40;
	if(!IIC_START())
	{
		if(!IIC_send_byte(0x64))//写年月日周时分秒
		{
			while(i < 6)
			{
				if(IIC_send_byte_ltoh(in[i])) return 0;
				i++;
			}
			if(!IIC_send_byte_ltoh(0x00)) state = 1;
		}
	}
	
	IIC_STOP();	
	
	
	return state;
}


