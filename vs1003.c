/*******************************************************

        +------------------------------+
        |振南电子 高级外设 VS1003部分  |
        +------------------------------+

  此源码版权属 振南 全权享有，如欲引用，敬请署名并告知
        严禁随意用于商业目的，违者必究，后果自负
         振南电子 
             ->产品网站 http://www.znmcu.cn/
             ->产品网店 http://shop.znmcu.cn/
             ->产品咨询 QQ:987582714 
										  
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
 - 功能描述：向VS1003的功能寄存器中写入数据（一个字，即两个字节）
 - 隶属模块：VS1003B模块
 - 函数属性：外部，用户可调用
 - 参数说明：addr是功能寄存器的地址
             hdat是要写入的高字节
             ldat是要写入的低字节
 - 返回说明：无返回
 ******************************************************************/

void VS_Write_Reg(unsigned char addr,unsigned char hdat,unsigned char ldat) reentrant
{  
 while(!MP3_DREQ);     //VS1003的DREQ为高电平时才接收数据
 MP3_XCS=0;            //打开片选，SCI有效，这样才能对功能寄存器进行读写
 IOSPI_ReadWriteByte(VS_Write);  //写入操作码0x02   00000010 （功能寄存器写操作）
 IOSPI_ReadWriteByte(addr);  //写入寄存器地址
 IOSPI_ReadWriteByte(hdat);  //写入高字节
 IOSPI_ReadWriteByte(ldat);  //写入低字节   
 MP3_XCS=1;            //关闭片选，SCI无效
}

/******************************************************************
 - 功能描述：从VS1003的功能寄存器中读取数据（一个字）
 - 隶属模块：VS1003B模块
 - 函数属性：外部，用户可调用
 - 参数说明：addr是功能寄存器的地址
 - 返回说明：返回从VS1003的功能寄存器中读到的值 
 ******************************************************************/
/*
unsigned int VS_Read_Reg(unsigned char addr) 
{
 unsigned int temp=0;
              
 while(!MP3_DREQ); //VS1003的DREQ为高电平时才接收数据

 MP3_XCS=0;	      //打开片选，SCI有效
 IOSPI_ReadWriteByte(VS_Read);  //读出操作码0x03   00000011（功能寄存器读操作）
 IOSPI_ReadWriteByte(addr);  //写入寄存器地址

 temp=IOSPI_ReadWriteByte(0XFF);  //读高字节
 temp<<=8;
 temp|=IOSPI_ReadWriteByte(0XFF); //读取低字节，与高字节拼成一个字

 MP3_XCS=1;	      //关闭片选，SCI无效
 return temp;     //返回读到的值
}
*/
/******************************************************************
 - 功能描述：VS1003软复位及初始化（设置时钟频率及音量）
 - 隶属模块：VS1003B模块
 - 函数属性：外部，用户可调用
 - 参数说明：无
 - 返回说明：无
 ******************************************************************/

void VS_Reset()
{
 unsigned long xdata i = 5000;
 VS1003_IO_Init();
 VS_Flush_Buffer();
 while(i--);

 VS_Write_Reg(VS_MODE  ,0x08,0x04);  //软件复位，向0号寄存器写入0x0804   SM_SDINEW为1   SM_RESET为1
 VS_Write_Reg(VS_CLOCKF,0x98,0x00);  //时钟设置，向3号寄存器写入0x9800   SC_MULT  为4   SC_ADD  为3   SC_FREQ为0
 //VS_Write_Reg(VS_VOL   ,0x00,0x00);  //音量设置，左右声道均最大音量
 VS_Write_Reg(VS_VOL   ,0x00,0x00);  //音量设置
   
 MP3_XDCS=0;	     //打开数据片选，注意此时XCS（片选）为高电平，SDI有效
 IOSPI_ReadWriteByte(0);    //写入数据，这里写入4个0，是无关数据，用来启动数据传输
 IOSPI_ReadWriteByte(0);
 IOSPI_ReadWriteByte(0);
 IOSPI_ReadWriteByte(0);
	
	
 MP3_XDCS=1;	    //关闭数据片选，SDI无效
} 

/******************************************************************
 - 功能描述：向VS1003写入一个字节的音频数据（即用于播放的数据）
             注：调用前先将VS_XDCS置为0，打开数据片选
 - 隶属模块：VS1003B模块
 - 函数属性：外部，用户可调用
 - 参数说明：dat是要写入的字节
 - 返回说明：无
 ******************************************************************/

void VS_Send_Dat(unsigned char data dat) 
{
 while(!MP3_DREQ);  //VS1003的DREQ为高才能写入数据
 IOSPI_ReadWriteByte(dat);//通过SPI向VS1003写入一个字节的音频数据
}

/******************************************************************
 - 功能描述：向VS1003写入2048个0，用于清空VS1003的数据缓冲区
             注：在播放完一个完整的音频（如一首完整的MP3）后，调用
             此函数，清空VS1003数据缓冲区，为下面的音频数据（如下
             一首MP3）作准备。        
 - 隶属模块：VS1003B模块
 - 函数属性：外部，用户可调用
 - 参数说明：无
 - 返回说明：无
 ******************************************************************/

void VS_Flush_Buffer() 
{
 unsigned int xdata i;
 MP3_XDCS=0;	   //打开数据片选，即开启SDI传输
 for(i=0;i<2048;i++)
 {
	VS_Send_Dat(0);
 }
 MP3_XDCS=1;        //关闭数据片选
}

/******************************************************************
 - 功能描述：正弦测试，这是测试VS1003芯片是否正常的有效手段！！
 - 隶属模块：VS1003B模块
 - 函数属性：外部，用户可调用
 - 参数说明：x决定了正弦测试中产生的正弦波的频率，直接影响听到的
             声音的频率      
 - 返回说明：无
 ******************************************************************/

void VS_sin_test(unsigned char x)
{ 
 unsigned long xdata i = 0;
 VS_Write_Reg(0x00,0x08,0x20);//启动测试，向0号寄存器写入0x0820   SM_SDINEW为1   SM_TEST为1

 while(!MP3_DREQ);   //等待DREQ变为高电平
 MP3_XDCS=0;	        //打开数据片选 SDI有效
 IOSPI_ReadWriteByte(0x53);//写入以下8个字节,进入正弦测试
 IOSPI_ReadWriteByte(0xef); 
 IOSPI_ReadWriteByte(0x6e);
 IOSPI_ReadWriteByte(x);   //参数x用来调整正弦测试中正弦波的频率   FsIdx (b7~b5):采样率表索引   S (b4~b0):正弦波的跃速   频率F=Fs X S / 128
 IOSPI_ReadWriteByte(0);   //比如x=126 (0b 011 11110) FsIdx=011=3   Fs=22050Hz   S=11110=30    F=22050Hz X 30 /128 =5168 Hz
 IOSPI_ReadWriteByte(0);
 IOSPI_ReadWriteByte(0);
 IOSPI_ReadWriteByte(0);
 i = 60000;
 while(i--);
 i = 60000;
 while(i--);

 //delay(60000);delay(60000);delay(60000);delay(60000);//这里延时一段时间，为了听到“正弦音”
 IOSPI_ReadWriteByte(0x45);//写入以下8个字节，退出正弦测试
 IOSPI_ReadWriteByte(0x78); 
 IOSPI_ReadWriteByte(0x69);
 IOSPI_ReadWriteByte(0x74);
 IOSPI_ReadWriteByte(0);
 IOSPI_ReadWriteByte(0);
 IOSPI_ReadWriteByte(0);
 IOSPI_ReadWriteByte(0);
 MP3_XDCS=1;	    //关闭数据片选 ，SDI无效
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

