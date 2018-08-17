////**********************************////
//=================//
//本程序适用于
//用于JMFK-3 刷卡器
//MCU：stc89c55rd+
//晶振：11.0592MHz 震荡周期1.085us
//文件：STC_USER.C
//================//
////**********************************////
#include <STC_USER.H>
#include <STC_INIT.H>
#include <stc_eeprom.h>
#include <intrins.h>
#include <operate_card.h>
#include <time_sd2200.h>

#define max_id_amount 7000
#define EEPROM_BASE_ADDR 0x8000

/*****定义局部字节变量*****///
volatile unsigned char xdata eeprom_data[512];//eeprom扇区缓冲	
unsigned char IC_sector;//用户卡扇区
unsigned char IC_group;//用户卡群号
unsigned char IC_elv;//用户卡梯号
unsigned char xdata open_card_state = 0x00;

//bit SA1,SA2,SA3;//地址码1 2 3 

sbit SA1 = P0^3;
sbit SA2 = P0^2;
sbit SA3 = P0^1;

/***/

/*****定义局部位变量*****/
sbit T1_SEL	=	P1^0;
sbit T2_SEL	=	P1^1;
sbit T3_SEL	=	P1^2;
sbit T4_SEL	=	P1^3;

sbit BEEP	=	P3^7;//蜂鸣器
/***/


/*全局变量定义*/
extern bit delay40ms_ready;
extern bit beep_start;

volatile bit delay_5s_start = 0;//用户卡有效5s开始
volatile bit delay_5s_stop = 0;//用户卡有效5s结束
volatile bit repeat_bit = 0;//重复读卡计数开始
volatile bit repeat_bit1 = 0;//重复读卡计数开始
volatile bit delay1s_ready = 0;//读取时间周期1s

volatile unsigned char pre_IC_ISN[4];//防止短时间重复读卡 
volatile unsigned char pre_IC_ISN1[4];//防止短时间重复读卡
extern unsigned char t0_count2;
extern unsigned char t0_count4;
extern unsigned char t0_count3;

/**/


/********************************/
/*name:													*/
/*discription:									*/
/*var in:												*/
/*result out:										*/
/********************************/
void action(void)
{
	unsigned char xdata read_data[53];
	unsigned int de_count=0;
	unsigned char function_count = 0;//
	unsigned char xdata now_date[3],now_time[2];


	arrayread_overwirte(EEPROM_BASE_ADDR, 9, eeprom_data);//读取短路点数据
	output_control(eeprom_data);
	while(1)
	{	
		if(delay40ms_ready)
		{
			delay40ms_ready = 0;
			switch(function_count)
			{
				case 0://寻管理卡 管理卡统一定义在IC卡0扇区开始
					operate_managmentcard(read_data);
					function_count = 1;
					break;
				case 1://寻用户卡 用户卡扇区号由用户定义写入到mcu eeprom
					operate_usercard(read_data,now_date,now_time);
					function_count = 0;
					break;
				default:
					function_count = 0;
					break;
			
			}
		}else if(delay1s_ready)
		{//读时间
			delay1s_ready = 0;
			if(read_time(read_data) == 1)
			{
				now_date[0] = read_data[0];//年
				now_date[1] = read_data[1] & 0x1f;//月
				now_date[2] = read_data[2] & 0x3f;//日
				now_time[0] = read_data[4] & 0x3f;//时
				now_time[1] = read_data[5] & 0x7f;//分	
			}
		}
		
		if(delay_5s_stop)
		{//用户卡有效结束
			delay_5s_stop = 0;
			arrayread_overwirte(EEPROM_BASE_ADDR,8,eeprom_data);//
			output_control(&eeprom_data[4*0]);		
		}
	
	}
	
}


/****************************************/
/*name:check_time												*/
/*discription:检查时间是否有效					*/
/*var in:年月日时间（范围时间 现在时间）*/
/*result out:状态												*/
/****************************************/
bit check_time(unsigned char xdata *bg_dt, unsigned char xdata *ed_dt, unsigned char xdata *bg_t0,
								unsigned char xdata *ed_t0,unsigned char xdata *bg_t1, unsigned char xdata *ed_t1,  
								unsigned char xdata *bg_t2,unsigned char xdata *ed_t2,unsigned char *now_dt, unsigned char xdata *now_t)
{
	unsigned char xdata t_sta = 0xff;
	unsigned long xdata bg_date,ed_date,now_date;
	unsigned int xdata bg_time0,bg_time1,bg_time2,ed_time0,ed_time1,ed_time2,now_time;
	
	bg_date = (((unsigned long)bg_dt[0] << 16) & 0xff0000) + (((unsigned long)bg_dt[1] << 8) & 0xff00) + bg_dt[2];
	ed_date = (((unsigned long)ed_dt[0] << 16) & 0xff0000) + (((unsigned long)ed_dt[1] << 8) & 0xff00) + ed_dt[2];
	now_date = (((unsigned long)now_dt[0] << 16) & 0xff0000) + (((unsigned long)now_dt[1] << 8) & 0xff00) + now_dt[2];		
	
	//判断日期
	if((now_date < bg_date) || (now_date > ed_date)) return 0;
	
	bg_time0 = ((unsigned int)(bg_t0[0] << 8) & 0xff00) + bg_t0[1];
	bg_time1 = ((unsigned int)(bg_t1[0] << 8) & 0xff00) + bg_t1[1];
	bg_time2 = ((unsigned int)(bg_t2[0] << 8) & 0xff00) + bg_t2[1];
	ed_time0 = ((unsigned int)(ed_t0[0] << 8) & 0xff00) + ed_t0[1];
	ed_time1 = ((unsigned int)(ed_t1[0] << 8) & 0xff00) + ed_t1[1];
	ed_time2 = ((unsigned int)(ed_t2[0] << 8) & 0xff00) + ed_t2[1];
	now_time = ((unsigned int)(now_t[0] << 8) & 0xff00) + now_t[1];	
	
	//判断时间
	t_sta = 0x00;
	if((now_time < bg_time0) || (now_time > ed_time0)) t_sta |= 0x01;
	if((now_time < bg_time1) || (now_time > ed_time1)) t_sta |= 0x02;
	if((now_time < bg_time2) || (now_time > ed_time2)) t_sta |= 0x04;
	
	if(t_sta == 0x07 ) return 0;
	return 1;
}


/********************************/
/*name:		seek_eeprom_isn				*/
/*discription: 比对isn是否有效	*/
/*var in:		isn 需要检查的isn		*/
/*result out:	检查状态					*/
/********************************/
bit seek_eeprom_isn(unsigned char xdata *isn)
{
	unsigned int xdata i = 0;
	unsigned char xdata read_isn[4];
	while(i < max_id_amount)
	{
		arrayread_overwirte(EEPROM_BASE_ADDR+0x200 + 4 * i,4,read_isn);	
		if((isn[0] == read_isn[0]) && (isn[1] == read_isn[1]) && (isn[2] == read_isn[2]) && (isn[3] == read_isn[3]))
			return 1;
		i++;
	}
	return 0;
}


/********************************/
/*name:	operate_usercard				*/
/*discription:用户卡操作				*/
/*var in:		rd_da 缓存数据			*/
/*var in:		now_dat 实时日期		*/
/*var in:		now_tim 实时时间		*/
/*result out:										*/
/********************************/
void operate_usercard(unsigned char xdata *rd_da,unsigned char xdata *now_dat, unsigned char xdata *now_tim)
{
	unsigned char xdata ii;
	/*有效开始日期 有效结束日期 有效开始时间0 1 2 有效结束时间0 1 2 有效次数*/
	unsigned char xdata begin_date[3],end_date[3],begin_time0[2],end_time0[2],begin_time1[2],end_time1[2],begin_time2[2],end_time2[2];
	unsigned long xdata valid_count;
	unsigned char xdata temp_data[53];
	/*读序列号块*/
	if(read_card(IC_sector+1,rd_da) == 1)
	{		
		if((pre_IC_ISN1[0] != rd_da[0])||(pre_IC_ISN1[1] != rd_da[1])||(pre_IC_ISN1[2] != rd_da[2])||(pre_IC_ISN1[3] != rd_da[3]))
		{//防止重复读取
			ii = 0;
			while(ii<4)
			{
				pre_IC_ISN1[ii] = rd_da[ii];
				ii++;
			}	
			repeat_bit1 = 1;//重复刷卡计数开始
			
			if(!seek_eeprom_isn(&rd_da[4])) return;//检查isn是否有效 无效直接退出函数
			ii = 0;
			while(ii < 3)
			{//保存有效结束日期
				begin_date[ii] = ((rd_da[8 + 2 * ii] << 4) & 0xf0) | (rd_da[9 + 2 * ii] & 0x0f);
				end_date[ii] = ((rd_da[14 + 2 * ii] << 4) & 0xf0) | (rd_da[15 + 2 * ii] & 0x0f);
				ii++;
			}
			ii = 0;
			while(ii < 2)
			{//保存有效开始时间012 结束时间012
				begin_time0[ii] = rd_da[20 + ii];
				begin_time1[ii] = rd_da[24 + ii];
				begin_time2[ii] = rd_da[28 + ii];
				end_time0[ii] = rd_da[22 + ii];	
				end_time1[ii] = rd_da[26 + ii];
				end_time2[ii] = rd_da[30 + ii];	
				ii++;
			}
			
			//保存有效次数
			valid_count = (((unsigned long)rd_da[32] << 24) & 0xff000000) + (((unsigned long)rd_da[33] << 16) & 0xff0000) + (((unsigned long)rd_da[34] << 8) & 0xff00) + rd_da[35];
			
			
			/*检查时间是否有效*/
			if(check_time(begin_date,end_date,begin_time0,end_time0,begin_time1,end_time1,begin_time2,end_time2,now_dat,now_tim) == 0) return;

			/*读控制数据块*/
			if(read_card(IC_sector,rd_da) == 1)
			{
				if((rd_da[21] == (((IC_group << 5) & 0xe0)|(IC_elv&0x1f))) ||(rd_da[21] == ((IC_group << 5) & 0xe0)) || (rd_da[21] == 0))
				{//群号 梯号相等 或群号相等梯号为0  或群号梯号为0

					if((rd_da[20] & 0x10) == 0x10)//次卡
					{
						if(valid_count <= 0) return;//次数为0退出
						if(read_card(IC_sector+1,temp_data) == 1)
						{
							valid_count -= 1;
							temp_data[32] = (valid_count >> 24) & 0xff;
							temp_data[33] = (valid_count >> 16) & 0xff;
							temp_data[34] = (valid_count >> 8) & 0xff;
							temp_data[35] = valid_count & 0xff;
							
							if(write_card((IC_sector + 1) * 4 + 1, &temp_data[12]) != 1) return;//未写成功退出
							
						}else return;//未读成功退出
					}
					
					if((rd_da[20] & 0xe0) == 0)//用户卡 刷卡保持5s							
					{
						/*输出至控制点 短路点*/
						arrayread_overwirte(EEPROM_BASE_ADDR,8,eeprom_data);//
						ii = 0;
						while(ii < 8)
						{
							rd_da[36 + ii] = rd_da[36 + ii] | eeprom_data[ii];
							ii++;
						}
						output_control(&rd_da[36]);
						beep_start = 1;
						delay_5s_start = 1;
						t0_count3 = 0;
						open_card_state = 0x00;//用户卡清零
					}else if((rd_da[20] & 0xe0) == 0x80)//用户开关卡 刷完即刻反向输出 
					{
						/*输出至控制点 短路点*/
						open_card_state = ~open_card_state;
						arrayread_overwirte(EEPROM_BASE_ADDR,8,eeprom_data);//
						ii = 0;
						while(ii < 8)
						{
							rd_da[36 + ii] = (rd_da[36 + ii] & open_card_state) | eeprom_data[ii];
							ii++;
						}
						output_control(&rd_da[36]);
						beep_start = 1;
					}					
				}			
			}
		}	
	}else
	{//无卡清掉序列号
			t0_count4 = 0;
		  repeat_bit1 = 0;
			pre_IC_ISN1[10]=0;
			pre_IC_ISN1[1]=0;
			pre_IC_ISN1[2]=0;
			pre_IC_ISN1[3]=0;
	}	

}


/********************************/
/*name:	operate_managmentcard		*/
/*discription:管理卡操作				*/
/*var in:	rd_da 缓存数据				*/
/*result out:										*/
/********************************/
void operate_managmentcard(unsigned char xdata *rd_da)
{
	unsigned int xdata ii,jj,kk;//用于计数
	unsigned int xdata IC_eeprom_addr,IC_Byte_num;
	if(read_card(0x00,rd_da) == 1)
	{
		if((pre_IC_ISN[0] != rd_da[0])||(pre_IC_ISN[1] != rd_da[1])||(pre_IC_ISN[2] != rd_da[2])||(pre_IC_ISN[3] != rd_da[3]))
		{//防止重复读取
			ii = 0;
			while(ii<4)
			{
				pre_IC_ISN[ii] = rd_da[ii];
				ii++;
			}	
			repeat_bit = 1;//重复刷卡计数开始
			IC_eeprom_addr = ((((unsigned int)rd_da[22] <<8) & 0xff00) | rd_da[23]);//获取需要写入eeprom地址
			IC_Byte_num = ((((unsigned int)rd_da[24] <<8) & 0xff00) | rd_da[25]);//获取需要写入序列号长度
			if((rd_da[20] & 0x60) == 0x40)//写管理卡短路点
			{
				open_card_state = 0x00;//管理卡清零
				
				sectorerase(IC_eeprom_addr);//擦除扇区
				if(arrywrite_overwrite(IC_eeprom_addr,8,&rd_da[44]) == 1)//写入短路点 
				{
					if(!SA1)
					{	//写入群号 梯号 用户卡扇区号								
						if(arrywrite_overwrite(IC_eeprom_addr+8,2,&rd_da[26]) == 1)
						{
							beep_start = 1;
						}											
					}else
							{		
								eeprom_data[0] = ((IC_group << 5) & 0xe0)|(IC_elv&0x1f);
								eeprom_data[1] = IC_sector;
								beep_start = 1;
								if(arrywrite_overwrite(IC_eeprom_addr+8,2,eeprom_data) == 1);												
							}					
					/*输出至短路点*/
					arrayread_overwirte(EEPROM_BASE_ADDR,8,eeprom_data);//
					output_control(&eeprom_data[4*0]);
				}							
			}
			if((rd_da[20] & 0x60) == 0x60)//写序列号
			{
				open_card_state = 0x00;//管理卡清零
				
				/*获取IC卡序列号*/
				if(IC_Byte_num < 128)//长度必须小于128
				{
					jj = IC_Byte_num / 12;
					if((IC_Byte_num % 12) != 0) jj += 1;
					ii = 0;					
					while(ii+1 < jj+1)//从ic卡第2扇区开始读出
					{
						if(read_card(ii + 1,rd_da) != 1)
						{//发现某一扇区未读出退出
							t0_count2 = 0;
							repeat_bit = 0;
							pre_IC_ISN[0]=0;
							pre_IC_ISN[1]=0;
							pre_IC_ISN[2]=0;
							pre_IC_ISN[3]=0;	
							return ;
						}
						kk = 0;
						while(kk < 48)
						{
							eeprom_data[48 * ii + kk] = rd_da[4 + kk];
							kk++;
						}
						ii++;
					}			
					sectorerase(IC_eeprom_addr);//擦除扇区
					if(arrywrite_overwrite(IC_eeprom_addr,IC_Byte_num*4,eeprom_data) == 1)
					{
							beep_start = 1;
					}
				}
			}							
		}
	}else
	{//无卡清掉序列号
			t0_count2 = 0;
		  repeat_bit = 0;
			pre_IC_ISN[0]=0;
			pre_IC_ISN[1]=0;
			pre_IC_ISN[2]=0;
			pre_IC_ISN[3]=0;	
	}		
}


/**********************************************************/
/*name:read_eeprom																				*/
/*discription:开机读取扇区号 群号 梯号 短路点并输出至接口	*/
/*var in:																									*/
/*result out:																							*/
/**********************************************************/
void read_eeprom(void)
{
	arrayread_overwirte(EEPROM_BASE_ADDR,10,eeprom_data);//
	IC_sector = eeprom_data[9]; //用户卡扇区号
	IC_group = ((eeprom_data[8] >> 5) & 0x07);//群号
	IC_elv = (eeprom_data[8] & 0x1f);//梯号
	output_control(&eeprom_data[4*0]);
}



/********************************/
/*name:	output_control					*/
/*discription:输出控制到74hc373	*/
/*var in:da 控制数据 4字节			*/
/*result out:										*/
/********************************/
void output_control(unsigned char xdata  *da )
{
		/*先锁住*/
		T1_SEL	=	0;
		T2_SEL	=	0;
		T3_SEL	=	0;
		T4_SEL	=	0;
		/*变化数据1*/
		T1_SEL	=	1;
		_nop_();
		P2	=	da[0];
		T1_SEL	=	0;
		/*变化数据2*/
		T2_SEL	=	1;
		_nop_();	
		P2	=	da[1];
		T2_SEL	=	0;
		/*变化数据3*/
		T3_SEL	=	1;
		_nop_();
		P2	=	da[2];
		T3_SEL	=	0;
		/*变化数据4*/
		T4_SEL	=	1;
		_nop_();
		P2	=	da[3];
		T4_SEL	=	0;	
}

