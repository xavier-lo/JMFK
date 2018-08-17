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


