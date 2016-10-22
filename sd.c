#include "sd.h"

#include <STC12C5A60S2.H>

u8 xdata response_buf[512] = {0x00};
//硬件SPI初始化程序//
//参数：speed 其有效取值为：0 1 2 3 分别对应 4/16/64/128分频//
void SdSPIInit(unsigned char speed)	//spi初始化//
{
    SPDAT = 0;         	//数据寄存器清0
    SPSTAT = 0xC0;  	//清SPI状态SPIF和WCOL写1清0
	  SPCTL = 0xDC | speed;		//1101 1111 主模式,使能,先MSB,前时钟为下降沿，并在下降沿驱动数据，上升沿检测数据//				 
}

//写一字节到SD卡,模拟SPI总线方式
void SdWrite(unsigned char n)
{
	/*
	unsigned char i;
	SD_CLK = 1;
	for(i=0;i<8;i++)
	{
		SD_CLK = 0;
		if(0x80 == (n&0x80))
			SD_DI = 1;
		else
			SD_DI = 0;
		n <<= 1;

		SD_CLK = 1;
	}
	SD_DI=1;
	*/
	SPDAT=n;
	while((SPSTAT&0x80)==0);
	SPSTAT=0xC0;  //SPIF写1清0 
}
//===========================================================
//从SD卡读一字节,模拟SPI总线方式
unsigned char SdRead(void)
{
	
	SPDAT = 0xff;
	while((SPSTAT&0x80)==0);
	SPSTAT=0xC0;  //  SPIF写1清0 
	return SPDAT;
}

void SdReadMore(unsigned char *buffer,unsigned int len)
{
	while(len--)
	{
		*buffer = SdRead();
		buffer++;
	}
}
	
	

//================================================================
//发命令到SD卡
void SdCommand(unsigned char command, unsigned long argument, unsigned char CRC)
{
	SD_CS = 1;
	SdWrite(0xff);
	SdWrite(0xff);
	SdWrite(0xff);
	SD_CS = 0;
	

	command &= 0x3F;
	command |= 0x40;
	SdWrite(command);
	SdWrite(((unsigned char *)&argument)[0]);
	SdWrite(((unsigned char *)&argument)[1]);
	SdWrite(((unsigned char *)&argument)[2]);
	SdWrite(((unsigned char *)&argument)[3]);
	SdWrite(CRC);

	SdWrite(0xFF);
	
}

//复位SD卡
unsigned char SdReset(void)
{
	unsigned char response = 0xFF;
	unsigned int i=0;
	//发送至少72个脉冲
	SD_CS = 1;
	for(i=1;i<=150;i++)
	{
		SdWrite(0xff);
	}

		
	
	
	i = 0;
	do
	{
		SdCommand(0x00,0x00000000L,0x95);
		response = SdRead();
		i++;
	}
	while (response != 0x01 && i < 10000 );
	

	return response;
}
//**********************
//初始化SD卡
unsigned char SdInit(void)
{
	unsigned char i = 0,j = 0,response = 0xFF;
	
	//判断卡类型
	i = 0;
	do
	{
		
		SdCommand(0x08,0x000001aa,0x87);
		for(j = 0;j<200;j++)
		{
			response = SdRead();
			if(response == 0x01)
				break;
		}
		
		i++;
	} while ( response != 0x01 && i < 200 );
	
	if(0x01 == response)
	{
		response = 0xFF;
		i = 0;
		do
		{
			SdCommand(55,0x00000000,0xFF);

				
			response = SdRead();


				


			if( 0x01 == response )
			{
					response = 0xFF;
					SdCommand(41,0x40000000,0xFF);
					response = SdRead();
					if( 0x00 == response )
						break;
			
				
			}
				
			i++;

		} while ( response != 0x00 && i < 200 );
	}
	
	
	
	//下面仅适用于SD2.0
	/*
	if(0x01 == response)
	{
		response = 0xFF;
		i = 0;
		do
		{
			SdCommand(55,0x00000000,0xFF);
			
			for(j = 0;j < 200;j++)
			{
				
				response = SdRead();
				if( 0xFF != response )
					break;
			}
			i++;
		} while ( response != 0xFF && i < 200 );
	}
	*/
	/*
	if(0x01 == response)
	{
		response = 0xFF;
		i = 0;
		do
		{
			SdCommand(41,0x40000000,0xFF);
			
			for(j = 0;j < 200;j++)
			{
				
				response = SdRead();
				if( 0xFF != response )
					break;
			}
			i++;
		} while ( response != 0xFF && i < 200 );
	}
	*/
	
	SD_CS = 1; 
	SdWrite(0xff);
	return response;

}
/*
unsigned char SdGetCSD(unsigned char * buffer)
{

	unsigned char i = 0,j = 0;
	do
	{
		j = 0;
		SdCommand(9,0x00UL,0xFF);
		while(SdRead() != 0xFE && j <20) j++;

		if(j != 20)
		{
			SdReadMore(buffer,16);
			i = 6;
			while(i--)
				SdRead();
			
			i = 20;
		}
		
		i++;
	} while(i < 20);
	return  (i == 21) ? 1 : 0;
}

unsigned long SdCalcSize(unsigned char* csd)
{
	unsigned int n;
	unsigned long csize,Capacity;
	if((csd[0]&0xC0)==0x40)//判断bit126是否为1
	{ 
		csize = csd[9] + ((unsigned long)csd[8] << 8) + ((unsigned long)(csd[7] & 63) << 16) + 1;
		Capacity = csize << 9; 
	}
	else
	{ 
		n = (csd[5] & 0x0F) + ((csd[10] & 0x80) >> 7) + ((csd[9] & 0x03) << 1) + 2;
		csize = (csd[8] >> 6) + ((unsigned int)csd[7] << 2) + ((unsigned int)(csd[6] & 0x03) << 10) + 1;
		Capacity = (unsigned long)csize << (n - 10);
	}
	return Capacity;
 
}
*/
unsigned char SdReadSector(unsigned long sector,unsigned char *buffer)
{
	unsigned char i = 0,j = 0,response = 0;

	SdCommand(17,sector,0xFF);

	if(0x00 == SdRead())
		while(SdRead() != 0xFE);
	else
		return 0;
	SdReadMore(buffer,512);
	SdRead();
	SdRead();//读取两个字节的CRC校验码，不用关心它们


	return   1;
}

