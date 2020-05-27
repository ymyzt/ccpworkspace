#pragma once
#define uchar  unsigned char
#define uint  unsigned int
#define ushort unsigned short
uint FileToImageBuff(uchar *imageBuff, uchar *fileBuff);
uint ImageToFileBuff(uchar *imageBuff, uchar *fileBuff);
void setBuffData(uchar *dst, uchar *src, uint dstbegin, uint srcbegin, uint len);

unsigned int getIntFromBuff(uchar *buff, uint beginpos);
	
unsigned short getShortFromBuff(uchar *buff, uint beginpos);