////**********************************////
//=================//
//������������
//����JMFK-3 ˢ����
//MCU��stc89c55rd+
//����11.0592MHz ������1.085us
//�ļ���STC_USER.C
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

/*****����ֲ��ֽڱ���*****///
volatile unsigned char xdata eeprom_data[512];//eeprom��������	
unsigned char IC_sector;//�û�������
unsigned char IC_group;//�û���Ⱥ��
unsigned char IC_elv;//�û����ݺ�
unsigned char xdata open_card_state = 0x00;

//bit SA1,SA2,SA3;//��ַ��1 2 3 

sbit SA1 = P0^3;
sbit SA2 = P0^2;
sbit SA3 = P0^1;

/***/

/*****����ֲ�λ����*****/
sbit T1_SEL	=	P1^0;
sbit T2_SEL	=	P1^1;
sbit T3_SEL	=	P1^2;
sbit T4_SEL	=	P1^3;

sbit BEEP	=	P3^7;//������
/***/


/*ȫ�ֱ�������*/
extern bit delay40ms_ready;
extern bit beep_start;

volatile bit delay_5s_start = 0;//�û�����Ч5s��ʼ
volatile bit delay_5s_stop = 0;//�û�����Ч5s����
volatile bit repeat_bit = 0;//�ظ�����������ʼ
volatile bit repeat_bit1 = 0;//�ظ�����������ʼ
volatile bit delay1s_ready = 0;//��ȡʱ������1s

volatile unsigned char pre_IC_ISN[4];//��ֹ��ʱ���ظ����� 
volatile unsigned char pre_IC_ISN1[4];//��ֹ��ʱ���ظ�����
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


	arrayread_overwirte(EEPROM_BASE_ADDR, 9, eeprom_data);//��ȡ��·������
	output_control(eeprom_data);
	while(1)
	{	
		if(delay40ms_ready)
		{
			delay40ms_ready = 0;
			switch(function_count)
			{
				case 0://Ѱ������ ������ͳһ������IC��0������ʼ
					operate_managmentcard(read_data);
					function_count = 1;
					break;
				case 1://Ѱ�û��� �û������������û�����д�뵽mcu eeprom
					operate_usercard(read_data,now_date,now_time);
					function_count = 0;
					break;
				default:
					function_count = 0;
					break;
			
			}
		}else if(delay1s_ready)
		{//��ʱ��
			delay1s_ready = 0;
			if(read_time(read_data) == 1)
			{
				now_date[0] = read_data[0];//��
				now_date[1] = read_data[1] & 0x1f;//��
				now_date[2] = read_data[2] & 0x3f;//��
				now_time[0] = read_data[4] & 0x3f;//ʱ
				now_time[1] = read_data[5] & 0x7f;//��	
			}
		}
		
		if(delay_5s_stop)
		{//�û�����Ч����
			delay_5s_stop = 0;
			arrayread_overwirte(EEPROM_BASE_ADDR,8,eeprom_data);//
			output_control(&eeprom_data[4*0]);		
		}
	
	}
	
}


/****************************************/
/*name:check_time												*/
/*discription:���ʱ���Ƿ���Ч					*/
/*var in:������ʱ�䣨��Χʱ�� ����ʱ�䣩*/
/*result out:״̬												*/
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
	
	//�ж�����
	if((now_date < bg_date) || (now_date > ed_date)) return 0;
	
	bg_time0 = ((unsigned int)(bg_t0[0] << 8) & 0xff00) + bg_t0[1];
	bg_time1 = ((unsigned int)(bg_t1[0] << 8) & 0xff00) + bg_t1[1];
	bg_time2 = ((unsigned int)(bg_t2[0] << 8) & 0xff00) + bg_t2[1];
	ed_time0 = ((unsigned int)(ed_t0[0] << 8) & 0xff00) + ed_t0[1];
	ed_time1 = ((unsigned int)(ed_t1[0] << 8) & 0xff00) + ed_t1[1];
	ed_time2 = ((unsigned int)(ed_t2[0] << 8) & 0xff00) + ed_t2[1];
	now_time = ((unsigned int)(now_t[0] << 8) & 0xff00) + now_t[1];	
	
	//�ж�ʱ��
	t_sta = 0x00;
	if((now_time < bg_time0) || (now_time > ed_time0)) t_sta |= 0x01;
	if((now_time < bg_time1) || (now_time > ed_time1)) t_sta |= 0x02;
	if((now_time < bg_time2) || (now_time > ed_time2)) t_sta |= 0x04;
	
	if(t_sta == 0x07 ) return 0;
	return 1;
}


/********************************/
/*name:		seek_eeprom_isn				*/
/*discription: �ȶ�isn�Ƿ���Ч	*/
/*var in:		isn ��Ҫ����isn		*/
/*result out:	���״̬					*/
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
/*discription:�û�������				*/
/*var in:		rd_da ��������			*/
/*var in:		now_dat ʵʱ����		*/
/*var in:		now_tim ʵʱʱ��		*/
/*result out:										*/
/********************************/
void operate_usercard(unsigned char xdata *rd_da,unsigned char xdata *now_dat, unsigned char xdata *now_tim)
{
	unsigned char xdata ii;
	/*��Ч��ʼ���� ��Ч�������� ��Ч��ʼʱ��0 1 2 ��Ч����ʱ��0 1 2 ��Ч����*/
	unsigned char xdata begin_date[3],end_date[3],begin_time0[2],end_time0[2],begin_time1[2],end_time1[2],begin_time2[2],end_time2[2];
	unsigned long xdata valid_count;
	unsigned char xdata temp_data[53];
	/*�����кſ�*/
	if(read_card(IC_sector+1,rd_da) == 1)
	{		
		if((pre_IC_ISN1[0] != rd_da[0])||(pre_IC_ISN1[1] != rd_da[1])||(pre_IC_ISN1[2] != rd_da[2])||(pre_IC_ISN1[3] != rd_da[3]))
		{//��ֹ�ظ���ȡ
			ii = 0;
			while(ii<4)
			{
				pre_IC_ISN1[ii] = rd_da[ii];
				ii++;
			}	
			repeat_bit1 = 1;//�ظ�ˢ��������ʼ
			
			if(!seek_eeprom_isn(&rd_da[4])) return;//���isn�Ƿ���Ч ��Чֱ���˳�����
			ii = 0;
			while(ii < 3)
			{//������Ч��������
				begin_date[ii] = ((rd_da[8 + 2 * ii] << 4) & 0xf0) | (rd_da[9 + 2 * ii] & 0x0f);
				end_date[ii] = ((rd_da[14 + 2 * ii] << 4) & 0xf0) | (rd_da[15 + 2 * ii] & 0x0f);
				ii++;
			}
			ii = 0;
			while(ii < 2)
			{//������Ч��ʼʱ��012 ����ʱ��012
				begin_time0[ii] = rd_da[20 + ii];
				begin_time1[ii] = rd_da[24 + ii];
				begin_time2[ii] = rd_da[28 + ii];
				end_time0[ii] = rd_da[22 + ii];	
				end_time1[ii] = rd_da[26 + ii];
				end_time2[ii] = rd_da[30 + ii];	
				ii++;
			}
			
			//������Ч����
			valid_count = (((unsigned long)rd_da[32] << 24) & 0xff000000) + (((unsigned long)rd_da[33] << 16) & 0xff0000) + (((unsigned long)rd_da[34] << 8) & 0xff00) + rd_da[35];
			
			
			/*���ʱ���Ƿ���Ч*/
			if(check_time(begin_date,end_date,begin_time0,end_time0,begin_time1,end_time1,begin_time2,end_time2,now_dat,now_tim) == 0) return;

			/*���������ݿ�*/
			if(read_card(IC_sector,rd_da) == 1)
			{
				if((rd_da[21] == (((IC_group << 5) & 0xe0)|(IC_elv&0x1f))) ||(rd_da[21] == ((IC_group << 5) & 0xe0)) || (rd_da[21] == 0))
				{//Ⱥ�� �ݺ���� ��Ⱥ������ݺ�Ϊ0  ��Ⱥ���ݺ�Ϊ0

					if((rd_da[20] & 0x10) == 0x10)//�ο�
					{
						if(valid_count <= 0) return;//����Ϊ0�˳�
						if(read_card(IC_sector+1,temp_data) == 1)
						{
							valid_count -= 1;
							temp_data[32] = (valid_count >> 24) & 0xff;
							temp_data[33] = (valid_count >> 16) & 0xff;
							temp_data[34] = (valid_count >> 8) & 0xff;
							temp_data[35] = valid_count & 0xff;
							
							if(write_card((IC_sector + 1) * 4 + 1, &temp_data[12]) != 1) return;//δд�ɹ��˳�
							
						}else return;//δ���ɹ��˳�
					}
					
					if((rd_da[20] & 0xe0) == 0)//�û��� ˢ������5s							
					{
						/*��������Ƶ� ��·��*/
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
						open_card_state = 0x00;//�û�������
					}else if((rd_da[20] & 0xe0) == 0x80)//�û����ؿ� ˢ�꼴�̷������ 
					{
						/*��������Ƶ� ��·��*/
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
	{//�޿�������к�
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
/*discription:����������				*/
/*var in:	rd_da ��������				*/
/*result out:										*/
/********************************/
void operate_managmentcard(unsigned char xdata *rd_da)
{
	unsigned int xdata ii,jj,kk;//���ڼ���
	unsigned int xdata IC_eeprom_addr,IC_Byte_num;
	if(read_card(0x00,rd_da) == 1)
	{
		if((pre_IC_ISN[0] != rd_da[0])||(pre_IC_ISN[1] != rd_da[1])||(pre_IC_ISN[2] != rd_da[2])||(pre_IC_ISN[3] != rd_da[3]))
		{//��ֹ�ظ���ȡ
			ii = 0;
			while(ii<4)
			{
				pre_IC_ISN[ii] = rd_da[ii];
				ii++;
			}	
			repeat_bit = 1;//�ظ�ˢ��������ʼ
			IC_eeprom_addr = ((((unsigned int)rd_da[22] <<8) & 0xff00) | rd_da[23]);//��ȡ��Ҫд��eeprom��ַ
			IC_Byte_num = ((((unsigned int)rd_da[24] <<8) & 0xff00) | rd_da[25]);//��ȡ��Ҫд�����кų���
			if((rd_da[20] & 0x60) == 0x40)//д��������·��
			{
				open_card_state = 0x00;//����������
				
				sectorerase(IC_eeprom_addr);//��������
				if(arrywrite_overwrite(IC_eeprom_addr,8,&rd_da[44]) == 1)//д���·�� 
				{
					if(!SA1)
					{	//д��Ⱥ�� �ݺ� �û���������								
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
					/*�������·��*/
					arrayread_overwirte(EEPROM_BASE_ADDR,8,eeprom_data);//
					output_control(&eeprom_data[4*0]);
				}							
			}
			if((rd_da[20] & 0x60) == 0x60)//д���к�
			{
				open_card_state = 0x00;//����������
				
				/*��ȡIC�����к�*/
				if(IC_Byte_num < 128)//���ȱ���С��128
				{
					jj = IC_Byte_num / 12;
					if((IC_Byte_num % 12) != 0) jj += 1;
					ii = 0;					
					while(ii+1 < jj+1)//��ic����2������ʼ����
					{
						if(read_card(ii + 1,rd_da) != 1)
						{//����ĳһ����δ�����˳�
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
					sectorerase(IC_eeprom_addr);//��������
					if(arrywrite_overwrite(IC_eeprom_addr,IC_Byte_num*4,eeprom_data) == 1)
					{
							beep_start = 1;
					}
				}
			}							
		}
	}else
	{//�޿�������к�
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
/*discription:������ȡ������ Ⱥ�� �ݺ� ��·�㲢������ӿ�	*/
/*var in:																									*/
/*result out:																							*/
/**********************************************************/
void read_eeprom(void)
{
	arrayread_overwirte(EEPROM_BASE_ADDR,10,eeprom_data);//
	IC_sector = eeprom_data[9]; //�û���������
	IC_group = ((eeprom_data[8] >> 5) & 0x07);//Ⱥ��
	IC_elv = (eeprom_data[8] & 0x1f);//�ݺ�
	output_control(&eeprom_data[4*0]);
}



/********************************/
/*name:	output_control					*/
/*discription:������Ƶ�74hc373	*/
/*var in:da �������� 4�ֽ�			*/
/*result out:										*/
/********************************/
void output_control(unsigned char xdata  *da )
{
		/*����ס*/
		T1_SEL	=	0;
		T2_SEL	=	0;
		T3_SEL	=	0;
		T4_SEL	=	0;
		/*�仯����1*/
		T1_SEL	=	1;
		_nop_();
		P2	=	da[0];
		T1_SEL	=	0;
		/*�仯����2*/
		T2_SEL	=	1;
		_nop_();	
		P2	=	da[1];
		T2_SEL	=	0;
		/*�仯����3*/
		T3_SEL	=	1;
		_nop_();
		P2	=	da[2];
		T3_SEL	=	0;
		/*�仯����4*/
		T4_SEL	=	1;
		_nop_();
		P2	=	da[3];
		T4_SEL	=	0;	
}
