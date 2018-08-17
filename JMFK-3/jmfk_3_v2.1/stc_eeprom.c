////**********************************////
//=================//
//本程序适用于
//用于JMFK-3 刷卡器
//MCU：stc89c55rd+
//晶振：11.0592MHz 震荡周期1.085us
//文件：STC_eeprom.C
//================//
////**********************************////

#include <stc_eeprom.h>
#include <intrins.h>

#define RdCommand       0x01
#define PrgCommand      0x02
#define EraseCommand    0x03
#define WaitTime      0x00 //40M以下用0 20M以下用1 10M以下用2 5M以下用3

#define EEPROM_SATRTADDR	0x8000 //EEPROM起始地址



/********************************/
/*name:ISP_IAP_enable						*/
/*discription:打开 ISP/IAP功能	*/
/*var in:												*/
/*result out:										*/
/********************************/
void ISP_IAP_enable(void)
{
	EA	=	0;//关闭中断
//	ISP_CONTR	|=	0x18;/*0001,1000*/
	ISP_CONTR	|=	WaitTime;/*写入硬件延时*/
	ISP_CONTR	|=	0x80;/*ISPEN=1*/
}

/********************************/
/*name:ISP_IAP_disable					*/
/*discription:关闭 ISP/IAP功能	*/
/*var in:												*/
/*result out:										*/
/********************************/
void ISP_IAP_disable(void)
{
	ISP_CONTR&=0x7f;/* ISPEN = 0 */
	ISP_TRIG=0x00;
	EA=1;//开中断
}

/********************************/
/*name:ISPgoon									*/
/*discription:IAP/ISP触发				*/
/*var in:												*/
/*result out:										*/
/********************************/
void ISPgoon(void)
{
	ISP_IAP_enable();/*打开ISP/IAP功能*/
	ISP_TRIG=0x46;/* 触发ISP_IAP字节1 */
	ISP_TRIG=0xb9;/* 触发ISP_IAP字节2 */
	_nop_();
}

/********************************/
/*name:	byte_read								*/
/*discription:字节读						*/
/*var in:byte_addr 地址					*/
/*result out:ISP_DATA读出数据		*/
/********************************/
unsigned char byte_read(unsigned int byte_addr)
{
	ISP_ADDRH=(unsigned char)(byte_addr>>8); //地址赋值
	ISP_ADDRL=(unsigned char)(byte_addr&0x00ff);
	ISP_CMD&=0xf8;  //清除低三位
	ISP_CMD|=RdCommand;//写入读命令
	ISPgoon();//触发执行
	ISP_IAP_disable();//关闭
	return ISP_DATA;//返回数据
}

/********************************/
/*name:sectorerase							*/
/*discription:扇区擦除					*/
/*var in:sector_addr扇区地址		*/
/*result out:										*/
/********************************/
void sectorerase(unsigned int sector_addr)
{
	if((sector_addr % 512)	!=	0)
	{
		sector_addr = sector_addr & 0x1ff;
	}
	ISP_ADDRH=(unsigned char)(sector_addr>>8);
	ISP_ADDRL=0x00;
	ISP_CMD&=0xf8;//清除低三位
	ISP_CMD|=EraseCommand;//写入擦除命令
	ISPgoon();//触发执行
	ISP_IAP_disable();//关闭	


}


/********************************/
/*name:byte_write								*/
/*discription:字节写						*/
/*var in:byte_addr 地址					*/
/*var in:original_data写入数据	*/
/*result out:										*/
/********************************/
//void byte_write(unsigned int byte_addr, unsigned char original_data)
//{
//	ISP_ADDRH=(unsigned char)(byte_addr>>8); //地址赋值
//	ISP_ADDRL=(unsigned char)(byte_addr & 0x00ff);
//	ISP_CMD&=0xf8;//清除低三位
//	ISP_CMD|=PrgCommand;//写入写命令
//	ISP_DATA=original_data;//写入数据准备
//	ISPgoon();//触发执行
//	ISP_IAP_disable();//关闭
//}

/********************************/
/*name:byte_write_verify				*/
/*discription:字节写并校验			*/
/*var in:byte_addr 地址					*/
/*var in:original_data写入数据	*/
/*result out:写入状态						*/
/********************************/
bit byte_write_verify(unsigned int byte_addr, unsigned char original_data)
{
	ISP_ADDRH=(unsigned char)(byte_addr>>8); //地址赋值
	ISP_ADDRL=(unsigned char)(byte_addr&0xff);
	ISP_CMD&=0xf8;//清除低三位
	ISP_CMD|=PrgCommand;//写入写命令
	ISP_DATA=original_data;
	ISPgoon();//触发执行
	/*开始读 不用重复给地址 地址不会自动改变*/
	ISP_DATA=0x00;//清楚数据缓冲区寄存器
	ISP_CMD&=0xf8;//清除低三位
	ISP_CMD|=RdCommand;//写入读命令
	ISP_TRIG=0x46;//触发
	ISP_TRIG=0xb9;
	_nop_();/* 延时*/
	ISP_IAP_disable();//关闭
	if(ISP_DATA==original_data)//比较数据是否相等
		 return 1;//返回成功
	else
		 return 0;
}



/**************************************/
/*name:	arrywrite_overwrite						*/
/*discription:写入指定长度数据到eeprom*/
/*var in:start_addr 起始地址					*/
/*var in:lenth 长度										*/
/*var in:da 需要写入的数据						*/
/*result out:  写入状态								*/
/**************************************/
bit arrywrite_overwrite(unsigned int start_addr, unsigned int lenth, unsigned char *da)
{
	unsigned int in_addr,i = 0;
	bit st;
	/*禁止跨区写入*/
	if(lenth > 512)
		 return 0;
	in_addr = start_addr & 0x01ff;/* 取偏移量 */
	if((in_addr + lenth) > 512)
		 return 0;
	in_addr = start_addr;
	while(i < lenth)
	{
		st = byte_write_verify(in_addr,da[i]);
		if(!st)
		{
			return 0;
		}
		i++;
		in_addr++;
	}
	return 1;
	
}



/********************************/
/*name:	arraywrite							*/
/*discription:写入一个扇区			*/
/*var in:sector_num 扇区号			*/
/*var in:array 写入数据					*/
/*result out:写入状态						*/
/********************************/
/*此程序测试ok 可以用 屏蔽是为了节省ram*/
//bit arraywrite(unsigned char sector_num, unsigned char *array)
//{
//	unsigned int    i;
//	unsigned int    in_addr;
//	in_addr = EEPROM_SATRTADDR	+	sector_num	*	512;
//	
//	/*逐个写入并校对*/
//	ISP_IAP_enable();//打开IAP功能
//	/*一个扇区512字节*/
//	for(i=0;i < 512;i++)
//		{
//		 /* 写一个字节 */
//		 ISP_ADDRH=(unsigned char)(in_addr >> 8);
//		 ISP_ADDRL=(unsigned char)(in_addr & 0x00ff);
//		 ISP_DATA=array[i];   /* 取数据     */
//		 ISP_CMD&=0xf8;/* 清楚低3位       */
//		 ISP_CMD|=PrgCommand;/*写入写命令*/
//		 ISP_TRIG=0x46;//触发
//		 ISP_TRIG=0xb9;
//		 _nop_();
//		 /* 读一个字节 */
//		 ISP_DATA=0x00;
//		 ISP_CMD&=0xf8;/* 清楚低3位 */
//		 ISP_CMD|=RdCommand;/* 写入读数据*/
//		 ISP_TRIG=0x46;//触发
//		 ISP_TRIG=0xb9;
//		 _nop_();
//		 /*  比较相等 */
//		 if(ISP_DATA!=array[i])
//			{
//			 ISP_IAP_disable();
//			 return 0;
//			}
//			in_addr++;/* 指向下一个字节*/
//		 }
//		 ISP_IAP_disable();
//		 return 1;
//}

/********************************/
/*name:arrayread								*/
/*discription:读一个扇区数据		*/
/*var in:sector_num 扇区号			*/
/*result out:										*/
/********************************/
/*此程序测试ok 可以用 屏蔽是为了节省ram*/
//void arrayread(unsigned char sector_num, unsigned char *out)
//{
//	unsigned int iSectorAddr;
//	unsigned int i;
//	iSectorAddr = EEPROM_SATRTADDR	+	sector_num	*	512;     // & 0xfe00; /*取扇区地址*/
//	ISP_IAP_enable();
//	/*一个扇区512字节*/
//	for(i=0;i<512;i++)
//	{
//		ISP_ADDRH=(unsigned char)(iSectorAddr>>8);
//		ISP_ADDRL=(unsigned char)(iSectorAddr & 0x00ff);
//		ISP_CMD&=0xf8;/* 清楚低3位 */
//		ISP_CMD|=RdCommand;/* 写入读命令*/
//		ISP_DATA=0;
//		ISP_TRIG=0x46;//触发
//		ISP_TRIG=0xb9;
//		_nop_();
//		out[i]=ISP_DATA;
//		iSectorAddr++;
//	}
//	ISP_IAP_disable();//关闭
//}


/********************************/
/*name:arrayread_overwirte			*/
/*discription:读指定长度数据		*/
/*var in:start_addr 起始地址		*/
/*var in:lenth 长度							*/
/*result out:										*/
/********************************/
void arrayread_overwirte(unsigned int start_addr, unsigned int lenth, unsigned char *out)
{
	unsigned int iSectorAddr;
	unsigned int i;
	iSectorAddr = start_addr;
	ISP_IAP_enable();
	/*一个扇区512字节*/
	for(i=0;i<lenth;i++)
	{
		out[i]=byte_read(iSectorAddr);
		iSectorAddr++;
	}
	ISP_IAP_disable();//关闭
}
