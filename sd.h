#ifndef __SD_H_
#define __SD_H_
#include "mytype.h"

//=============================================================
//定义SD卡需要的4根信号线
#define SD_CLK  P17
#define SD_DI   P15
#define SD_DO   P16
#define SD_CS   P42

extern u8 xdata response_buf[512];
void SdReadMore(unsigned char *,unsigned int);
void SdWrite(unsigned char);
unsigned char SdRead(void);
void SdCommand(unsigned char, unsigned long, unsigned char);
unsigned char SdReset(void);
unsigned char SdInit(void);

/**
 * 获取CSDstr
 * @param char*
 * return unsigned char 0 失败 1 成功 
 */
//unsigned char SdGetCSD(unsigned char *);
//unsigned long SdCalcSize(unsigned char*);
/**
 * 读取扇区
 * @param unsigned long 扇区号
 * @param unsigned char * 缓冲区
 * return unsigned char 0 失败 1 成功 
 */
unsigned char SdReadSector(unsigned long,unsigned char *);
void SdSPIInit(unsigned char speed);
#endif