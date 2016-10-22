#ifndef __FAT32_H_
#define __FAT32_H_
#include "mytype.h"


#define NFDI_PER_SEC (16)
#define MBR_SECTOR (0) //MBR扇区


#define NFATITEMBYTES   (4)    //FAT表项所占用的字节数
#define NITEMSINFATSEC  (128)  //FAT表一个扇区中包含的表项数

#define SOC(c)  (((c-2)*(myFAT_Init_Args.SectorsPerClust))+(myFAT_Init_Args.FirstDirSector)) //计算簇的开始扇区 Start Sector Of Cluster
#define IS_END_CLU(cluster) ((0X0FFFFFFF)==(cluster)) //判断一个簇项的值是否是结束簇
#define IS_END_SEC_OF_CLU(sec,cluster) ((sec-SOC(cluster))==(myFAT_Init_Args.SectorsPerClust-1)) //判断是否是簇的最后一个扇区
#define LAST_SEC_OF_CLU(cluster) (SOC(cluster)+(myFAT_Init_Args.SectorsPerClust-1)) //簇的最后一个扇区的地址
#define IS_FAT32_TYPE(FST) (('F'==(FST[0])) && ('A'==(FST[1])) && ('T'==(FST[2])) && ('3'==(FST[3])) && ('2'==(FST[4]))) //检验文件系统是否FAT32 


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

#define CHK_ATTR_FILE(attr) ((!((attr&0X10)!=0X00)) && (0X0F!=attr) && (0X00!=attr) && (0X08!=attr)) //属性字节第4位为0，同时不是长名属性0X0F，空项或卷标 
#define CHK_ATTR_DIR(attr) ((attr&0X10)!=0X00) //属性字节第4位为1，则视其为目录
#define CHK_ATTR_LFN(attr) (0X0F==attr) //属性字节为0X0F，则为长名项


// MBR的4个分区
typedef struct MyPart_Record
{
	u8 Active;	//引导指示符
	u8 StartHead;	//分区的开始磁头
	u8 StartCylSect[2];	//开始柱面与扇区
	u8 PartType;	//分区类型
	u8 EndHead;		//分区的结束头
	u8 EndtCylSect[2];	//结束柱面与扇区
	u8 StartLBA[4];	//分区的第一个扇区
	u8 TotalSector[4];	//分区的总扇区数
	
} Part_Record;

//MBR_Record 的记录
typedef struct MyMBR_Record
{
	u8 PartCode[446];	//MBR的引导程序
	Part_Record Parts[4];	//4个分区记录
	u8 BootSectSig0;	// 55
	u8 BootSectSig1;	// AA
	
} MBR_Record;



u32 Bytes2Value(u8 *dat,u8 len);

// DBR扇区数据结构
//znFAT中对DBR的定义如下  一共占用90个字节
typedef struct myDBR
{
 u8 BS_jmpBoot[3];     //跳转指令            offset: 0
 u8 BS_OEMName[8];     //OEM名称             offset: 3

 u8 BPB_BytesPerSec[2];//每扇区字节数        offset:11
 u8 BPB_SecPerClus;    //每簇扇区数          offset:13
 u8 BPB_RsvdSecCnt[2]; //保留扇区数目        offset:14
 u8 BPB_NumFATs;       //此卷中FAT表数       offset:16
 u8 BPB_RootEntCnt[2]; //FAT32为0            offset:17
 u8 BPB_TotSec16[2];   //FAT32为0            offset:19
 u8 BPB_Media;         //存储介质            offset:21
 u8 BPB_FATSz16[2];    //FAT32为0            offset:22
 u8 BPB_SecPerTrk[2];  //磁道扇区数          offset:24
 u8 BPB_NumHeads[2];   //磁头数              offset:26
 u8 BPB_HiddSec[4];    //FAT区前隐扇区数     offset:28
 u8 BPB_TotSec32[4];   //该卷总扇区数        offset:32
 u8 BPB_FATSz32[4];    //一个FAT表扇区数     offset:36
 u8 BPB_ExtFlags[2];   //FAT32特有           offset:40
 u8 BPB_FSVer[2];      //FAT32特有           offset:42
 u8 BPB_RootClus[4];   //根目录簇号          offset:44
 u8 FSInfo[2];         //保留扇区FSINFO扇区数offset:48
 u8 BPB_BkBootSec[2];  //通常为6             offset:50
 u8 BPB_Reserved[12];  //扩展用              offset:52
 u8 BS_DrvNum;         //                    offset:64
 u8 BS_Reserved1;      //                    offset:65
 u8 BS_BootSig;        //                    offset:66
 u8 BS_VolID[4];       //                    offset:67
 u8 BS_FilSysType[11]; //	                offset:71
 u8 BS_FilSysType1[8]; //"FAT32    "         offset:82
} DBR;


//FAT初始化时初始参数装入如下结构体中
typedef struct myFAT_Init_Args
{
 u32 BPB_Sector_No;   //DBR(BPB)所在扇区号
 u32 Total_SizeKB;    //磁盘的总容量，单位为KB
 u32 BytesPerSector;	 //每个扇区的字节数
 u32 FATsectors;      //FAT表所占扇区数
 u32 SectorsPerClust; //每簇的扇区数
 u32 FirstFATSector;	 //第一个FAT表所在扇区
 u32 FirstDirSector;	 //第一个目录所在扇区
#ifdef FSINFO
 u32 FSINFO_Sec;      //FSINFO扇区所在的扇区
 u32 Free_nCluster;   //空闲簇的个数
 u32 Next_Free_Cluster; //下一空簇
#endif
} FAT_Init_Args;

extern FAT_Init_Args xdata myFAT_Init_Args;
//返回值 0 FALSE or 1 TRUE
u8 FAT32_Init(FAT_Init_Args*);


struct Date	 //用于存储日期信息
{
 u16 year;
 u8 month;
 u8 day;
};

struct Time	 //用于存储时间信息
{
 u8 hour;
 u8 min;
 u8 sec;
};

struct DateTime //日期与时间
{
 struct Date date; //日期
 struct Time time; //时间
};

typedef struct myFileInfo
{
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
 u16 File_CurPos;	 //文件当前扇区偏移量

 u8  File_IsEOF;      //文件是否到达结束位置

 u32 File_CurOffset;	 //文件当前偏移量

} FileInfo;

//znFAT中对文件目录项(振南叫它FDI)的定义如下  一共占用32个字节
typedef struct myFDI
{
 u8 Name[8];         // 文件名，不足部分以空格补充
 u8 Extension[3]; 	// 扩展名，不足部分以空格补充
 u8 Attributes;   	// 文件属性
 u8 LowerCase;    	// 0
 u8 CTime10ms;   	// 创建时间的10毫秒位
 u8 CTime[2];     	// 创建时间
 u8 CDate[2];     	// 创建日期
 u8 ADate[2];     	// 访问日期
 u8 HighClust[2];    // 开始簇的高字
 u8 MTime[2];     	// 最近的修改时间
 u8 MDate[2];     	// 最近的修改日期
 u8 LowClust[2]; 	// 开始簇的低字
 u8 FileSize[4];     // 文件大小
} FDI;

typedef struct myFDIesInSEC
{
  FDI FDIes[NFDI_PER_SEC]; //扇区中的文件目录项数组
} FDIesInSEC;
extern FDIesInSEC *myfdis;

u8 Analyse_FDI(FileInfo *pfi,FDI *pFDI);
extern FileInfo data fat32_fi;


//FAT表
typedef struct myFAT_Item  //znFAT中对FAT表项的结构定义
{
	u8 Item[NFATITEMBYTES]; //FAT32中FAT表项占用4个字节，即32位
} FAT_Item;
	
typedef struct myFAT_Sec	//znFAT中对FAT表扇区结构的定义
{
  FAT_Item items[NITEMSINFATSEC]; //FAT扇区包含128个FAT表项	                                //FAT扇区的结构就是有128个FAT表项类型的数组
} FAT_Sec;

u32 Get_Next_Cluster(u32 cluster);
u16 FAT_Read_File(FileInfo *fileinfo,u8 *buffer);
#endif
