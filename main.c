#include <STC12C5A60S2.H>

#include "sd.h"
#include "mytype.h"
#include "mystr.h"
#include "FAT32.H"
//VS1003
#include "vs1003.h"


u8 isnext = 0;

u8 code cmp[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
u8 yinliang = 0;
u8 volatile is_send_cmd = 0;
void main(void)
{

	u32 read_len = 0;
	u32 j = 0;
	u32 i = 0;
	//当前的根目录扇区 初始化为根目录扇区的第一个
	u32 pFirstSector = myFAT_Init_Args.FirstDirSector;
	u32 rootCluster = 2;
	
	//开启外部中断0
	
	IT0=1;//外部中断0控制方式选择，0为低电平，1为下降沿
	EX0=1;//外部中断0（INT0）源允许位
	IT1=1;//外部中断1控制方式选择，0为低电平，1为下降沿
	EX1=1;//外部中断1（INT0）源允许位
	EA=1;//CPU总中断开启
	
	
	VS_Reset();
	VS_sin_test(100);
	
	FAT32_Init(&myFAT_Init_Args);
	pFirstSector++;
	i = 5;
	//播放处理
	while(1)
	{
		
		SdReadSector(pFirstSector,response_buf);
		myfdis = (FDIesInSEC*)(response_buf);
		
		
		
		while( (i < 16) && myfdis->FDIes[i].Name[0] > 0 && (0 == mystrcmp(cmp,(unsigned char*)&myfdis->FDIes[i],16)) )
		{
			
			if((1 == mystrcmp("MP3",(unsigned char*)&myfdis->FDIes[i].Extension,3)))
			{
				MP3_XDCS=0;    //打开SDI，此时可以向VS1003写入音频数据
				Analyse_FDI(&fat32_fi,&myfdis->FDIes[i]);

				do
				{
					SdReadSector(fat32_fi.File_CurSec,response_buf);
					read_len = ((fat32_fi.File_Size-fat32_fi.File_CurOffset) > myFAT_Init_Args.BytesPerSector) ? myFAT_Init_Args.BytesPerSector : (fat32_fi.File_Size<fat32_fi.File_CurOffset) ? 0 : (fat32_fi.File_Size-fat32_fi.File_CurOffset);
					
					fat32_fi.File_CurOffset += read_len;
					//播放
					for(j = 0;j < read_len;j++)
					{
						VS_Send_Dat(response_buf[j]);
					}
						
					
					if(0 == read_len)
					{
						//VS_sin_test(100);
						fat32_fi.File_CurOffset = fat32_fi.File_Size;
						fat32_fi.File_IsEOF = 1;
						break;
						
					}
					else
					{
						fat32_fi.File_CurSec++;
					}
					//如果当前扇区的前个扇区是扇区的末尾说明可以更新簇了
					if(IS_END_SEC_OF_CLU(fat32_fi.File_CurSec-1,fat32_fi.File_CurClust))
					{
						
						fat32_fi.File_CurClust=Get_Next_Cluster(fat32_fi.File_CurClust);
						fat32_fi.File_CurSec = SOC(fat32_fi.File_CurClust);
					}
					
				} while(0 == isnext && 0 == fat32_fi.File_IsEOF);
				MP3_XDCS=1;	 //关闭SDI
				//清空VS1003的数据缓冲区，此函数中最后将VS_XDCS置高，即关闭SDI	
				VS_Flush_Buffer();

			}
			

			SdReadSector(pFirstSector,response_buf);

			myfdis = (FDIesInSEC*)(response_buf);
			
			i++;
			isnext = 0;

		}
		
		

		
		//如果到达目录末尾就重新置一
		pFirstSector++;
		i = 0;
		if(IS_END_SEC_OF_CLU(pFirstSector-1,rootCluster))
		{

			rootCluster=Get_Next_Cluster(rootCluster);
			if(IS_END_CLU(rootCluster))
			{
				
				rootCluster = 2;
				pFirstSector = myFAT_Init_Args.FirstDirSector;
			}
			else
			{
				VS_sin_test(100);
				pFirstSector = SOC(rootCluster);
			}
		}

	}
	


}
void exter0() interrupt 0//中断函数，无需声明，exter0自定义函数名，0为优先级最高的外部中断0使用号
{
	isnext = 1;
}

void exter1() interrupt 2
{
	yinliang += 3;
	if(yinliang > 30)
		yinliang = 0;
	VS_Write_Reg(VS_VOL,yinliang,yinliang);  //音量设置

}