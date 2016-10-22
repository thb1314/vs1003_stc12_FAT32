#ifndef __COM_H__
#define __COM_H__

/*
 * 设置接收字符
 */
extern unsigned char UartRcvChar;

/**
 * 串口初始化设置
 * return void
 */
extern void UartInit(void);

/**
 * 串口发送单个字符
 * @param char 要发送的字符串
 * return void
 */
extern void _com_send_char(char);


extern void _com_send_string(unsigned char* );

/**
 * 串口发送字符串 \0结束的
 * @param char*
 * return void 
 */
extern void UartWrite(char*,unsigned int);

/**
 * 串口接收中断函数
 * return void 
 */
extern void UartRcv(void);

#endif