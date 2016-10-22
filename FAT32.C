#include "FAT32.H"
#include "sd.h"

FAT_Init_Args xdata myFAT_Init_Args;
FDIesInSEC *myfdis;
FileInfo data fat32_fi;

u32 Bytes2Value(u8 *dat,u8 len)
{
	u32 temp =0;
	if(len >=1) temp |= ((u32)(dat[0]));
	if(len >=2) temp |= ((u32)(dat[1]) << 8);
	if(len >=3) temp |= ((u32)(dat[2]) << 16);
	if(len >=4) temp |= ((u32)(dat[3]) << 24);
	return temp;
}

u8 FAT32_Init(FAT_Init_Args* myinit_args)
{
	

	DBR *pmy_dbr = 0;
	u8 response = 2;

	//初始化SD卡
	SdSPIInit(0);
	response = SdReset();

	response = SdInit();
	if(0x00 != response)
		return 0;


	SdReadSector(MBR_SECTOR,response_buf);

	//myinit_args->
	if(!(response_buf[0]==0XEB && response_buf[1]==0X58 && response_buf[2]==0X90)) //检测0扇区是否为DBR扇区
	{
		myinit_args->BPB_Sector_No=Bytes2Value(((((MBR_Record *)(response_buf))->Parts[0]).StartLBA),4);
	}
	else
	{
		myinit_args->BPB_Sector_No=0;
	}
	SdReadSector(myinit_args->BPB_Sector_No,response_buf);
	pmy_dbr = (DBR *)response_buf;
	myinit_args->BytesPerSector  =Bytes2Value((pmy_dbr->BPB_BytesPerSec),2);//装入每扇区字节数到BytesPerSector中

	myinit_args->FATsectors      =Bytes2Value((pmy_dbr->BPB_FATSz32)    ,4);//装入FAT表占用的扇区数到FATsectors中

	myinit_args->SectorsPerClust =pmy_dbr->BPB_SecPerClus;//装入每簇扇区数到SectorsPerClust 中
	myinit_args->FirstFATSector  =Bytes2Value((pmy_dbr->BPB_RsvdSecCnt) ,2)+myinit_args->BPB_Sector_No;//装入第一个FAT表扇区号到FirstFATSector 中
	myinit_args->FirstDirSector  =(myinit_args->FirstFATSector)+(pmy_dbr->BPB_NumFATs)*(myinit_args->FATsectors); //装入第一个目录扇区到FirstDirSector中
	myinit_args->Total_SizeKB    =Bytes2Value((pmy_dbr->BPB_TotSec32),4)/2;  //磁盘的总容量，单位是KB
	return 1;
}


u8 To_File_Name(u8 *name_in_fdi,u8 *pfileName)
{
	 u8 i=0,n=7,m=10;

	 while(' '==name_in_fdi[n])
	 {
	  n--;
	 }
	 n++;

	 while(' '==name_in_fdi[m] && m>=8)
	 {
	  m--;
	 }
	 m-=7;

	 for(i=0;i<n;i++)
	 {
	  pfileName[i]=name_in_fdi[i];
	 }
	 pfileName[i]='.';

	 for(i=0;i<m;i++)
	 {
	  pfileName[n+i+1]=name_in_fdi[8+i];
	 }

	 if('.'==pfileName[n+m]) pfileName[n+m]=0;
	 else pfileName[n+m+1]=0;

	 return 0;
}
u32 StringLen(u8 *pstr)
{
 u32 len=0;
 while('\0'!=pstr[len]) 
 {
  len++;
 }
 return len;
}
u8 Analyse_FDI(FileInfo *pfi,FDI *pFDI)
{
	u32 temp=0,i=0;



	To_File_Name((u8 *)pFDI,pfi->File_Name);

	temp=(StringLen(pfi->File_Name)-1);
	while('.'!=(pfi->File_Name)[temp] && temp>0)
	{
		if(((pFDI->LowerCase)&0x10)!=0) 
		{
			(pfi->File_Name)[temp]=(u8)Upper2Low((pfi->File_Name)[temp]);
		}
		temp--;
	}
	if(((pFDI->LowerCase)&0x08)!=0) 
	{
		for(i=0;i<temp;i++)
		{
			(pfi->File_Name)[i]=(u8)Upper2Low((pfi->File_Name)[i]);   
		}
	}

	temp=(StringLen(pfi->File_Name)-1); 
	if(CHK_ATTR_DIR(pFDI->Attributes)) //如果是目录则将最后的.去掉
	{
		(pfi->File_Name)[temp+1]='\0';
	}
	//==以上是按照LowerCase字节对主文件名与扩展文件名进行小写化

	pfi->File_Attr=pFDI->Attributes; //文件属性
	pfi->File_StartClust=Bytes2Value(pFDI->LowClust,2)+Bytes2Value(pFDI->HighClust,2)*65536;
	pfi->File_Size=Bytes2Value(pFDI->FileSize,4);

	//解析文件创建时间与日期
	temp=Bytes2Value(pFDI->CTime,2);
	pfi->File_CTime.sec=(u8)((temp&TIME_SEC_MARK)*2); temp>>=TIME_SEC_NBITS;  //创建时间的2秒位
	pfi->File_CTime.min=(u8)(temp&TIME_MIN_MARK);   temp>>=TIME_MIN_NBITS; //创建时间的分位
	pfi->File_CTime.hour=(u8)(temp&TIME_HOUR_MARK); //创建时间的时位
	pfi->File_CTime.sec+=(u8)((u16)(pFDI->CTime10ms)/100); //在秒上加上10毫秒位

	temp=Bytes2Value(pFDI->CDate,2);
	pfi->File_CDate.day=(u8)(temp&DATE_DAY_MARK);     temp>>=DATE_DAY_NBITS;   //创建日期的日位
	pfi->File_CDate.month=(u8)(temp&DATE_MONTH_MARK); temp>>=DATE_MONTH_NBITS; //创建日期的月位
	pfi->File_CDate.year=(u16)((temp&DATE_YEAR_MARK)+DATE_YEAR_BASE); //创建日期的年位（加上年份基数）

	//解析文件修改时间与日期
	//temp=Bytes2Value(pFDI->MTime,2);
	//pfi->File_MTime.sec=(u8)((temp&TIME_SEC_MARK)*2); temp>>=TIME_SEC_NBITS;  //创建时间的2秒位
	//pfi->File_MTime.min=(u8)(temp&TIME_MIN_MARK);   temp>>=TIME_MIN_NBITS; //创建时间的分位
	//pfi->File_MTime.hour=(u8)(temp&TIME_HOUR_MARK); //创建时间的时位
	//文件的修改时间没有10毫秒位，所以它只能表达偶数秒

	//temp=Bytes2Value(pFDI->MDate,2);
	//pfi->File_MDate.day=(u8)(temp&DATE_DAY_MARK);     temp>>=DATE_DAY_NBITS;   //创建日期的日位
	//pfi->File_MDate.month=(u8)(temp&DATE_MONTH_MARK); temp>>=DATE_MONTH_NBITS; //创建日期的月位
	//pfi->File_MDate.year=(u8)((temp&DATE_YEAR_MARK)+DATE_YEAR_BASE); //创建日期的年位

	//解析文件访问日期
	//temp=Bytes2Value(pFDI->ADate,2);
	//pfi->File_ADate.day=(u8)(temp&DATE_DAY_MARK);     temp>>=DATE_DAY_NBITS;   //创建日期的日位
	//pfi->File_ADate.month=(u8)(temp&DATE_MONTH_MARK); temp>>=DATE_MONTH_NBITS; //创建日期的月位
	//pfi->File_ADate.year=(u8)((temp&DATE_YEAR_MARK)+DATE_YEAR_BASE); //创建日期的年位

	pfi->File_CurClust=pfi->File_StartClust;
	pfi->File_CurSec = (pfi->File_CurClust) ? SOC(pfi->File_CurClust) :  0;
	pfi->File_CurPos=0;
	pfi->File_CurOffset=0;
	pfi->File_IsEOF=(u8)((pfi->File_StartClust)?0:1);

	return 0;
}

u32 Get_Next_Cluster(u32 cluster)
{
	u32 clu_sec=0;
	FAT_Sec *pFAT_Sec;
	FAT_Item *pFAT_Item;


	clu_sec=(cluster/NITEMSINFATSEC)+(myFAT_Init_Args.FirstFATSector); //指定簇的簇项所在的扇区为其FAT区内的偏移量加上
														  
	SdReadSector(clu_sec,response_buf); //将簇项所在的扇区数据读入缓冲区

	pFAT_Sec=(FAT_Sec *)&response_buf; //将数据缓冲区首地址强转为FAT_Sec结构体的指针类型

	pFAT_Item=&((pFAT_Sec->items)[cluster%NITEMSINFATSEC]); //获取指定簇的簇项在所在扇区中的地址

	return Bytes2Value((u8 *)pFAT_Item,NFATITEMBYTES); //返回簇项的值，即指定簇下一簇的簇号
}


/*
thb 原创
读取文件内容

 u8 File_Name[13];	    //完整文件名（主文件名与扩展文件名）
 u8 File_Attr;		    //文件属性
 struct Time File_CTime;    //文件创建时间
 struct Date File_CDate;    //文件创建日期
 //struct Date File_ADate;    //文件访问日期
 //struct Time File_MTime;	//文件修改时间
 //struct Date File_MDate;	//文件修改日期
 u32 File_StartClust;    //文件开始簇
 u32 File_Size;		    //文件大小

 u32 File_CurClust;   //文件当前簇
 u32 File_CurSec;     //文件当前扇区
 u16 File_CurPos;	 //文件当前扇区偏移量(相对于扇区的)

 u8  File_IsEOF;      //文件是否到达结束位置

 u32 File_CurOffset;	 //文件当前偏移量

*/

/*
返回是否读取完 0 还有数据 1读取完毕
*/
/*
u16 FAT_Read_File(FileInfo *fileinfo,u8 *buffer)
{

	u16 data read_len = 0;
	SdReadSector(fileinfo->File_CurSec,buffer);
	read_len = ((fileinfo->File_Size-fileinfo->File_CurOffset) > myFAT_Init_Args.BytesPerSector) ? myFAT_Init_Args.BytesPerSector : (fileinfo->File_Size-fileinfo->File_CurOffset);
	
	fileinfo->File_CurOffset += read_len;
	
	
	if(fileinfo->File_CurOffset >= fileinfo->File_Size)
	{
		fileinfo->File_CurOffset = fileinfo->File_Size;
		fileinfo->File_IsEOF = 1;
		
	}
	else
	{
		fileinfo->File_CurSec++;
	}
	//如果当前扇区的前个扇区是扇区的末尾说明可以更新簇了
	if(IS_END_SEC_OF_CLU(fileinfo->File_CurSec-1,fileinfo->File_CurClust))
	{
		
		fileinfo->File_CurClust=Get_Next_Cluster(fileinfo->File_CurClust);
		//if(IS_END_CLU(fileinfo->File_CurClust))
		fileinfo->File_CurSec = SOC(fileinfo->File_CurClust);
	}
	

	return read_len;

}
*/
