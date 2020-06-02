#pragma once
#define uchar  unsigned char
#define uint  unsigned int
#define ushort unsigned short
uint FileToImageBuff(uchar *imageBuff, uchar *fileBuff);//将文件buff保存为imagebuff
uint ImageToFileBuff(uchar *imageBuff, uchar *fileBuff);//将imagebuff保存为文件buff
void setBuffData(uchar *dst, uchar *src, uint dstbegin, uint srcbegin, uint len);//设置一段内存的内容

unsigned int getIntFromBuff(uchar *buff, uint beginpos);
	
unsigned short getShortFromBuff(uchar *buff, uint beginpos);
uint writeBitCodeToFileBuff_Empty(uchar *code, uint codesize, uchar *buff, uchar *saveBuff);
uint writeBitCodeToFileBuff_addSection(uchar *code, uint codesize, uchar *buff, uchar *saveBuff);