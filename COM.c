#include <STC12C5A60S2.H>
#include "COM.h"


/*
 * 设置接收字符
 */
 
unsigned char UartRcvChar = 0x00;

// 串口初始化
void UartInit()
{
	TMOD=0x20;					//定时器1  T1 工作于8位自动重载模式，用于产生波特率
	TH1=0xF4;					//22.1184MHz的晶振波特率为9600
	TL1=0xF4;					//因为工作于 8位自动重载模式，所以TH1=TL1
	SCON=0x50;					//设定串行口工作方式1 允许接收 
	PCON=0x80;					//波特率倍增
	TR1=1;						//启动定时器1  T1	产生波特率
	EA=1;						//开启中断总开关
	ES=1;						//开启串口中断
}

//串口接收中断函数
void UartRcv(void) interrupt 4
{
	//char Rcv=0;
	if(RI)						//查询接收标志位（有数据发送过来时置为1）
	{
		RI=0;					//标志位置为0
		UartRcvChar=SBUF;				//存储缓冲器的数据
		//_com_send_char(UartRcvChar);			//返回给计算机
	}
}

//串口发送单个字符
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
//串口发送字符串
void UartWrite(char* s,unsigned int len)
{
	while(len--)
	{
		_com_send_char(*s++);

	}
}