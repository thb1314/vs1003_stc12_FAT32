#ifndef __COM_H__
#define __COM_H__

/*
 * ���ý����ַ�
 */
extern unsigned char UartRcvChar;

/**
 * ���ڳ�ʼ������
 * return void
 */
extern void UartInit(void);

/**
 * ���ڷ��͵����ַ�
 * @param char Ҫ���͵��ַ���
 * return void
 */
extern void _com_send_char(char);


extern void _com_send_string(unsigned char* );

/**
 * ���ڷ����ַ��� \0������
 * @param char*
 * return void 
 */
extern void UartWrite(char*,unsigned int);

/**
 * ���ڽ����жϺ���
 * return void 
 */
extern void UartRcv(void);

#endif