#include "mystr.h"

unsigned char mystrcmp(const unsigned char* str1,const unsigned char* str2,unsigned char len)
{
	unsigned char i;
	for(i=0;i<len;i++)
	{
		if(str1[i] != str2[i])
			return 0;
	}
	return 1;
}