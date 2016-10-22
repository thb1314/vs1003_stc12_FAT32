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

	//��ʼ��SD��
	SdSPIInit(0);
	response = SdReset();

	response = SdInit();
	if(0x00 != response)
		return 0;


	SdReadSector(MBR_SECTOR,response_buf);

	//myinit_args->
	if(!(response_buf[0]==0XEB && response_buf[1]==0X58 && response_buf[2]==0X90)) //���0�����Ƿ�ΪDBR����
	{
		myinit_args->BPB_Sector_No=Bytes2Value(((((MBR_Record *)(response_buf))->Parts[0]).StartLBA),4);
	}
	else
	{
		myinit_args->BPB_Sector_No=0;
	}
	SdReadSector(myinit_args->BPB_Sector_No,response_buf);
	pmy_dbr = (DBR *)response_buf;
	myinit_args->BytesPerSector  =Bytes2Value((pmy_dbr->BPB_BytesPerSec),2);//װ��ÿ�����ֽ�����BytesPerSector��

	myinit_args->FATsectors      =Bytes2Value((pmy_dbr->BPB_FATSz32)    ,4);//װ��FAT��ռ�õ���������FATsectors��

	myinit_args->SectorsPerClust =pmy_dbr->BPB_SecPerClus;//װ��ÿ����������SectorsPerClust ��
	myinit_args->FirstFATSector  =Bytes2Value((pmy_dbr->BPB_RsvdSecCnt) ,2)+myinit_args->BPB_Sector_No;//װ���һ��FAT�������ŵ�FirstFATSector ��
	myinit_args->FirstDirSector  =(myinit_args->FirstFATSector)+(pmy_dbr->BPB_NumFATs)*(myinit_args->FATsectors); //װ���һ��Ŀ¼������FirstDirSector��
	myinit_args->Total_SizeKB    =Bytes2Value((pmy_dbr->BPB_TotSec32),4)/2;  //���̵�����������λ��KB
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
	if(CHK_ATTR_DIR(pFDI->Attributes)) //�����Ŀ¼������.ȥ��
	{
		(pfi->File_Name)[temp+1]='\0';
	}
	//==�����ǰ���LowerCase�ֽڶ����ļ�������չ�ļ�������Сд��

	pfi->File_Attr=pFDI->Attributes; //�ļ�����
	pfi->File_StartClust=Bytes2Value(pFDI->LowClust,2)+Bytes2Value(pFDI->HighClust,2)*65536;
	pfi->File_Size=Bytes2Value(pFDI->FileSize,4);

	//�����ļ�����ʱ��������
	temp=Bytes2Value(pFDI->CTime,2);
	pfi->File_CTime.sec=(u8)((temp&TIME_SEC_MARK)*2); temp>>=TIME_SEC_NBITS;  //����ʱ���2��λ
	pfi->File_CTime.min=(u8)(temp&TIME_MIN_MARK);   temp>>=TIME_MIN_NBITS; //����ʱ��ķ�λ
	pfi->File_CTime.hour=(u8)(temp&TIME_HOUR_MARK); //����ʱ���ʱλ
	pfi->File_CTime.sec+=(u8)((u16)(pFDI->CTime10ms)/100); //�����ϼ���10����λ

	temp=Bytes2Value(pFDI->CDate,2);
	pfi->File_CDate.day=(u8)(temp&DATE_DAY_MARK);     temp>>=DATE_DAY_NBITS;   //�������ڵ���λ
	pfi->File_CDate.month=(u8)(temp&DATE_MONTH_MARK); temp>>=DATE_MONTH_NBITS; //�������ڵ���λ
	pfi->File_CDate.year=(u16)((temp&DATE_YEAR_MARK)+DATE_YEAR_BASE); //�������ڵ���λ��������ݻ�����

	//�����ļ��޸�ʱ��������
	//temp=Bytes2Value(pFDI->MTime,2);
	//pfi->File_MTime.sec=(u8)((temp&TIME_SEC_MARK)*2); temp>>=TIME_SEC_NBITS;  //����ʱ���2��λ
	//pfi->File_MTime.min=(u8)(temp&TIME_MIN_MARK);   temp>>=TIME_MIN_NBITS; //����ʱ��ķ�λ
	//pfi->File_MTime.hour=(u8)(temp&TIME_HOUR_MARK); //����ʱ���ʱλ
	//�ļ����޸�ʱ��û��10����λ��������ֻ�ܱ���ż����

	//temp=Bytes2Value(pFDI->MDate,2);
	//pfi->File_MDate.day=(u8)(temp&DATE_DAY_MARK);     temp>>=DATE_DAY_NBITS;   //�������ڵ���λ
	//pfi->File_MDate.month=(u8)(temp&DATE_MONTH_MARK); temp>>=DATE_MONTH_NBITS; //�������ڵ���λ
	//pfi->File_MDate.year=(u8)((temp&DATE_YEAR_MARK)+DATE_YEAR_BASE); //�������ڵ���λ

	//�����ļ���������
	//temp=Bytes2Value(pFDI->ADate,2);
	//pfi->File_ADate.day=(u8)(temp&DATE_DAY_MARK);     temp>>=DATE_DAY_NBITS;   //�������ڵ���λ
	//pfi->File_ADate.month=(u8)(temp&DATE_MONTH_MARK); temp>>=DATE_MONTH_NBITS; //�������ڵ���λ
	//pfi->File_ADate.year=(u8)((temp&DATE_YEAR_MARK)+DATE_YEAR_BASE); //�������ڵ���λ

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


	clu_sec=(cluster/NITEMSINFATSEC)+(myFAT_Init_Args.FirstFATSector); //ָ���صĴ������ڵ�����Ϊ��FAT���ڵ�ƫ��������
														  
	SdReadSector(clu_sec,response_buf); //���������ڵ��������ݶ��뻺����

	pFAT_Sec=(FAT_Sec *)&response_buf; //�����ݻ������׵�ַǿתΪFAT_Sec�ṹ���ָ������

	pFAT_Item=&((pFAT_Sec->items)[cluster%NITEMSINFATSEC]); //��ȡָ���صĴ��������������еĵ�ַ

	return Bytes2Value((u8 *)pFAT_Item,NFATITEMBYTES); //���ش����ֵ����ָ������һ�صĴغ�
}


/*
thb ԭ��
��ȡ�ļ�����

 u8 File_Name[13];	    //�����ļ��������ļ�������չ�ļ�����
 u8 File_Attr;		    //�ļ�����
 struct Time File_CTime;    //�ļ�����ʱ��
 struct Date File_CDate;    //�ļ���������
 //struct Date File_ADate;    //�ļ���������
 //struct Time File_MTime;	//�ļ��޸�ʱ��
 //struct Date File_MDate;	//�ļ��޸�����
 u32 File_StartClust;    //�ļ���ʼ��
 u32 File_Size;		    //�ļ���С

 u32 File_CurClust;   //�ļ���ǰ��
 u32 File_CurSec;     //�ļ���ǰ����
 u16 File_CurPos;	 //�ļ���ǰ����ƫ����(�����������)

 u8  File_IsEOF;      //�ļ��Ƿ񵽴����λ��

 u32 File_CurOffset;	 //�ļ���ǰƫ����

*/

/*
�����Ƿ��ȡ�� 0 �������� 1��ȡ���
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
	//�����ǰ������ǰ��������������ĩβ˵�����Ը��´���
	if(IS_END_SEC_OF_CLU(fileinfo->File_CurSec-1,fileinfo->File_CurClust))
	{
		
		fileinfo->File_CurClust=Get_Next_Cluster(fileinfo->File_CurClust);
		//if(IS_END_CLU(fileinfo->File_CurClust))
		fileinfo->File_CurSec = SOC(fileinfo->File_CurClust);
	}
	

	return read_len;

}
*/