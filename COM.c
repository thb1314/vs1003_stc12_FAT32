#include <STC12C5A60S2.H>
#include "COM.h"


/*
 * ���ý����ַ�
 */
 
unsigned char UartRcvChar = 0x00;

// ���ڳ�ʼ��
void UartInit()
{
	TMOD=0x20;					//��ʱ��1  T1 ������8λ�Զ�����ģʽ�����ڲ���������
	TH1=0xF4;					//22.1184MHz�ľ�������Ϊ9600
	TL1=0xF4;					//��Ϊ������ 8λ�Զ�����ģʽ������TH1=TL1
	SCON=0x50;					//�趨���пڹ�����ʽ1 ������� 
	PCON=0x80;					//�����ʱ���
	TR1=1;						//������ʱ��1  T1	����������
	EA=1;						//�����ж��ܿ���
	ES=1;						//���������ж�
}

//���ڽ����жϺ���
void UartRcv(void) interrupt 4
{
	//char Rcv=0;
	if(RI)						//��ѯ���ձ�־λ�������ݷ��͹���ʱ��Ϊ1��
	{
		RI=0;					//��־λ��Ϊ0
		UartRcvChar=SBUF;				//�洢������������
		//_com_send_char(UartRcvChar);			//���ظ������
	}
}

//���ڷ��͵����ַ�
void _com_send_char(char c)
{
	SBUF=c;
	while(!TI);
	TI=0;	
}
void _com_send_string(unsigned char* s)
{
	while(*s != '\0')
	{
		_com_send_char(*s);
		s++;
	}
}
//���ڷ����ַ���
void UartWrite(char* s,unsigned int len)
{
	while(len--)
	{
		_com_send_char(*s++);

	}
}