////**********************************////
//=================//
//������������
//����JMFK-3 ˢ����
//MCU��stc89c55rd+
//����11.0592MHz ������1.085us
//�ļ���time_sd2200.C
//================//
#include<time_sd2200.h>
#include<SHJY_IIC.h>


/********************************/
/*name:	read_time								*/
/*discription:��ȡʱ��					*/
/*var in:												*/
/*result out:out ��Ŷ�ȡ����		*/
/********************************/
/*�� �� �� �� ʱ �� ��					*/
bit read_time(unsigned char *out)
{
	unsigned char i=0;
	bit state = 0;
	if(!IIC_START())
	{
		if(!IIC_send_byte(0x65))//���� 
		{
			while(i<7)
			{
				out[i]=IIC_read_Byte();	
				IIC_send_bit(0);//Ӧ��0
				i++;
			}
			IIC_send_bit(1);//Ӧ��1
			state = 1;
		}
	}
	
	IIC_STOP();
	return state;
}

/********************************/
/*name:		set_time							*/
/*discription:	���ô�绰			*/
/*var in:in ��������ʱ��				*/
/*result out:״̬								*/
/********************************/
bit set_time(unsigned char *in)
{
	bit state = 0;
	unsigned char xdata i = 0;
	if((in[4] >= 13) &&(in[4] <= 23)) in[4] |= 0x40;
	if(!IIC_START())
	{
		if(!IIC_send_byte(0x64))//д��������ʱ����
		{
			while(i < 6)
			{
				if(IIC_send_byte_ltoh(in[i])) return 0;
				i++;
			}
			if(!IIC_send_byte_ltoh(0x00)) state = 1;
		}
	}
	
	IIC_STOP();	
	
	
	return state;
}


