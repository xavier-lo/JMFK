////**********************************////
//=================//
//������������
//����JMFK-3 ˢ����
//MCU��stc89c55rd+
//����11.0592MHz ������1.085us
//�ļ���STC_eeprom.H
//================//
////**********************************////


#ifndef __STC_EEPROM_H__
#define __STC_EEPROM_H__

#include <stc_mcu.h>

/********************************/
/*name:ISP_IAP_enable						*/
/*discription:�� ISP/IAP����	*/
/*var in:												*/
/*result out:										*/
/********************************/
void ISP_IAP_enable(void);

/********************************/
/*name:ISP_IAP_disable					*/
/*discription:�ر� ISP/IAP����	*/
/*var in:												*/
/*result out:										*/
/********************************/
void ISP_IAP_disable(void);

/********************************/
/*name:ISPgoon									*/
/*discription:IAP/ISP����				*/
/*var in:												*/
/*result out:										*/
/********************************/
void ISPgoon(void);

/********************************/
/*name:	byte_read								*/
/*discription:�ֽڶ�						*/
/*var in:byte_addr ��ַ					*/
/*result out:ISP_DATA��������		*/
/********************************/
unsigned char byte_read(unsigned int byte_addr);


/********************************/
/*name:byte_write								*/
/*discription:�ֽ�д						*/
/*var in:byte_addr ��ַ					*/
/*var in:original_dataд������	*/
/*result out:										*/
/********************************/
//void byte_write(unsigned int byte_addr, unsigned char original_data);

/********************************/
/*name:sectorerase							*/
/*discription:��������					*/
/*var in:sector_addr������			*/
/*result out:										*/
/********************************/
void sectorerase(unsigned int sector_addr);

/********************************/
/*name:byte_write_verify				*/
/*discription:�ֽ�д��У��			*/
/*var in:byte_addr ��ַ					*/
/*var in:original_dataд������	*/
/*result out:д��״̬						*/
/********************************/
bit byte_write_verify(unsigned int byte_addr, unsigned char original_data);

/********************************/
/*name:	arraywrite							*/
/*discription:д��һ������			*/
/*var in:sector_num ������			*/
/*var in:array д������					*/
/*result out:д��״̬						*/
/********************************/
//bit arraywrite(unsigned char sector_num, unsigned char *array);


/********************************/
/*name:arrayread								*/
/*discription:��һ����������		*/
/*var in:sector_num ������			*/
/*result out:										*/
/********************************/
//void arrayread(unsigned char sector_num, unsigned char *out);

/**************************************/
/*name:	arrywrite_overwrite						*/
/*discription:д��ָ���������ݵ�eeprom*/
/*var in:start_addr ��ʼ��ַ					*/
/*var in:lenth ����										*/
/*var in:da ��Ҫд�������						*/
/*result out:  д��״̬								*/
/**************************************/
bit arrywrite_overwrite(unsigned int start_addr, unsigned int lenth, unsigned char *da);

/********************************/
/*name:arrayread_overwirte			*/
/*discription:��ָ����������		*/
/*var in:start_addr ��ʼ��ַ		*/
/*var in:lenth ����							*/
/*result out:										*/
/********************************/
void arrayread_overwirte(unsigned int start_addr, unsigned int lenth, unsigned char *out);

#endif