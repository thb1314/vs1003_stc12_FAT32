/*******************************************************

        +------------------------------+
        |���ϵ��� �߼����� VS1003����  |
        +------------------------------+

  ��Դ���Ȩ�� ���� ȫȨ���У��������ã�������������֪
        �Ͻ�����������ҵĿ�ģ�Υ�߱ؾ�������Ը�
         ���ϵ��� 
             ->��Ʒ��վ http://www.znmcu.cn/
             ->��Ʒ���� http://shop.znmcu.cn/
             ->��Ʒ��ѯ QQ:987582714 
										  
********************************************************/
#include <STC12C5A60S2.H>

#include "vs1003.h"
#include "iospi.h"


void VS1003_IO_Init(void)
{
 MP3_RST=1;
 MP3_DREQ=1;
 MP3_XDCS=1;
 MP3_XCS=1;
	
 IOSPI_IO_Init();
}

/******************************************************************
 - ������������VS1003�Ĺ��ܼĴ�����д�����ݣ�һ���֣��������ֽڣ�
 - ����ģ�飺VS1003Bģ��
 - �������ԣ��ⲿ���û��ɵ���
 - ����˵����addr�ǹ��ܼĴ����ĵ�ַ
             hdat��Ҫд��ĸ��ֽ�
             ldat��Ҫд��ĵ��ֽ�
 - ����˵�����޷���
 ******************************************************************/

void VS_Write_Reg(unsigned char addr,unsigned char hdat,unsigned char ldat) reentrant
{  
 while(!MP3_DREQ);     //VS1003��DREQΪ�ߵ�ƽʱ�Ž�������
 MP3_XCS=0;            //��Ƭѡ��SCI��Ч���������ܶԹ��ܼĴ������ж�д
 IOSPI_ReadWriteByte(VS_Write);  //д�������0x02   00000010 �����ܼĴ���д������
 IOSPI_ReadWriteByte(addr);  //д��Ĵ�����ַ
 IOSPI_ReadWriteByte(hdat);  //д����ֽ�
 IOSPI_ReadWriteByte(ldat);  //д����ֽ�   
 MP3_XCS=1;            //�ر�Ƭѡ��SCI��Ч
}

/******************************************************************
 - ������������VS1003�Ĺ��ܼĴ����ж�ȡ���ݣ�һ���֣�
 - ����ģ�飺VS1003Bģ��
 - �������ԣ��ⲿ���û��ɵ���
 - ����˵����addr�ǹ��ܼĴ����ĵ�ַ
 - ����˵�������ش�VS1003�Ĺ��ܼĴ����ж�����ֵ 
 ******************************************************************/
/*
unsigned int VS_Read_Reg(unsigned char addr) 
{
 unsigned int temp=0;
              
 while(!MP3_DREQ); //VS1003��DREQΪ�ߵ�ƽʱ�Ž�������

 MP3_XCS=0;	      //��Ƭѡ��SCI��Ч
 IOSPI_ReadWriteByte(VS_Read);  //����������0x03   00000011�����ܼĴ�����������
 IOSPI_ReadWriteByte(addr);  //д��Ĵ�����ַ

 temp=IOSPI_ReadWriteByte(0XFF);  //�����ֽ�
 temp<<=8;
 temp|=IOSPI_ReadWriteByte(0XFF); //��ȡ���ֽڣ�����ֽ�ƴ��һ����

 MP3_XCS=1;	      //�ر�Ƭѡ��SCI��Ч
 return temp;     //���ض�����ֵ
}
*/
/******************************************************************
 - ����������VS1003��λ����ʼ��������ʱ��Ƶ�ʼ�������
 - ����ģ�飺VS1003Bģ��
 - �������ԣ��ⲿ���û��ɵ���
 - ����˵������
 - ����˵������
 ******************************************************************/

void VS_Reset()
{
 unsigned long xdata i = 5000;
 VS1003_IO_Init();
 VS_Flush_Buffer();
 while(i--);

 VS_Write_Reg(VS_MODE  ,0x08,0x04);  //�����λ����0�żĴ���д��0x0804   SM_SDINEWΪ1   SM_RESETΪ1
 VS_Write_Reg(VS_CLOCKF,0x98,0x00);  //ʱ�����ã���3�żĴ���д��0x9800   SC_MULT  Ϊ4   SC_ADD  Ϊ3   SC_FREQΪ0
 //VS_Write_Reg(VS_VOL   ,0x00,0x00);  //�������ã������������������
 VS_Write_Reg(VS_VOL   ,0x00,0x00);  //��������
   
 MP3_XDCS=0;	     //������Ƭѡ��ע���ʱXCS��Ƭѡ��Ϊ�ߵ�ƽ��SDI��Ч
 IOSPI_ReadWriteByte(0);    //д�����ݣ�����д��4��0�����޹����ݣ������������ݴ���
 IOSPI_ReadWriteByte(0);
 IOSPI_ReadWriteByte(0);
 IOSPI_ReadWriteByte(0);
	
	
 MP3_XDCS=1;	    //�ر�����Ƭѡ��SDI��Ч
} 

/******************************************************************
 - ������������VS1003д��һ���ֽڵ���Ƶ���ݣ������ڲ��ŵ����ݣ�
             ע������ǰ�Ƚ�VS_XDCS��Ϊ0��������Ƭѡ
 - ����ģ�飺VS1003Bģ��
 - �������ԣ��ⲿ���û��ɵ���
 - ����˵����dat��Ҫд����ֽ�
 - ����˵������
 ******************************************************************/

void VS_Send_Dat(unsigned char data dat) 
{
 while(!MP3_DREQ);  //VS1003��DREQΪ�߲���д������
 IOSPI_ReadWriteByte(dat);//ͨ��SPI��VS1003д��һ���ֽڵ���Ƶ����
}

/******************************************************************
 - ������������VS1003д��2048��0���������VS1003�����ݻ�����
             ע���ڲ�����һ����������Ƶ����һ��������MP3���󣬵���
             �˺��������VS1003���ݻ�������Ϊ�������Ƶ���ݣ�����
             һ��MP3����׼����        
 - ����ģ�飺VS1003Bģ��
 - �������ԣ��ⲿ���û��ɵ���
 - ����˵������
 - ����˵������
 ******************************************************************/

void VS_Flush_Buffer() 
{
 unsigned int xdata i;
 MP3_XDCS=0;	   //������Ƭѡ��������SDI����
 for(i=0;i<2048;i++)
 {
	VS_Send_Dat(0);
 }
 MP3_XDCS=1;        //�ر�����Ƭѡ
}

/******************************************************************
 - �������������Ҳ��ԣ����ǲ���VS1003оƬ�Ƿ���������Ч�ֶΣ���
 - ����ģ�飺VS1003Bģ��
 - �������ԣ��ⲿ���û��ɵ���
 - ����˵����x���������Ҳ����в��������Ҳ���Ƶ�ʣ�ֱ��Ӱ��������
             ������Ƶ��      
 - ����˵������
 ******************************************************************/

void VS_sin_test(unsigned char x)
{ 
 unsigned long xdata i = 0;
 VS_Write_Reg(0x00,0x08,0x20);//�������ԣ���0�żĴ���д��0x0820   SM_SDINEWΪ1   SM_TESTΪ1

 while(!MP3_DREQ);   //�ȴ�DREQ��Ϊ�ߵ�ƽ
 MP3_XDCS=0;	        //������Ƭѡ SDI��Ч
 IOSPI_ReadWriteByte(0x53);//д������8���ֽ�,�������Ҳ���
 IOSPI_ReadWriteByte(0xef); 
 IOSPI_ReadWriteByte(0x6e);
 IOSPI_ReadWriteByte(x);   //����x�����������Ҳ��������Ҳ���Ƶ��   FsIdx (b7~b5):�����ʱ�����   S (b4~b0):���Ҳ���Ծ��   Ƶ��F=Fs X S / 128
 IOSPI_ReadWriteByte(0);   //����x=126 (0b 011 11110) FsIdx=011=3   Fs=22050Hz   S=11110=30    F=22050Hz X 30 /128 =5168 Hz
 IOSPI_ReadWriteByte(0);
 IOSPI_ReadWriteByte(0);
 IOSPI_ReadWriteByte(0);
 i = 60000;
 while(i--);
 i = 60000;
 while(i--);

 //delay(60000);delay(60000);delay(60000);delay(60000);//������ʱһ��ʱ�䣬Ϊ����������������
 IOSPI_ReadWriteByte(0x45);//д������8���ֽڣ��˳����Ҳ���
 IOSPI_ReadWriteByte(0x78); 
 IOSPI_ReadWriteByte(0x69);
 IOSPI_ReadWriteByte(0x74);
 IOSPI_ReadWriteByte(0);
 IOSPI_ReadWriteByte(0);
 IOSPI_ReadWriteByte(0);
 IOSPI_ReadWriteByte(0);
 MP3_XDCS=1;	    //�ر�����Ƭѡ ��SDI��Ч
} 
/*
int VS_Enable_Record(void)
{
 VS_Write_Reg(VS_AICTRL0,0x00,0x12);
 VS_Write_Reg(VS_AICTRL1,0x10,0x00);

 VS_Write_Reg(VS_MODE,0x38,0x04);

 VS_Write_Reg(VS_BASS,0x00,0x00);

 VS_Write_Reg(VS_CLOCKF,0x98,0x00);

 return 0;
}

int VS_Disable_Record(void)
{
 VS_Write_Reg(VS_MODE,0x08,0x04);
	
 return 0;
}

*/

