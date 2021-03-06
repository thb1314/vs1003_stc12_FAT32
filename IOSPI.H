#ifndef  _IOSPI_H_
#define  _IOSPI_H_

/*******************************************************

        +----------------------------------------+
        | 振南电子 驱动程序模块 软件模拟SPI 部分 |
        +----------------------------------------+

  此源码版权属 振南 全权享有，如欲引用，敬请署名并告知
        严禁随意用于商业目的，违者必究，后果自负
         振南电子 
             ->产品网站 http://www.znmcu.cn/
             ->产品网店 http://shop.znmcu.cn/
             ->产品咨询 QQ:987582714 
									  
********************************************************/

#include <STC12C5A60S2.H>

sbit IOSPI_SCK=P1^4;  //SPI同步时钟  输出
sbit IOSPI_SI =P1^3;  //SPI同步数据  输出
sbit IOSPI_SO =P1^2;  //SPI同步数据  输入

#define IOSPI_DELAY_TIME 0 //设置越大，模拟SPI速度越慢

void IOSPI_IO_Init(void);
unsigned char IOSPI_ReadWriteByte(unsigned char x);
//unsigned char IOSPI_ReadWriteByte_R(unsigned char x);

#endif