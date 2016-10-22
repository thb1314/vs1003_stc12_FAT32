/*******************************************************

        +------------------------------------+
        |  ���ϵ��� ������Դ���� ����� ���� |
        +------------------------------------+

  ��Դ���Ȩ�� ���� ȫȨ���У��������ã�������������֪
        �Ͻ�����������ҵĿ�ģ�Υ�߱ؾ�������Ը�
         ���ϵ��� 
             ->��Ʒ��վ http://www.znmcu.cn/
             ->��Ʒ���� http://shop.znmcu.cn/
             ->��Ʒ��ѯ QQ:987582714 
									  
********************************************************/
#ifndef  _VS1003_H_
#define  _VS1003_H_



#define MP3_RST P10
#define MP3_DREQ P11
#define MP3_XDCS P40
#define MP3_XCS  P41

//VS1003 �ڲ�����   
#define VS_Write            0x02    //д����
#define VS_Read             0x03    //������
//VS1003 �Ĵ�������
#define VS_MODE             0x00    //����ģʽ
#define VS_STATUS           0x01    //VS1003 ״̬
#define VS_BASS             0x02    //���ø�/������ǿ��
#define VS_CLOCKF           0x03    //ʱ��Ƶ�� + ��Ƶ��
#define VS_DECODE_TIME      0x04    //ÿ��������
#define VS_AUDATA           0x05    //��Ƶ����
#define VS_WRAM             0x06    //RAM ��д
#define VS_WRAMADDR         0x07    //RAM ��д��ַ
#define VS_HDAT0            0x08    //��ͷ����0
#define VS_HDAT1            0x09    //��ͷ����1
#define VS_AIADDR           0x0A    //Ӧ�ó�����ʼ��ַ
#define VS_VOL              0x0B    //��������
#define VS_AICTRL0          0x0C    //Ӧ�ÿ��ƼĴ���0
#define VS_AICTRL1          0x0D    //Ӧ�ÿ��ƼĴ���1
#define VS_AICTRL2          0x0E    //Ӧ�ÿ��ƼĴ���2
#define VS_AICTRL3          0x0F    //Ӧ�ÿ��ƼĴ���3

void VS1003_IO_Init(void);
void VS_Reset(); //VS1003��λ����ʼ��
void VS_Write_Reg(unsigned char addr,unsigned char hdat,unsigned char ldat) reentrant; //��VS1003�Ĺ��ܼĴ���д��һ����
unsigned int VS_Read_Reg(unsigned char addr); //��VS1003�Ĺ��ܼĴ�����ȡһ����
void VS_Send_Dat(unsigned char dat); //��VS1003������Ƶ����
void VS_Flush_Buffer(); //���VS1003�����ݻ�����
void VS_sin_test(unsigned char x); //���Ҳ���
int VS_Enable_Record(void);
int VS_Disable_Record(void);

#endif
