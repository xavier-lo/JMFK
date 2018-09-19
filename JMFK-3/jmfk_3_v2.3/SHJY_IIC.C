////**********************************////
//=================//
//本程序适用于模拟IIC通信
//文件:SHJY_IIC.C
//================//
////**********************************////

#include <SHJY_IIC.H>

sbit SDA  = P0^5; //iic数据
sbit SCL  = P0^4; //iic时钟
sbit WP		=	P3^4;
sbit INIT1	=	P3^3;
sbit INIT2	=	P3^2;
sbit SCLE		=	P0^7;
sbit SDAE		=	P0^6;



/********************************/
/*Description:延时							*/
/*In:														*/
/*Out:													*/
/********************************/
void delay(void)
{
	int i=0;
}
/*****/

/********************************/
/*Description:模拟IIC启动				*/
/*In:														*/
/*Out:IIC应答位ACK							*/
/********************************/
bit IIC_START(void)
{
	bit ACK_BIT;  //应答位
 	SDA=1;
	delay();
	SCL=1;
	delay();
	if(SDA&&SCL)
	{
		SDA=0;
		delay();
		SCL=0;
		delay();
		ACK_BIT=0;		
	}else
	{
		ACK_BIT=1;
	}
	return ACK_BIT;
}
/***/

/********************************/
/*Description:模拟IIC停止				*/
/*In:														*/
/*Out:													*/
/********************************/
void IIC_STOP(void)
{
	SCL=0;
	delay();
	SDA=0;
	delay();
	SCL=1;
	delay();
	SDA=1;
	delay();
}
/*****/

/********************************/
/*Description:模拟IIC发送1位		*/
/*In:send_bit 需要发送的1位数据	*/
/*Out:													*/
/********************************/
void IIC_send_bit(bit send_bit)
{
	SDA=send_bit;
	delay();
	SCL=1;
	delay();
	SCL=0;
	delay();
}
/*****/


/********************************/
/*Description:模拟IIC读取应答位 */
/*In:														*/
/*Out:IIC应答位ACK							*/
/********************************/
bit IIC_read_bit(void)
{
	bit ACK_BIT;  //应答位
	SDA=1;
	delay();
	SCL=1;
	delay();
	ACK_BIT=SDA;
	delay();
	SCL=0;
	delay();
	return ACK_BIT;
}
/*****/

/**************************************/
/*Description:模拟IIC发送1字节数据		*/
/*In:send_buffer 需要发送的1字节数据	*/
/*Out:IIC应答位ACK										*/
/**************************************/
bit IIC_send_byte(unsigned char send_buffer)
{
	bit ACK_BIT;  //应答位
	IIC_send_bit(send_buffer&0x80);//高位先发送
	IIC_send_bit(send_buffer&0x40);
	IIC_send_bit(send_buffer&0x20);
	IIC_send_bit(send_buffer&0x10);
	IIC_send_bit(send_buffer&0x08);
	IIC_send_bit(send_buffer&0x04);
	IIC_send_bit(send_buffer&0x02);
	IIC_send_bit(send_buffer&0x01);

	ACK_BIT=IIC_read_bit();
	return ACK_BIT;

}
/******/

/**********************************************/
/*Description:模拟IIC发送1字节数据低位先发		*/
/*In:send_buffer 需要发送的1字节数据					*/
/*Out:IIC应答位ACK														*/
/**********************************************/
bit IIC_send_byte_ltoh(unsigned char send_buffer)
{
	bit ACK_BIT;  //应答位
	IIC_send_bit(send_buffer&0x01);//低位先发送
	IIC_send_bit(send_buffer&0x02);
	IIC_send_bit(send_buffer&0x04);
	IIC_send_bit(send_buffer&0x08);
	IIC_send_bit(send_buffer&0x10);
	IIC_send_bit(send_buffer&0x20);
	IIC_send_bit(send_buffer&0x40);
	IIC_send_bit(send_buffer&0x80);

	ACK_BIT=IIC_read_bit();
	return ACK_BIT;

}
/******/

/********************************/
/*name:	IIC_read_Byte						*/
/*discription:模拟IIC读取1字节	*/
/*var in:												*/
/*result out:r_b 								*/
/********************************/
unsigned char IIC_read_Byte(void)
{
	unsigned char r_b;
	bit ack;
	ack=IIC_read_bit();
	r_b=((unsigned char)ack)&0x01;
	ack=IIC_read_bit();	
	r_b|=((unsigned char)ack<<1)&0x02;	
	ack=IIC_read_bit();	
	r_b|=((unsigned char)ack<<2)&0x04;
	ack=IIC_read_bit();	
	r_b|=((unsigned char)ack<<3)&0x08;
	ack=IIC_read_bit();	
	r_b|=((unsigned char)ack<<4)&0x10;
	ack=IIC_read_bit();	
	r_b|=((unsigned char)ack<<5)&0x20;
	ack=IIC_read_bit();	
	r_b|=((unsigned char)ack<<6)&0x40;
	ack=IIC_read_bit();	
	r_b|=((unsigned char)ack<<7)&0x80;	
	return r_b;
}





