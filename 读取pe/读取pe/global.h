#pragma once
#define uchar  unsigned char
#define uint  unsigned int
#define ushort unsigned short
uint FileToImageBuff(uchar *imageBuff, uchar *fileBuff);//���ļ�buff����Ϊimagebuff
uint ImageToFileBuff(uchar *imageBuff, uchar *fileBuff);//��imagebuff����Ϊ�ļ�buff
void setBuffData(uchar *dst, uchar *src, uint dstbegin, uint srcbegin, uint len);//����һ���ڴ������

unsigned int getIntFromBuff(uchar *buff, uint beginpos);
	
unsigned short getShortFromBuff(uchar *buff, uint beginpos);
uint writeBitCodeToFileBuff_Empty(uchar *code, uint codesize, uchar *buff, uchar *saveBuff);
uint writeBitCodeToFileBuff_addSection(uchar *code, uint codesize, uchar *buff, uchar *saveBuff);