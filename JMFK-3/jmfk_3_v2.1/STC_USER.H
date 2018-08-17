////**********************************////
//=================//
//本程序适用于
//用于JMFK-3 刷卡器
//MCU：stc89c55rd+
//晶振：11.0592MHz 震荡周期1.085us
//文件：STC_USER.H
//================//
////**********************************////
#ifndef __STC_USER_H__
#define __STC_USER_H__

#include<STC_MCU.H>
void action(void);

/********************************/
/*name:	output_control					*/
/*discription:输出控制到74hc373	*/
/*var in:da 控制数据 4字节			*/
/*result out:										*/
/********************************/
void output_control(unsigned char xdata *da );

/**********************************************************/
/*name:read_eeprom																				*/
/*discription:开机读取扇区号 群号 梯号 短路点并输出至接口	*/
/*var in:																									*/
/*result out:																							*/
/**********************************************************/
void read_eeprom(void);

/********************************/
/*name:	operate_managmentcard		*/
/*discription:管理卡操作				*/
/*var in:	rd_da 缓存数据				*/
/*var in:	now_date 日期					*/
/*var in:	now_time 时间					*/
/*var in:	wk	周								*/
/*result out:										*/
/********************************/
void operate_managmentcard(unsigned char xdata *rd_da, unsigned char xdata *now_date, unsigned char xdata *now_time, unsigned char wk);

/********************************/
/*name:		seek_eeprom_isn				*/
/*discription: 比对isn是否有效	*/
/*var in:		isn 需要检查的isn		*/
/*result out:	检查状态					*/
/********************************/
bit seek_eeprom_isn(unsigned int sector, unsigned char xdata *isn);

/********************************/
/*name:	operate_usercard				*/
/*discription:用户卡操作				*/
/*var in:		rd_da 缓存数据			*/
/*var in:		now_dat 实时日期		*/
/*var in:		now_tim 实时时间		*/
/*result out:										*/
/********************************/
void operate_usercard(unsigned char xdata *rd_da,unsigned char xdata *now_dat, unsigned char xdata *now_tim);

/****************************************/
/*name:check_time												*/
/*discription:检查时间是否有效					*/
/*var in:年月日时间（范围时间 现在时间）*/
/*result out:状态												*/
/****************************************/
bit check_time(unsigned char xdata *bg_dt, unsigned char xdata *ed_dt, unsigned char xdata *bg_t0,
								unsigned char xdata *ed_t0,unsigned char xdata *bg_t1, unsigned char xdata *ed_t1,  
								unsigned char xdata *bg_t2,unsigned char xdata *ed_t2,unsigned char *now_dt, unsigned char xdata *now_t);

#endif