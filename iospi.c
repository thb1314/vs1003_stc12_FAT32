#include "iospi.h"


unsigned char bdata dat; //dat是可位寻址的变量

sbit dat7=dat^7;
sbit dat6=dat^6;
sbit dat5=dat^5;
sbit dat4=dat^4;
sbit dat3=dat^3;
sbit dat2=dat^2;
sbit dat1=dat^1;
sbit dat0=dat^0; //取出dat的各个位
unsigned char bdata rbyte = 0;
sbit rdat7=rbyte^7;
sbit rdat6=rbyte^6;
sbit rdat5=rbyte^5;
sbit rdat4=rbyte^4;
sbit rdat3=rbyte^3;
sbit rdat2=rbyte^2;
sbit rdat1=rbyte^1;
sbit rdat0=rbyte^0; //取出dat的各个位
void IOSPI_IO_Init(void)
{
 IOSPI_SI=1;
 IOSPI_SCK=1;
 IOSPI_SO=1; 
}

/******************************************************************
 - 功能描述：IO模拟SPI，发送一个字节，同时读回一个字节
 - 隶属模块：SPI模块
 - 函数属性：外部，供用户调用
 - 参数说明：x:要发送的字节
 - 返回说明：读到的字节
 - 注：很多情况下，SPI是需要有较高的速度的，此函数中不使用任何循环
       结构，如for(;;) while等等，并且使用了位寻址就是为了提高速度
 ******************************************************************/

unsigned char IOSPI_ReadWriteByte(unsigned char data x) 
{
 /*----这种使用循环与位运算的实现方式，速度要比直接用位寻址与顺序执行方式实现慢得多
   ----因为它把大部分的时间花在了循环因子的递增、比较与位运行上了
  unsigned char i=0;
  for(i=0;i<8;i++)
  {
   IOSPI_SI=x&(0x80>>i);
   IOSPI_SCK=0; 
   IOSPI_SCK=1;
  }
 ----------------------------------*/
	
 rbyte = 0;

 dat=x;	//将x的值赋给可位寻址的变量dat，以便取出各个位
	
 IOSPI_SO=1;
	
 IOSPI_SCK=1;

 
 IOSPI_SI=dat7; //取出第7个位，写到数据线上
 IOSPI_SCK=0; 
 
 if(IOSPI_SO) rdat7 = 1;
 IOSPI_SCK=1;	  //时钟线产生上升沿，数据被写入
 

 IOSPI_SI=dat6; //取出第6个位，写到数据线上
 IOSPI_SCK=0; 
 
 if(IOSPI_SO) rdat6 = 1;
 IOSPI_SCK=1;	  //时钟线产生上升沿，数据被写入
 
 
 IOSPI_SI=dat5; //取出第5个位，写到数据线上
 IOSPI_SCK=0; 
 
 if(IOSPI_SO) rdat5 = 1;
 IOSPI_SCK=1;	  //时钟线产生上升沿，数据被写入
 
 
 IOSPI_SI=dat4; //取出第4个位，写到数据线上
 IOSPI_SCK=0; 
 
 if(IOSPI_SO) rdat4 = 1;
 IOSPI_SCK=1;	  //时钟线产生上升沿，数据被写入
 
 
 IOSPI_SI=dat3; //取出第3个位，写到数据线上
 IOSPI_SCK=0; 
 
 if(IOSPI_SO) rdat3 = 1;
 IOSPI_SCK=1;	  //时钟线产生上升沿，数据被写入
 
 
 IOSPI_SI=dat2; //取出第2个位，写到数据线上
 IOSPI_SCK=0; 
 
 if(IOSPI_SO) rdat2 = 1;
 IOSPI_SCK=1;	  //时钟线产生上升沿，数据被写入
 
 
 IOSPI_SI=dat1; //取出第1个位，写到数据线上
 IOSPI_SCK=0; 
 
 if(IOSPI_SO) rdat1 = 1;
 IOSPI_SCK=1;	  //时钟线产生上升沿，数据被写入
 
 
 IOSPI_SI=dat0; //取出第0个位，写到数据线上
 IOSPI_SCK=0; 
 
 if(IOSPI_SO) rdat0 = 1;
 IOSPI_SCK=1;	  //时钟线产生上升沿，数据被写入
 
 
 return rbyte;
}

/******************************************************************
 - 功能描述：IO模拟SPI，发送一个字节，同时读回一个字节(极性相反)
 - 隶属模块：SPI模块
 - 函数属性：外部，供用户调用
 - 参数说明：x:要发送的字节
 - 返回说明：读到的字节
 - 注：很多情况下，SPI是需要有较高的速度的，此函数中不使用任何循环
       结构，如for(;;) while等等，并且使用了位寻址就是为了提高速度
 ******************************************************************/
/*
unsigned char IOSPI_ReadWriteByte_R(unsigned char x) 
{
 //----这种使用循环与位运算的实现方式，速度要比直接用位寻址与顺序执行方式实现慢得多
   //----因为它把大部分的时间花在了循环因子的递增、比较与位运行上了
  unsigned char i=0;
  for(i=0;i<8;i++)
  {
   IOSPI_SI=x&(0x80>>i);
   IOSPI_SCK=0; 
   IOSPI_SCK=1;
  }

	
 unsigned char idata rbyte=0;

 dat=x;	//将x的值赋给可位寻址的变量dat，以便取出各个位
	
 IOSPI_SO=1;
	
 IOSPI_SCK=0;
 //delay(IOSPI_DELAY_TIME);
 
 IOSPI_SI=dat7; //取出第7个位，写到数据线上
 IOSPI_SCK=1; 
 //delay(IOSPI_DELAY_TIME);
 if(IOSPI_SO) rbyte|=0x80;
 IOSPI_SCK=0;	  //时钟线产生上升沿，数据被写入
 //delay(IOSPI_DELAY_TIME);

 IOSPI_SI=dat6; //取出第6个位，写到数据线上
 IOSPI_SCK=1; 
 //delay(IOSPI_DELAY_TIME);
 if(IOSPI_SO) rbyte|=0x40;
 IOSPI_SCK=0;	  //时钟线产生上升沿，数据被写入
 //delay(IOSPI_DELAY_TIME);
 
 IOSPI_SI=dat5; //取出第5个位，写到数据线上
 IOSPI_SCK=1; 
// delay(IOSPI_DELAY_TIME);
 if(IOSPI_SO) rbyte|=0x20;
 IOSPI_SCK=0;	  //时钟线产生上升沿，数据被写入
 //delay(IOSPI_DELAY_TIME);
 
 IOSPI_SI=dat4; //取出第4个位，写到数据线上
 IOSPI_SCK=1; 
 //delay(IOSPI_DELAY_TIME);
 if(IOSPI_SO) rbyte|=0x10;
 IOSPI_SCK=0;	  //时钟线产生上升沿，数据被写入
 //delay(IOSPI_DELAY_TIME);
 
 IOSPI_SI=dat3; //取出第3个位，写到数据线上
 IOSPI_SCK=1; 
 //delay(IOSPI_DELAY_TIME);
 if(IOSPI_SO) rbyte|=0x08;
 IOSPI_SCK=0;	  //时钟线产生上升沿，数据被写入
 //delay(IOSPI_DELAY_TIME);
 
 IOSPI_SI=dat2; //取出第2个位，写到数据线上
 IOSPI_SCK=1; 
 //delay(IOSPI_DELAY_TIME);
 if(IOSPI_SO) rbyte|=0x04;
 IOSPI_SCK=0;	  //时钟线产生上升沿，数据被写入
 //delay(IOSPI_DELAY_TIME);
 
 IOSPI_SI=dat1; //取出第1个位，写到数据线上
 IOSPI_SCK=1; 
 //delay(IOSPI_DELAY_TIME);
 if(IOSPI_SO) rbyte|=0x02;
 IOSPI_SCK=0;	  //时钟线产生上升沿，数据被写入
 //delay(IOSPI_DELAY_TIME);
 
 IOSPI_SI=dat0; //取出第0个位，写到数据线上
 IOSPI_SCK=1; 
 //delay(IOSPI_DELAY_TIME);
 if(IOSPI_SO) rbyte|=0x01;
 IOSPI_SCK=0;	  //时钟线产生上升沿，数据被写入
 //delay(IOSPI_DELAY_TIME);
 
 return rbyte;
}
*/