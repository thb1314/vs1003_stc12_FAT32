/*******************************************************

        +------------------------------------+
        |  振南电子 基础资源驱动 数码管 部分 |
        +------------------------------------+

  此源码版权属 振南 全权享有，如欲引用，敬请署名并告知
        严禁随意用于商业目的，违者必究，后果自负
         振南电子 
             ->产品网站 http://www.znmcu.cn/
             ->产品网店 http://shop.znmcu.cn/
             ->产品咨询 QQ:987582714 
									  
********************************************************/
#ifndef  _VS1003_H_
#define  _VS1003_H_



#define MP3_RST P10
#define MP3_DREQ P11
#define MP3_XDCS P40
#define MP3_XCS  P41

//VS1003 内部定义   
#define VS_Write            0x02    //写命令
#define VS_Read             0x03    //读命令
//VS1003 寄存器定义
#define VS_MODE             0x00    //控制模式
#define VS_STATUS           0x01    //VS1003 状态
#define VS_BASS             0x02    //内置高/低音增强器
#define VS_CLOCKF           0x03    //时钟频率 + 倍频数
#define VS_DECODE_TIME      0x04    //每秒解码次数
#define VS_AUDATA           0x05    //音频数据
#define VS_WRAM             0x06    //RAM 读写
#define VS_WRAMADDR         0x07    //RAM 读写地址
#define VS_HDAT0            0x08    //流头数据0
#define VS_HDAT1            0x09    //流头数据1
#define VS_AIADDR           0x0A    //应用程序起始地址
#define VS_VOL              0x0B    //音量控制
#define VS_AICTRL0          0x0C    //应用控制寄存器0
#define VS_AICTRL1          0x0D    //应用控制寄存器1
#define VS_AICTRL2          0x0E    //应用控制寄存器2
#define VS_AICTRL3          0x0F    //应用控制寄存器3

void VS1003_IO_Init(void);
void VS_Reset(); //VS1003软复位及初始化
void VS_Write_Reg(unsigned char addr,unsigned char hdat,unsigned char ldat) reentrant; //向VS1003的功能寄存器写入一个字
unsigned int VS_Read_Reg(unsigned char addr); //从VS1003的功能寄存器读取一个字
void VS_Send_Dat(unsigned char dat); //向VS1003发送音频数据
void VS_Flush_Buffer(); //清空VS1003的数据缓冲区
void VS_sin_test(unsigned char x); //正弦测试
int VS_Enable_Record(void);
int VS_Disable_Record(void);

#endif
