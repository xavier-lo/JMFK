////**********************************////
//=================//
//本程序适用于
//用于JMFK-3 刷卡器
//MCU：stc89c55rd+
//晶振：29.4912MHz 震荡周期0.4069us 双倍速下载
//文件：time_sd2200.H
//================//
////**********************************////
#ifndef __TIME_SD2200_H__
#define __TIME_SD2200_H__

#include<STC_MCU.H>

/********************************/
/*name:	read_time								*/
/*discription:读取时间					*/
/*var in:												*/
/*result out:out 存放读取数据		*/
/*年 月 日 周 时 分 秒					*/
/********************************/
bit read_time(unsigned char *out);

/********************************/
/*name:		set_time							*/
/*discription:	设置打电话			*/
/*var in:in 输入日期时间				*/
/*result out:状态								*/
/********************************/
bit set_time(unsigned char *in);
	

#endif