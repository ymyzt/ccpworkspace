#include "pch.h"
#include "global.h"
#include <iostream>
using namespace std;
BData::BData():_data(NULL),_len(0){
}
BData::BData(uchar * data,long len) {
	setData(data, len);
}
BData::~BData(){
	if (_data != NULL) {
		delete[] _data;
	}
}
int BData::setData(uchar * data, long len) {
	if (_data != NULL) {
		delete[] _data;
		_data = NULL;
		_len = 0;
	}
	_data = new uchar[len];
	_len = len;


	if (_data == NULL) {
		_len = 0;
		return 0;
	}
	for (int i = 0; i < len; i++) {
		_data[i] =data[i];
	}
	return 1;
}

uchar * BData::getDataByBuff(uchar * data, long len)
{
	int suc = setData(data, len);
	if (suc) {
		return getData();
	}
	else {
		return NULL;
	}
}

void setBuffData(uchar *dst, uchar *src, uint dstbegin, uint srcbegin, uint len) {
	printf("setBuffData dstbegin:%x\n", dstbegin);
	for (uint i = 0; i < len; i++)
	{
		dst[dstbegin + i] = src[srcbegin + i];
	}
}

struct IMAGE_SECTION_HEADER__ {
	uchar    Name[8];
	union {
		uint   PhysicalAddress;
		uint   VirtualSize;
	} Misc;
	uint   VirtualAddress;
	uint   SizeOfRawData;
	uint   PointerToRawData;
	uint   PointerToRelocations;
	uint   PointerToLinenumbers;
	uchar    NumberOfRelocations;
	uchar    NumberOfLinenumbers;
	uint   Characteristics;
};
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
uint writeBitCodeToFileBuff_Empty(uchar * code,uint codesize ,uchar * fileBuff,uchar *saveBuff)
{
	uint TOPE = getIntFromBuff(fileBuff, 60);	//tope
	//NT头
	uint PEchar = getIntFromBuff(fileBuff, TOPE);
	//标准pe头内容
	ushort NumberOfSections = getShortFromBuff(fileBuff, TOPE + 6);
	cout << "NumberOfSections2: " << NumberOfSections << endl;

	ushort SizeOfOptionalHeader = getShortFromBuff(fileBuff, TOPE + 20); //SizeOfOptionalHeader
	cout << "SizeOfOptionalHeader: " << hex << SizeOfOptionalHeader << endl;
	//可选pe头内容
	uint AddressOfEntryPoint = getIntFromBuff(fileBuff, TOPE + 24 + 0x10);
	printf("AddressOfEntryPoint1 :%x\n", AddressOfEntryPoint);
	uint ImageBase = getIntFromBuff(fileBuff, TOPE + 52);//ImageBase
	ushort SizeOfHeaders = getIntFromBuff(fileBuff, 0x3c + TOPE + 24);
	cout << "SizeOfHeaders: " << hex << SizeOfHeaders << endl;
	for (int i = 0; i < SizeOfHeaders; i++) {//headers无脑靠过去
		saveBuff[i] = fileBuff[i];
	}
	uint beginSections = SizeOfOptionalHeader + 24 + TOPE;
	cout << "beginSections: " << beginSections << "   " << dec << beginSections << endl;
	int off = 0x28;
	bool isfind = false;
	uint AddressOfEntryPoint2 = 0;
	uint PointerToRawData2 = 0;
	uint VirtualSize2 = 0;
	for (int i = 0; i < NumberOfSections; i++)
	{
		uint begin = beginSections + off * (i);
		uint VirtualAddress = getIntFromBuff(fileBuff, begin + 0xc);//在内存中的偏移
		uint PointerToRawData = getIntFromBuff(fileBuff, begin + 0x14);//该节在文件中的偏移
		uint SizeOfRawData = getIntFromBuff(fileBuff, begin + 16);//该节在文件中对齐后的大小
		uint VirtualSize = getIntFromBuff(fileBuff, begin + 8);
		cout << "i: " << i << endl;
		printf("begin2:%x\n", begin);
		printf("VirtualAddress2:%x\n", VirtualAddress);
		printf("PointerToRawData2:%x\n", PointerToRawData);
		printf("SizeOfRawData2:%x\n", SizeOfRawData);
		printf("VirtualSize:%x\n", VirtualSize);
		if ((int)(SizeOfRawData - VirtualSize) >= 18&&!isfind) {
			isfind = true;
			PointerToRawData2 = PointerToRawData;
			VirtualSize2 = VirtualSize;
			AddressOfEntryPoint2 = VirtualAddress +  VirtualSize;
			printf("AddressOfEntryPoint2 :%x\n", AddressOfEntryPoint2);
			setBuffData(saveBuff, (uchar *)&AddressOfEntryPoint2, TOPE + 24 + 0x10, 0, 4);
			uint dstaddre = 0x75481060 - (AddressOfEntryPoint2 + 13+ ImageBase);
			uint srctaddre = AddressOfEntryPoint - (AddressOfEntryPoint2 + 13+5);
			printf("dstaddre :%x\n", dstaddre);
			printf("srctaddre :%x\n", srctaddre);
			setBuffData(code, (uchar *)&dstaddre, 9, 0, 4);
			setBuffData(code, (uchar *)&srctaddre, 14, 0, 4);
		}
		setBuffData(saveBuff, fileBuff, PointerToRawData, PointerToRawData, SizeOfRawData);
	}
	if (!isfind)
		cout << "未找到合适位置写入" << endl;
	else
		setBuffData(saveBuff, code, VirtualSize2+ PointerToRawData2, 0, codesize);

	printf("AddressOfEntryPoint3 :%x\n", AddressOfEntryPoint);
	return 1;
}
uint writeBitCodeToFileBuff_addSection(uchar * code, uint codesize, uchar * fileBuff, uchar * saveBuff)
{
	uint TOPE = getIntFromBuff(fileBuff, 60);	//tope
		//NT头
	uint PEchar = getIntFromBuff(fileBuff, TOPE);
	//标准pe头内容
	ushort NumberOfSections = getShortFromBuff(fileBuff, TOPE + 6);
	cout << "NumberOfSections2: " << NumberOfSections << endl;
	
	ushort SizeOfOptionalHeader = getShortFromBuff(fileBuff, TOPE + 20); //SizeOfOptionalHeader
	cout << "SizeOfOptionalHeader: " << hex << SizeOfOptionalHeader << endl;
	//可选pe头内容
	uint AddressOfEntryPoint = getIntFromBuff(fileBuff, TOPE + 24 + 0x10);
	uint FileAlignment = getIntFromBuff(fileBuff, TOPE + 24 + 0x24);
	printf("AddressOfEntryPoint1 :%x\n", AddressOfEntryPoint);
	uint ImageBase = getIntFromBuff(fileBuff, TOPE + 52);//ImageBase
	uint SizeOfImage = getIntFromBuff(fileBuff, TOPE + 24+0x38);
	ushort SizeOfHeaders = getIntFromBuff(fileBuff, 0x3c + TOPE + 24);
	cout << "SizeOfHeaders: " << hex << SizeOfHeaders << endl;
	uint len = TOPE + 24 + SizeOfOptionalHeader + 40 * NumberOfSections;
	printf("len :%x\n", len);
	if (SizeOfHeaders- len > 80) {
		for (int i = 0; i < SizeOfHeaders; i++) {//headers无脑靠过去
			saveBuff[i] = fileBuff[i];
		}
		IMAGE_SECTION_HEADER__ section;
		memset(&section, 0, sizeof(section));
		
		uint beginSections = SizeOfOptionalHeader + 24 + TOPE;
		cout << "beginSections: " << beginSections << "   " << dec << beginSections << endl;
		int off = 0x28;
		uint AddressOfEntryPoint2 = 0;
		uint PointerToRawData2 = 0;
		uint VirtualSize2 = 0;
		uint VirtualAddress2 = 0;
		uint SizeOfRawData2 = 0;
		for (int i = 0; i < NumberOfSections; i++)
		{
			uint begin = beginSections + off * (i);
			uint VirtualAddress = getIntFromBuff(fileBuff, begin + 0xc);//在内存中的偏移
			uint PointerToRawData = getIntFromBuff(fileBuff, begin + 0x14);//该节在文件中的偏移
			uint SizeOfRawData = getIntFromBuff(fileBuff, begin + 16);//该节在文件中对齐后的大小
			uint VirtualSize = getIntFromBuff(fileBuff, begin + 8);
			cout << "i: " << i << endl;
			printf("begin2:%x\n", begin);
			printf("VirtualAddress2:%x\n", VirtualAddress);
			printf("PointerToRawData2:%x\n", PointerToRawData);
			printf("SizeOfRawData2:%x\n", SizeOfRawData);
			printf("VirtualSize:%x\n", VirtualSize);
			setBuffData(saveBuff, fileBuff, PointerToRawData, PointerToRawData, SizeOfRawData);
			if (i == NumberOfSections - 1) {
				VirtualAddress2 = VirtualAddress;
				VirtualSize2 = VirtualSize;
				PointerToRawData2 = PointerToRawData;
				SizeOfRawData2 = SizeOfRawData;
			}
			
		}
		section.Name[0] = '.';
		section.Name[1] = '1';
		section.Misc.VirtualSize = FileAlignment;
		section.VirtualAddress = 0x12000;
		section.SizeOfRawData = FileAlignment;
		section.PointerToRawData = PointerToRawData2 + SizeOfRawData2;
		section.Characteristics = 0x20000000 | 0x40000000 | 0x00000020;
		printf("section.VirtualAddress :%x\n", section.VirtualAddress);
		printf("section.SizeOfRawData :%x\n", section.SizeOfRawData);
		printf("section.PointerToRawData :%x\n", section.PointerToRawData);
		printf("section.Characteristics :%x\n", section.Characteristics);
		setBuffData(saveBuff, (uchar *)&section.VirtualAddress, TOPE + 24 + 0x10, 0, 4);
		uint dstaddre = 0x75871060 - (section.VirtualAddress + 13 + ImageBase);
		uint srctaddre = AddressOfEntryPoint - (section.VirtualAddress + 13 + 5);
		printf("dstaddre :%x\n", dstaddre);
		printf("srctaddre :%x\n", srctaddre);
		setBuffData(code, (uchar *)&dstaddre, 9, 0, 4);
		setBuffData(code, (uchar *)&srctaddre, 14, 0, 4);
		setBuffData(saveBuff, code, PointerToRawData2 + SizeOfRawData2, 0, codesize);
		setBuffData(saveBuff,(uchar *) &section,len, 0, 40);
		ushort NumberOfSections2 = NumberOfSections + 1;
		setBuffData(saveBuff, (uchar *)&NumberOfSections2, TOPE + 6, 0, 2);//节加1
		uint SizeOfImage2 = 0x13000;
		setBuffData(saveBuff, (uchar *)&SizeOfImage2, TOPE + 24 + 0x38, 0, 4);//内存变大
	
		
	}
	else {
		cout << "空间不足不能添加节" << endl;
	}
	return 1;
	
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
		
		/*printf("begin2:%x\n", begin);
		printf("VirtualAddress2:%x\n", VirtualAddress);
		printf("PointerToRawData2:%x\n", PointerToRawData);
		printf("SizeOfRawData2:%x\n", SizeOfRawData);
		printf("VirtualSize:%x\n", VirtualSize);*/

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