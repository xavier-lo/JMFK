////**********************************////
//=================//
//������������
//����JMFK-3 ˢ����
//MCU��stc89c55rd+
//����11.0592MHz ������1.085us
//�ļ���operate_card.H
//================//
////**********************************////
#ifndef __OPERATE_CARD_H__
#define __OPERATE_CARD_H__

#include<STC_MCU.H>
/**********************************/
/*name:make_verr									*/
/*discription:����У��						*/
/*var in:da0 ��Ҫ���ɵ�������			*/
/*result out:	da0 ���ɺ��������	*/
/**********************************/
void make_verr(unsigned char *da0);
/********************************/
/*name:	verrify									*/
/*discription:У���������			*/
/*var in:da0 ��ҪУ�������			*/
/*result out:bool ״̬					*/
/********************************/
bit verrify(unsigned char *da0);

/********************************/
/*name:seek_card								*/
/*discription:Ѱ��							*/
/*var in:												*/
/*result out:recive_da ��������	*/
/********************************/
char seek_card(unsigned char *recive_da);

/**************************************/
/*name:	write_card										*/
/*discription:д���ݵ���ʹ�ÿ�ͷ����	*/
/*var in:block ���										*/
/*var in:da ��Ҫд������16�ֽ�				*/
/*result out:state д��״̬						*/
/**************************************/
bit write_card(unsigned char block,unsigned char *da);

/********************************************/
/*name:	read_card														*/
/*discription:��ĳһ������									*/
/*var in:	sector ������											*/
/*result out:	out ��������48�ֽڴ���53�ֽ�	*/
/*result out:	state ״̬										*/
/********************************************/
bit read_card(unsigned char sector, unsigned char *out);

/********************************/
/*name:read_card_1block					*/
/*discription:����1������				*/
/*var in:����16�ֽ� ����21�ֽ�	*/
/*result out:										*/
/********************************/
bit read_card_1block(unsigned char block,unsigned char *out);

#endif