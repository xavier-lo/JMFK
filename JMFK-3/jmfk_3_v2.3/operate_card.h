////**********************************////
//=================//
//本程序适用于
//用于JMFK-3 刷卡器
//MCU：stc89c55rd+
//晶振：29.4912MHz 震荡周期0.4069us 双倍速下载
//文件：operate_card.H
//================//
////**********************************////
#ifndef __OPERATE_CARD_H__
#define __OPERATE_CARD_H__

#include<STC_MCU.H>
/**********************************/
/*name:make_verr									*/
/*discription:产生校验						*/
/*var in:da0 需要生成的数据列			*/
/*result out:	da0 生成后的数据列	*/
/**********************************/
void make_verr(unsigned char *da0);
/********************************/
/*name:	verrify									*/
/*discription:校验接收数据			*/
/*var in:da0 需要校验的数据			*/
/*result out:bool 状态					*/
/********************************/
bit verrify(unsigned char *da0);

/********************************/
/*name:seek_card								*/
/*discription:寻卡							*/
/*var in:												*/
/*result out:recive_da 返回数据	*/
/********************************/
char seek_card(unsigned char *recive_da);

/**************************************/
/*name:	write_card										*/
/*discription:写数据到块使用卡头密码	*/
/*var in:block 块号										*/
/*var in:da 需要写的数据16字节				*/
/*result out:state 写卡状态						*/
/**************************************/
bit write_card(unsigned char block,unsigned char *da);

/********************************************/
/*name:	read_card														*/
/*discription:读某一块数据									*/
/*var in:	sector 扇区号											*/
/*result out:	out 读出数据48字节传入53字节	*/
/*result out:	state 状态										*/
/********************************************/
bit read_card(unsigned char sector, unsigned char *out);

/********************************/
/*name:read_card_1block					*/
/*discription:读出1块数据				*/
/*var in:读出16字节 传入21字节	*/
/*result out:										*/
/********************************/
bit read_card_1block(unsigned char block,unsigned char *out);

/********************************/
/*name:	read_user_card					*/
/*discription:读用户卡					*/
/*var in:	in 扇区号							*/
/*var in:	out 传入69个字节			*/
/*result out:	读出状态					*/
/********************************/
bit read_user_card(unsigned char sector, unsigned char *out);


#endif