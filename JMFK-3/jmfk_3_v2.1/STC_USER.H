////**********************************////
//=================//
//������������
//����JMFK-3 ˢ����
//MCU��stc89c55rd+
//����11.0592MHz ������1.085us
//�ļ���STC_USER.H
//================//
////**********************************////
#ifndef __STC_USER_H__
#define __STC_USER_H__

#include<STC_MCU.H>
void action(void);

/********************************/
/*name:	output_control					*/
/*discription:������Ƶ�74hc373	*/
/*var in:da �������� 4�ֽ�			*/
/*result out:										*/
/********************************/
void output_control(unsigned char xdata *da );

/**********************************************************/
/*name:read_eeprom																				*/
/*discription:������ȡ������ Ⱥ�� �ݺ� ��·�㲢������ӿ�	*/
/*var in:																									*/
/*result out:																							*/
/**********************************************************/
void read_eeprom(void);

/********************************/
/*name:	operate_managmentcard		*/
/*discription:����������				*/
/*var in:	rd_da ��������				*/
/*var in:	now_date ����					*/
/*var in:	now_time ʱ��					*/
/*var in:	wk	��								*/
/*result out:										*/
/********************************/
void operate_managmentcard(unsigned char xdata *rd_da, unsigned char xdata *now_date, unsigned char xdata *now_time, unsigned char wk);

/********************************/
/*name:		seek_eeprom_isn				*/
/*discription: �ȶ�isn�Ƿ���Ч	*/
/*var in:		isn ��Ҫ����isn		*/
/*result out:	���״̬					*/
/********************************/
bit seek_eeprom_isn(unsigned int sector, unsigned char xdata *isn);

/********************************/
/*name:	operate_usercard				*/
/*discription:�û�������				*/
/*var in:		rd_da ��������			*/
/*var in:		now_dat ʵʱ����		*/
/*var in:		now_tim ʵʱʱ��		*/
/*result out:										*/
/********************************/
void operate_usercard(unsigned char xdata *rd_da,unsigned char xdata *now_dat, unsigned char xdata *now_tim);

/****************************************/
/*name:check_time												*/
/*discription:���ʱ���Ƿ���Ч					*/
/*var in:������ʱ�䣨��Χʱ�� ����ʱ�䣩*/
/*result out:״̬												*/
/****************************************/
bit check_time(unsigned char xdata *bg_dt, unsigned char xdata *ed_dt, unsigned char xdata *bg_t0,
								unsigned char xdata *ed_t0,unsigned char xdata *bg_t1, unsigned char xdata *ed_t1,  
								unsigned char xdata *bg_t2,unsigned char xdata *ed_t2,unsigned char *now_dt, unsigned char xdata *now_t);

#endif