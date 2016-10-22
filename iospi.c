#include "iospi.h"


unsigned char bdata dat; //dat�ǿ�λѰַ�ı���

sbit dat7=dat^7;
sbit dat6=dat^6;
sbit dat5=dat^5;
sbit dat4=dat^4;
sbit dat3=dat^3;
sbit dat2=dat^2;
sbit dat1=dat^1;
sbit dat0=dat^0; //ȡ��dat�ĸ���λ
unsigned char bdata rbyte = 0;
sbit rdat7=rbyte^7;
sbit rdat6=rbyte^6;
sbit rdat5=rbyte^5;
sbit rdat4=rbyte^4;
sbit rdat3=rbyte^3;
sbit rdat2=rbyte^2;
sbit rdat1=rbyte^1;
sbit rdat0=rbyte^0; //ȡ��dat�ĸ���λ
void IOSPI_IO_Init(void)
{
 IOSPI_SI=1;
 IOSPI_SCK=1;
 IOSPI_SO=1; 
}

/******************************************************************
 - ����������IOģ��SPI������һ���ֽڣ�ͬʱ����һ���ֽ�
 - ����ģ�飺SPIģ��
 - �������ԣ��ⲿ�����û�����
 - ����˵����x:Ҫ���͵��ֽ�
 - ����˵�����������ֽ�
 - ע���ܶ�����£�SPI����Ҫ�нϸߵ��ٶȵģ��˺����в�ʹ���κ�ѭ��
       �ṹ����for(;;) while�ȵȣ�����ʹ����λѰַ����Ϊ������ٶ�
 ******************************************************************/

unsigned char IOSPI_ReadWriteByte(unsigned char data x) 
{
 /*----����ʹ��ѭ����λ�����ʵ�ַ�ʽ���ٶ�Ҫ��ֱ����λѰַ��˳��ִ�з�ʽʵ�����ö�
   ----��Ϊ���Ѵ󲿷ֵ�ʱ�仨����ѭ�����ӵĵ������Ƚ���λ��������
  unsigned char i=0;
  for(i=0;i<8;i++)
  {
   IOSPI_SI=x&(0x80>>i);
   IOSPI_SCK=0; 
   IOSPI_SCK=1;
  }
 ----------------------------------*/
	
 rbyte = 0;

 dat=x;	//��x��ֵ������λѰַ�ı���dat���Ա�ȡ������λ
	
 IOSPI_SO=1;
	
 IOSPI_SCK=1;

 
 IOSPI_SI=dat7; //ȡ����7��λ��д����������
 IOSPI_SCK=0; 
 
 if(IOSPI_SO) rdat7 = 1;
 IOSPI_SCK=1;	  //ʱ���߲��������أ����ݱ�д��
 

 IOSPI_SI=dat6; //ȡ����6��λ��д����������
 IOSPI_SCK=0; 
 
 if(IOSPI_SO) rdat6 = 1;
 IOSPI_SCK=1;	  //ʱ���߲��������أ����ݱ�д��
 
 
 IOSPI_SI=dat5; //ȡ����5��λ��д����������
 IOSPI_SCK=0; 
 
 if(IOSPI_SO) rdat5 = 1;
 IOSPI_SCK=1;	  //ʱ���߲��������أ����ݱ�д��
 
 
 IOSPI_SI=dat4; //ȡ����4��λ��д����������
 IOSPI_SCK=0; 
 
 if(IOSPI_SO) rdat4 = 1;
 IOSPI_SCK=1;	  //ʱ���߲��������أ����ݱ�д��
 
 
 IOSPI_SI=dat3; //ȡ����3��λ��д����������
 IOSPI_SCK=0; 
 
 if(IOSPI_SO) rdat3 = 1;
 IOSPI_SCK=1;	  //ʱ���߲��������أ����ݱ�д��
 
 
 IOSPI_SI=dat2; //ȡ����2��λ��д����������
 IOSPI_SCK=0; 
 
 if(IOSPI_SO) rdat2 = 1;
 IOSPI_SCK=1;	  //ʱ���߲��������أ����ݱ�д��
 
 
 IOSPI_SI=dat1; //ȡ����1��λ��д����������
 IOSPI_SCK=0; 
 
 if(IOSPI_SO) rdat1 = 1;
 IOSPI_SCK=1;	  //ʱ���߲��������أ����ݱ�д��
 
 
 IOSPI_SI=dat0; //ȡ����0��λ��д����������
 IOSPI_SCK=0; 
 
 if(IOSPI_SO) rdat0 = 1;
 IOSPI_SCK=1;	  //ʱ���߲��������أ����ݱ�д��
 
 
 return rbyte;
}

/******************************************************************
 - ����������IOģ��SPI������һ���ֽڣ�ͬʱ����һ���ֽ�(�����෴)
 - ����ģ�飺SPIģ��
 - �������ԣ��ⲿ�����û�����
 - ����˵����x:Ҫ���͵��ֽ�
 - ����˵�����������ֽ�
 - ע���ܶ�����£�SPI����Ҫ�нϸߵ��ٶȵģ��˺����в�ʹ���κ�ѭ��
       �ṹ����for(;;) while�ȵȣ�����ʹ����λѰַ����Ϊ������ٶ�
 ******************************************************************/
/*
unsigned char IOSPI_ReadWriteByte_R(unsigned char x) 
{
 //----����ʹ��ѭ����λ�����ʵ�ַ�ʽ���ٶ�Ҫ��ֱ����λѰַ��˳��ִ�з�ʽʵ�����ö�
   //----��Ϊ���Ѵ󲿷ֵ�ʱ�仨����ѭ�����ӵĵ������Ƚ���λ��������
  unsigned char i=0;
  for(i=0;i<8;i++)
  {
   IOSPI_SI=x&(0x80>>i);
   IOSPI_SCK=0; 
   IOSPI_SCK=1;
  }

	
 unsigned char idata rbyte=0;

 dat=x;	//��x��ֵ������λѰַ�ı���dat���Ա�ȡ������λ
	
 IOSPI_SO=1;
	
 IOSPI_SCK=0;
 //delay(IOSPI_DELAY_TIME);
 
 IOSPI_SI=dat7; //ȡ����7��λ��д����������
 IOSPI_SCK=1; 
 //delay(IOSPI_DELAY_TIME);
 if(IOSPI_SO) rbyte|=0x80;
 IOSPI_SCK=0;	  //ʱ���߲��������أ����ݱ�д��
 //delay(IOSPI_DELAY_TIME);

 IOSPI_SI=dat6; //ȡ����6��λ��д����������
 IOSPI_SCK=1; 
 //delay(IOSPI_DELAY_TIME);
 if(IOSPI_SO) rbyte|=0x40;
 IOSPI_SCK=0;	  //ʱ���߲��������أ����ݱ�д��
 //delay(IOSPI_DELAY_TIME);
 
 IOSPI_SI=dat5; //ȡ����5��λ��д����������
 IOSPI_SCK=1; 
// delay(IOSPI_DELAY_TIME);
 if(IOSPI_SO) rbyte|=0x20;
 IOSPI_SCK=0;	  //ʱ���߲��������أ����ݱ�д��
 //delay(IOSPI_DELAY_TIME);
 
 IOSPI_SI=dat4; //ȡ����4��λ��д����������
 IOSPI_SCK=1; 
 //delay(IOSPI_DELAY_TIME);
 if(IOSPI_SO) rbyte|=0x10;
 IOSPI_SCK=0;	  //ʱ���߲��������أ����ݱ�д��
 //delay(IOSPI_DELAY_TIME);
 
 IOSPI_SI=dat3; //ȡ����3��λ��д����������
 IOSPI_SCK=1; 
 //delay(IOSPI_DELAY_TIME);
 if(IOSPI_SO) rbyte|=0x08;
 IOSPI_SCK=0;	  //ʱ���߲��������أ����ݱ�д��
 //delay(IOSPI_DELAY_TIME);
 
 IOSPI_SI=dat2; //ȡ����2��λ��д����������
 IOSPI_SCK=1; 
 //delay(IOSPI_DELAY_TIME);
 if(IOSPI_SO) rbyte|=0x04;
 IOSPI_SCK=0;	  //ʱ���߲��������أ����ݱ�д��
 //delay(IOSPI_DELAY_TIME);
 
 IOSPI_SI=dat1; //ȡ����1��λ��д����������
 IOSPI_SCK=1; 
 //delay(IOSPI_DELAY_TIME);
 if(IOSPI_SO) rbyte|=0x02;
 IOSPI_SCK=0;	  //ʱ���߲��������أ����ݱ�д��
 //delay(IOSPI_DELAY_TIME);
 
 IOSPI_SI=dat0; //ȡ����0��λ��д����������
 IOSPI_SCK=1; 
 //delay(IOSPI_DELAY_TIME);
 if(IOSPI_SO) rbyte|=0x01;
 IOSPI_SCK=0;	  //ʱ���߲��������أ����ݱ�д��
 //delay(IOSPI_DELAY_TIME);
 
 return rbyte;
}
*/