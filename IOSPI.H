#ifndef  _IOSPI_H_
#define  _IOSPI_H_

/*******************************************************

        +----------------------------------------+
        | ���ϵ��� ��������ģ�� ����ģ��SPI ���� |
        +----------------------------------------+

  ��Դ���Ȩ�� ���� ȫȨ���У��������ã�������������֪
        �Ͻ�����������ҵĿ�ģ�Υ�߱ؾ�������Ը�
         ���ϵ��� 
             ->��Ʒ��վ http://www.znmcu.cn/
             ->��Ʒ���� http://shop.znmcu.cn/
             ->��Ʒ��ѯ QQ:987582714 
									  
********************************************************/

#include <STC12C5A60S2.H>

sbit IOSPI_SCK=P1^4;  //SPIͬ��ʱ��  ���
sbit IOSPI_SI =P1^3;  //SPIͬ������  ���
sbit IOSPI_SO =P1^2;  //SPIͬ������  ����

#define IOSPI_DELAY_TIME 0 //����Խ��ģ��SPI�ٶ�Խ��

void IOSPI_IO_Init(void);
unsigned char IOSPI_ReadWriteByte(unsigned char x);
//unsigned char IOSPI_ReadWriteByte_R(unsigned char x);

#endif