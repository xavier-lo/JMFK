////**********************************////
//=================//
//������������
//����JMFK-3 ˢ����
//MCU��stc89c55rd+
//����11.0592MHz ������1.085us
//�ļ���STC_eeprom.C
//================//
////**********************************////

#include <stc_eeprom.h>
#include <intrins.h>

#define RdCommand       0x01
#define PrgCommand      0x02
#define EraseCommand    0x03
#define WaitTime      0x00 //40M������0 20M������1 10M������2 5M������3

#define EEPROM_SATRTADDR	0x8000 //EEPROM��ʼ��ַ



/********************************/
/*name:ISP_IAP_enable						*/
/*discription:�� ISP/IAP����	*/
/*var in:												*/
/*result out:										*/
/********************************/
void ISP_IAP_enable(void)
{
	EA	=	0;//�ر��ж�
//	ISP_CONTR	|=	0x18;/*0001,1000*/
	ISP_CONTR	|=	WaitTime;/*д��Ӳ����ʱ*/
	ISP_CONTR	|=	0x80;/*ISPEN=1*/
}

/********************************/
/*name:ISP_IAP_disable					*/
/*discription:�ر� ISP/IAP����	*/
/*var in:												*/
/*result out:										*/
/********************************/
void ISP_IAP_disable(void)
{
	ISP_CONTR&=0x7f;/* ISPEN = 0 */
	ISP_TRIG=0x00;
	EA=1;//���ж�
}

/********************************/
/*name:ISPgoon									*/
/*discription:IAP/ISP����				*/
/*var in:												*/
/*result out:										*/
/********************************/
void ISPgoon(void)
{
	ISP_IAP_enable();/*��ISP/IAP����*/
	ISP_TRIG=0x46;/* ����ISP_IAP�ֽ�1 */
	ISP_TRIG=0xb9;/* ����ISP_IAP�ֽ�2 */
	_nop_();
}

/********************************/
/*name:	byte_read								*/
/*discription:�ֽڶ�						*/
/*var in:byte_addr ��ַ					*/
/*result out:ISP_DATA��������		*/
/********************************/
unsigned char byte_read(unsigned int byte_addr)
{
	ISP_ADDRH=(unsigned char)(byte_addr>>8); //��ַ��ֵ
	ISP_ADDRL=(unsigned char)(byte_addr&0x00ff);
	ISP_CMD&=0xf8;  //�������λ
	ISP_CMD|=RdCommand;//д�������
	ISPgoon();//����ִ��
	ISP_IAP_disable();//�ر�
	return ISP_DATA;//��������
}

/********************************/
/*name:sectorerase							*/
/*discription:��������					*/
/*var in:sector_addr������ַ		*/
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
	ISP_CMD&=0xf8;//�������λ
	ISP_CMD|=EraseCommand;//д���������
	ISPgoon();//����ִ��
	ISP_IAP_disable();//�ر�	


}


/********************************/
/*name:byte_write								*/
/*discription:�ֽ�д						*/
/*var in:byte_addr ��ַ					*/
/*var in:original_dataд������	*/
/*result out:										*/
/********************************/
//void byte_write(unsigned int byte_addr, unsigned char original_data)
//{
//	ISP_ADDRH=(unsigned char)(byte_addr>>8); //��ַ��ֵ
//	ISP_ADDRL=(unsigned char)(byte_addr & 0x00ff);
//	ISP_CMD&=0xf8;//�������λ
//	ISP_CMD|=PrgCommand;//д��д����
//	ISP_DATA=original_data;//д������׼��
//	ISPgoon();//����ִ��
//	ISP_IAP_disable();//�ر�
//}

/********************************/
/*name:byte_write_verify				*/
/*discription:�ֽ�д��У��			*/
/*var in:byte_addr ��ַ					*/
/*var in:original_dataд������	*/
/*result out:д��״̬						*/
/********************************/
bit byte_write_verify(unsigned int byte_addr, unsigned char original_data)
{
	ISP_ADDRH=(unsigned char)(byte_addr>>8); //��ַ��ֵ
	ISP_ADDRL=(unsigned char)(byte_addr&0xff);
	ISP_CMD&=0xf8;//�������λ
	ISP_CMD|=PrgCommand;//д��д����
	ISP_DATA=original_data;
	ISPgoon();//����ִ��
	/*��ʼ�� �����ظ�����ַ ��ַ�����Զ��ı�*/
	ISP_DATA=0x00;//������ݻ������Ĵ���
	ISP_CMD&=0xf8;//�������λ
	ISP_CMD|=RdCommand;//д�������
	ISP_TRIG=0x46;//����
	ISP_TRIG=0xb9;
	_nop_();/* ��ʱ*/
	ISP_IAP_disable();//�ر�
	if(ISP_DATA==original_data)//�Ƚ������Ƿ����
		 return 1;//���سɹ�
	else
		 return 0;
}



/**************************************/
/*name:	arrywrite_overwrite						*/
/*discription:д��ָ���������ݵ�eeprom*/
/*var in:start_addr ��ʼ��ַ					*/
/*var in:lenth ����										*/
/*var in:da ��Ҫд�������						*/
/*result out:  д��״̬								*/
/**************************************/
bit arrywrite_overwrite(unsigned int start_addr, unsigned int lenth, unsigned char *da)
{
	unsigned int in_addr,i = 0;
	bit st;
	/*��ֹ����д��*/
	if(lenth > 512)
		 return 0;
	in_addr = start_addr & 0x01ff;/* ȡƫ���� */
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
/*discription:д��һ������			*/
/*var in:sector_num ������			*/
/*var in:array д������					*/
/*result out:д��״̬						*/
/********************************/
/*�˳������ok ������ ������Ϊ�˽�ʡram*/
//bit arraywrite(unsigned char sector_num, unsigned char *array)
//{
//	unsigned int    i;
//	unsigned int    in_addr;
//	in_addr = EEPROM_SATRTADDR	+	sector_num	*	512;
//	
//	/*���д�벢У��*/
//	ISP_IAP_enable();//��IAP����
//	/*һ������512�ֽ�*/
//	for(i=0;i < 512;i++)
//		{
//		 /* дһ���ֽ� */
//		 ISP_ADDRH=(unsigned char)(in_addr >> 8);
//		 ISP_ADDRL=(unsigned char)(in_addr & 0x00ff);
//		 ISP_DATA=array[i];   /* ȡ����     */
//		 ISP_CMD&=0xf8;/* �����3λ       */
//		 ISP_CMD|=PrgCommand;/*д��д����*/
//		 ISP_TRIG=0x46;//����
//		 ISP_TRIG=0xb9;
//		 _nop_();
//		 /* ��һ���ֽ� */
//		 ISP_DATA=0x00;
//		 ISP_CMD&=0xf8;/* �����3λ */
//		 ISP_CMD|=RdCommand;/* д�������*/
//		 ISP_TRIG=0x46;//����
//		 ISP_TRIG=0xb9;
//		 _nop_();
//		 /*  �Ƚ���� */
//		 if(ISP_DATA!=array[i])
//			{
//			 ISP_IAP_disable();
//			 return 0;
//			}
//			in_addr++;/* ָ����һ���ֽ�*/
//		 }
//		 ISP_IAP_disable();
//		 return 1;
//}

/********************************/
/*name:arrayread								*/
/*discription:��һ����������		*/
/*var in:sector_num ������			*/
/*result out:										*/
/********************************/
/*�˳������ok ������ ������Ϊ�˽�ʡram*/
//void arrayread(unsigned char sector_num, unsigned char *out)
//{
//	unsigned int iSectorAddr;
//	unsigned int i;
//	iSectorAddr = EEPROM_SATRTADDR	+	sector_num	*	512;     // & 0xfe00; /*ȡ������ַ*/
//	ISP_IAP_enable();
//	/*һ������512�ֽ�*/
//	for(i=0;i<512;i++)
//	{
//		ISP_ADDRH=(unsigned char)(iSectorAddr>>8);
//		ISP_ADDRL=(unsigned char)(iSectorAddr & 0x00ff);
//		ISP_CMD&=0xf8;/* �����3λ */
//		ISP_CMD|=RdCommand;/* д�������*/
//		ISP_DATA=0;
//		ISP_TRIG=0x46;//����
//		ISP_TRIG=0xb9;
//		_nop_();
//		out[i]=ISP_DATA;
//		iSectorAddr++;
//	}
//	ISP_IAP_disable();//�ر�
//}


/********************************/
/*name:arrayread_overwirte			*/
/*discription:��ָ����������		*/
/*var in:start_addr ��ʼ��ַ		*/
/*var in:lenth ����							*/
/*result out:										*/
/********************************/
void arrayread_overwirte(unsigned int start_addr, unsigned int lenth, unsigned char *out)
{
	unsigned int iSectorAddr;
	unsigned int i;
	iSectorAddr = start_addr;
	ISP_IAP_enable();
	/*һ������512�ֽ�*/
	for(i=0;i<lenth;i++)
	{
		out[i]=byte_read(iSectorAddr);
		iSectorAddr++;
	}
	ISP_IAP_disable();//�ر�
}
