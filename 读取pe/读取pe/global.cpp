#include "pch.h"
#include "global.h"
#include <iostream>
using namespace std;
uint getIntFromBuff(uchar *buff, uint beginpos) {
	uint res = 0;
	for (int i = 0; i < 4; i++) {
		res = res |buff[beginpos + 3 - i];
		if (i != 3) res = res << 8;
	}
	return res;
}
ushort getShortFromBuff(uchar *buff, uint beginpos) {
	ushort res = 0;
	for (int i = 0; i < 2; i++) {
		res = res | buff[beginpos + 1 - i];
		if (i != 1) res = res << 8;
	}
	return res;
}
void setBuffData(uchar *dst, uchar *src, uint dstbegin,uint srcbegin,uint len){
	for (uint i = 0;i<len;i++)
	{
		dst[dstbegin + i] = src[srcbegin + i];
	}
}
uint FileToImageBuff(uchar *fileBuff,uchar *imageBuff ) {
	
	uint TOPE = getIntFromBuff(fileBuff, 60);	//tope
	//NT头
	uint PEchar = getIntFromBuff(fileBuff, TOPE);
	//标准pe头内容
	ushort NumberOfSections = getShortFromBuff(fileBuff, TOPE +6);
	cout << "NumberOfSections2: " << NumberOfSections << endl;
	
	ushort SizeOfOptionalHeader = getShortFromBuff(fileBuff, TOPE + 20); //SizeOfOptionalHeader
	cout << "SizeOfOptionalHeader: " << hex << SizeOfOptionalHeader << endl;
	 //可选pe头内容
	uint ImageBase = getIntFromBuff(fileBuff, TOPE + 52);//ImageBase
	ushort SizeOfHeaders = getIntFromBuff(fileBuff, 0x3c + TOPE + 24);
	cout << "SizeOfHeaders: "<< hex <<SizeOfHeaders << endl;
	for (int i = 0; i < SizeOfHeaders; i++) {//headers无脑靠过去
		imageBuff[i] = fileBuff[i];
	}
	uint beginSections = SizeOfOptionalHeader + 24+TOPE;
	cout << "beginSections: " << beginSections <<"   "<< dec<< beginSections <<endl;
	int off = 0x28;
	for (int i = 0;i< NumberOfSections;i++)
	{
		uint begin = beginSections + off * (i);
		uint VirtualAddress = getIntFromBuff(fileBuff, begin + 0xc);//在内存中的偏移
		uint PointerToRawData = getIntFromBuff(fileBuff, begin + 0x14);//该节在文件中的偏移
		uint SizeOfRawData = getIntFromBuff(fileBuff, begin +16);//该节在文件中对齐后的大小
		uint VirtualSize = getIntFromBuff(fileBuff, begin + 8);
		cout << "i: " << i << endl;
		/*cout << "begin: " << begin << endl;
		cout << "VirtualAddress: " << VirtualAddress << endl;
		cout << "PointerToRawData: " << PointerToRawData << endl;
		cout << "SizeOfRawData: " <<hex<< SizeOfRawData << endl;*/

		printf("begin2:%x\n", begin);
		printf("VirtualAddress2:%x\n", VirtualAddress);
		printf("PointerToRawData2:%x\n", PointerToRawData);
		printf("SizeOfRawData2:%x\n", SizeOfRawData);
		printf("VirtualSize:%x\n", VirtualSize);

		setBuffData(imageBuff, fileBuff, VirtualAddress, PointerToRawData, SizeOfRawData);
	}
	return 1;
}
uint ImageToFileBuff(uchar *imageBuff, uchar *fileBuff) {

	uint TOPE = getIntFromBuff(imageBuff, 60);	//tope
	//NT头
	uint PEchar = getIntFromBuff(imageBuff, TOPE);
	//标准pe头内容
	ushort NumberOfSections = getShortFromBuff(imageBuff, TOPE + 6);
	cout << "NumberOfSections2: " << NumberOfSections << endl;

	ushort SizeOfOptionalHeader = getShortFromBuff(imageBuff, TOPE + 20); //SizeOfOptionalHeader
	cout << "SizeOfOptionalHeader: " << hex << SizeOfOptionalHeader << endl;
	//可选pe头内容
	uint ImageBase = getIntFromBuff(imageBuff, TOPE + 52);//ImageBase
	ushort SizeOfHeaders = getIntFromBuff(imageBuff, 0x3c + TOPE + 24);
	cout << "SizeOfHeaders: " << hex << SizeOfHeaders << endl;
	for (int i = 0; i < SizeOfHeaders; i++) {//headers无脑靠过去
		fileBuff[i] = imageBuff[i];
	}
	uint beginSections = SizeOfOptionalHeader + 24 + TOPE;
	cout << "beginSections: " << beginSections << "   " << dec << beginSections << endl;
	int off = 0x28;
	for (int i = 0; i < NumberOfSections; i++)
	{
		uint begin = beginSections + off * (i);
		uint VirtualAddress = getIntFromBuff(imageBuff, begin + 0xc);//在内存中的偏移
		uint PointerToRawData = getIntFromBuff(imageBuff, begin + 0x14);//该节在文件中的偏移
		uint SizeOfRawData = getIntFromBuff(imageBuff, begin + 16);//该节在文件中对齐后的大小
		uint VirtualSize = getIntFromBuff(imageBuff, begin + 8);
		cout << "i: " << i << endl;
		printf("begin2:%x\n", begin);
		printf("VirtualAddress2:%x\n", VirtualAddress);
		printf("PointerToRawData2:%x\n", PointerToRawData);
		printf("SizeOfRawData2:%x\n", SizeOfRawData);
		printf("VirtualSize:%x\n", VirtualSize);

		setBuffData(fileBuff, imageBuff, PointerToRawData,VirtualAddress , SizeOfRawData);
	}
	return 1;
}