#ifndef __FAT32_H_
#define __FAT32_H_
#include "mytype.h"


#define NFDI_PER_SEC (16)
#define MBR_SECTOR (0) //MBR����


#define NFATITEMBYTES   (4)    //FAT������ռ�õ��ֽ���
#define NITEMSINFATSEC  (128)  //FAT��һ�������а����ı�����

#define SOC(c)  (((c-2)*(myFAT_Init_Args.SectorsPerClust))+(myFAT_Init_Args.FirstDirSector)) //����صĿ�ʼ���� Start Sector Of Cluster
#define IS_END_CLU(cluster) ((0X0FFFFFFF)==(cluster)) //�ж�һ�������ֵ�Ƿ��ǽ�����
#define IS_END_SEC_OF_CLU(sec,cluster) ((sec-SOC(cluster))==(myFAT_Init_Args.SectorsPerClust-1)) //�ж��Ƿ��Ǵص����һ������
#define LAST_SEC_OF_CLU(cluster) (SOC(cluster)+(myFAT_Init_Args.SectorsPerClust-1)) //�ص����һ�������ĵ�ַ
#define IS_FAT32_TYPE(FST) (('F'==(FST[0])) && ('A'==(FST[1])) && ('T'==(FST[2])) && ('3'==(FST[3])) && ('2'==(FST[4]))) //�����ļ�ϵͳ�Ƿ�FAT32 


#define Lower2Up(c) ((c>='a' && c<='z')?(c-32):c)
#define Upper2Low(C) ((C>='A' && C<='Z')?(C+32):C)
#define WLower2Up(c) ((c>=(UINT16)'a' && c<=(UINT16)'z')?(c-32):c)
#define WUpper2Low(C) ((C>=(UINT16)'A' && C<=(UINT16)'Z')?(C+32):C)
#define IS_ASC(c) ((c&0X80)==0)



#define DATE_YEAR_BASE (1980)

#define TIME_HOUR_MARK  (0X001F)
#define TIME_MIN_MARK   (0X002F)
#define TIME_SEC_MARK   (0X001F)

#define DATE_YEAR_MARK  (0X007F)
#define DATE_MONTH_MARK (0X000F)
#define DATE_DAY_MARK   (0X001F) 

#define TIME_HOUR_NBITS   (5)
#define TIME_MIN_NBITS    (6)
#define TIME_SEC_NBITS    (5)

#define DATE_YEAR_NBITS   (7)
#define DATE_MONTH_NBITS  (4)
#define DATE_DAY_NBITS    (5)

#define CHK_ATTR_FILE(attr) ((!((attr&0X10)!=0X00)) && (0X0F!=attr) && (0X00!=attr) && (0X08!=attr)) //�����ֽڵ�4λΪ0��ͬʱ���ǳ�������0X0F���������� 
#define CHK_ATTR_DIR(attr) ((attr&0X10)!=0X00) //�����ֽڵ�4λΪ1��������ΪĿ¼
#define CHK_ATTR_LFN(attr) (0X0F==attr) //�����ֽ�Ϊ0X0F����Ϊ������


// MBR��4������
typedef struct MyPart_Record
{
	u8 Active;	//����ָʾ��
	u8 StartHead;	//�����Ŀ�ʼ��ͷ
	u8 StartCylSect[2];	//��ʼ����������
	u8 PartType;	//��������
	u8 EndHead;		//�����Ľ���ͷ
	u8 EndtCylSect[2];	//��������������
	u8 StartLBA[4];	//�����ĵ�һ������
	u8 TotalSector[4];	//��������������
	
} Part_Record;

//MBR_Record �ļ�¼
typedef struct MyMBR_Record
{
	u8 PartCode[446];	//MBR����������
	Part_Record Parts[4];	//4��������¼
	u8 BootSectSig0;	// 55
	u8 BootSectSig1;	// AA
	
} MBR_Record;



u32 Bytes2Value(u8 *dat,u8 len);

// DBR�������ݽṹ
//znFAT�ж�DBR�Ķ�������  һ��ռ��90���ֽ�
typedef struct myDBR
{
 u8 BS_jmpBoot[3];     //��תָ��            offset: 0
 u8 BS_OEMName[8];     //OEM����             offset: 3

 u8 BPB_BytesPerSec[2];//ÿ�����ֽ���        offset:11
 u8 BPB_SecPerClus;    //ÿ��������          offset:13
 u8 BPB_RsvdSecCnt[2]; //����������Ŀ        offset:14
 u8 BPB_NumFATs;       //�˾���FAT����       offset:16
 u8 BPB_RootEntCnt[2]; //FAT32Ϊ0            offset:17
 u8 BPB_TotSec16[2];   //FAT32Ϊ0            offset:19
 u8 BPB_Media;         //�洢����            offset:21
 u8 BPB_FATSz16[2];    //FAT32Ϊ0            offset:22
 u8 BPB_SecPerTrk[2];  //�ŵ�������          offset:24
 u8 BPB_NumHeads[2];   //��ͷ��              offset:26
 u8 BPB_HiddSec[4];    //FAT��ǰ��������     offset:28
 u8 BPB_TotSec32[4];   //�þ���������        offset:32
 u8 BPB_FATSz32[4];    //һ��FAT��������     offset:36
 u8 BPB_ExtFlags[2];   //FAT32����           offset:40
 u8 BPB_FSVer[2];      //FAT32����           offset:42
 u8 BPB_RootClus[4];   //��Ŀ¼�غ�          offset:44
 u8 FSInfo[2];         //��������FSINFO������offset:48
 u8 BPB_BkBootSec[2];  //ͨ��Ϊ6             offset:50
 u8 BPB_Reserved[12];  //��չ��              offset:52
 u8 BS_DrvNum;         //                    offset:64
 u8 BS_Reserved1;      //                    offset:65
 u8 BS_BootSig;        //                    offset:66
 u8 BS_VolID[4];       //                    offset:67
 u8 BS_FilSysType[11]; //	                offset:71
 u8 BS_FilSysType1[8]; //"FAT32    "         offset:82
} DBR;


//FAT��ʼ��ʱ��ʼ����װ�����½ṹ����
typedef struct myFAT_Init_Args
{
 u32 BPB_Sector_No;   //DBR(BPB)����������
 u32 Total_SizeKB;    //���̵�����������λΪKB
 u32 BytesPerSector;	 //ÿ���������ֽ���
 u32 FATsectors;      //FAT����ռ������
 u32 SectorsPerClust; //ÿ�ص�������
 u32 FirstFATSector;	 //��һ��FAT����������
 u32 FirstDirSector;	 //��һ��Ŀ¼��������
#ifdef FSINFO
 u32 FSINFO_Sec;      //FSINFO�������ڵ�����
 u32 Free_nCluster;   //���дصĸ���
 u32 Next_Free_Cluster; //��һ�մ�
#endif
} FAT_Init_Args;

extern FAT_Init_Args xdata myFAT_Init_Args;
//����ֵ 0 FALSE or 1 TRUE
u8 FAT32_Init(FAT_Init_Args*);


struct Date	 //���ڴ洢������Ϣ
{
 u16 year;
 u8 month;
 u8 day;
};

struct Time	 //���ڴ洢ʱ����Ϣ
{
 u8 hour;
 u8 min;
 u8 sec;
};

struct DateTime //������ʱ��
{
 struct Date date; //����
 struct Time time; //ʱ��
};

typedef struct myFileInfo
{
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
 u16 File_CurPos;	 //�ļ���ǰ����ƫ����

 u8  File_IsEOF;      //�ļ��Ƿ񵽴����λ��

 u32 File_CurOffset;	 //�ļ���ǰƫ����

} FileInfo;

//znFAT�ж��ļ�Ŀ¼��(���Ͻ���FDI)�Ķ�������  һ��ռ��32���ֽ�
typedef struct myFDI
{
 u8 Name[8];         // �ļ��������㲿���Կո񲹳�
 u8 Extension[3]; 	// ��չ�������㲿���Կո񲹳�
 u8 Attributes;   	// �ļ�����
 u8 LowerCase;    	// 0
 u8 CTime10ms;   	// ����ʱ���10����λ
 u8 CTime[2];     	// ����ʱ��
 u8 CDate[2];     	// ��������
 u8 ADate[2];     	// ��������
 u8 HighClust[2];    // ��ʼ�صĸ���
 u8 MTime[2];     	// ������޸�ʱ��
 u8 MDate[2];     	// ������޸�����
 u8 LowClust[2]; 	// ��ʼ�صĵ���
 u8 FileSize[4];     // �ļ���С
} FDI;

typedef struct myFDIesInSEC
{
  FDI FDIes[NFDI_PER_SEC]; //�����е��ļ�Ŀ¼������
} FDIesInSEC;
extern FDIesInSEC *myfdis;

u8 Analyse_FDI(FileInfo *pfi,FDI *pFDI);
extern FileInfo data fat32_fi;


//FAT��
typedef struct myFAT_Item  //znFAT�ж�FAT����Ľṹ����
{
	u8 Item[NFATITEMBYTES]; //FAT32��FAT����ռ��4���ֽڣ���32λ
} FAT_Item;
	
typedef struct myFAT_Sec	//znFAT�ж�FAT�������ṹ�Ķ���
{
  FAT_Item items[NITEMSINFATSEC]; //FAT��������128��FAT����	                                //FAT�����Ľṹ������128��FAT�������͵�����
} FAT_Sec;

u32 Get_Next_Cluster(u32 cluster);
u16 FAT_Read_File(FileInfo *fileinfo,u8 *buffer);
#endif