////**********************************////
//=================//
//本程序适用于
//用于JMFK-3 刷卡器
//MCU：stc89c55rd+
//晶振：11.0592MHz 震荡周期1.085us
//文件：STC_eeprom.H
//================//
////**********************************////


#ifndef __STC_EEPROM_H__
#define __STC_EEPROM_H__

#include <stc_mcu.h>

/********************************/
/*name:ISP_IAP_enable						*/
/*discription:打开 ISP/IAP功能	*/
/*var in:												*/
/*result out:										*/
/********************************/
void ISP_IAP_enable(void);

/********************************/
/*name:ISP_IAP_disable					*/
/*discription:关闭 ISP/IAP功能	*/
/*var in:												*/
/*result out:										*/
/********************************/
void ISP_IAP_disable(void);

/********************************/
/*name:ISPgoon									*/
/*discription:IAP/ISP触发				*/
/*var in:												*/
/*result out:										*/
/********************************/
void ISPgoon(void);

/********************************/
/*name:	byte_read								*/
/*discription:字节读						*/
/*var in:byte_addr 地址					*/
/*result out:ISP_DATA读出数据		*/
/********************************/
unsigned char byte_read(unsigned int byte_addr);


/********************************/
/*name:byte_write								*/
/*discription:字节写						*/
/*var in:byte_addr 地址					*/
/*var in:original_data写入数据	*/
/*result out:										*/
/********************************/
//void byte_write(unsigned int byte_addr, unsigned char original_data);

/********************************/
/*name:sectorerase							*/
/*discription:扇区擦除					*/
/*var in:sector_addr扇区号			*/
/*result out:										*/
/********************************/
void sectorerase(unsigned int sector_addr);

/********************************/
/*name:byte_write_verify				*/
/*discription:字节写并校验			*/
/*var in:byte_addr 地址					*/
/*var in:original_data写入数据	*/
/*result out:写入状态						*/
/********************************/
bit byte_write_verify(unsigned int byte_addr, unsigned char original_data);

/********************************/
/*name:	arraywrite							*/
/*discription:写入一个扇区			*/
/*var in:sector_num 扇区号			*/
/*var in:array 写入数据					*/
/*result out:写入状态						*/
/********************************/
//bit arraywrite(unsigned char sector_num, unsigned char *array);


/********************************/
/*name:arrayread								*/
/*discription:读一个扇区数据		*/
/*var in:sector_num 扇区号			*/
/*result out:										*/
/********************************/
//void arrayread(unsigned char sector_num, unsigned char *out);

/**************************************/
/*name:	arrywrite_overwrite						*/
/*discription:写入指定长度数据到eeprom*/
/*var in:start_addr 起始地址					*/
/*var in:lenth 长度										*/
/*var in:da 需要写入的数据						*/
/*result out:  写入状态								*/
/**************************************/
bit arrywrite_overwrite(unsigned int start_addr, unsigned int lenth, unsigned char *da);

/********************************/
/*name:arrayread_overwirte			*/
/*discription:读指定长度数据		*/
/*var in:start_addr 起始地址		*/
/*var in:lenth 长度							*/
/*result out:										*/
/********************************/
void arrayread_overwirte(unsigned int start_addr, unsigned int lenth, unsigned char *out);

#endif