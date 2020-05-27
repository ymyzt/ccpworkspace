// 读取pe.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "global.h"
using namespace std;
#define filePath   "pixie.exe"
int main()
{
	FILE *f = fopen(filePath, "rb");
	if (f == NULL) {
		cout << "file open fail!!" << endl;
		return 0;
	}
	fseek(f, 0, SEEK_END);
	long  len = ftell(f);
	uchar * buff = new uchar[len];
	memset(buff, 0, len);
	fseek(f, 0, SEEK_SET);
	int res = fread(buff, len, 1, f);
	fclose(f);
	

	//Dos头40h
	ushort MZ = getShortFromBuff(buff, 0);//*(short *)shortchar;//MZ
	uint TOPE = getIntFromBuff(buff,60);	//tope
	//NT头
	uint PEchar = getIntFromBuff(buff, TOPE);
			//标准pe头TOPE+4~TOPE+23  20个字节
	ushort Machine = getShortFromBuff(buff, TOPE + 4);//Machine
	ushort NumberOfSections = getShortFromBuff(buff, TOPE + 6);//NumberOfSections
	ushort SizeOfOptionalHeader = getShortFromBuff(buff, TOPE + 20); //SizeOfOptionalHeader
			//可选pe头 TOPE+24~ 32位一般e0个字节
	uint AddressOfEntryPoint = getIntFromBuff(buff, TOPE + 40);//AddressOfEntryPoint
	uint ImageBase = getIntFromBuff(buff, TOPE + 52);//ImageBase
	uint SizeOfImage = getIntFromBuff(buff, 0x38 + TOPE + 24);


	cout << "MZ: " << hex << MZ << endl;
	cout << "TOPE: " << hex << TOPE << endl;
	cout << "PEchar: " << hex << PEchar << endl;
	cout << "Machine: " << Machine << endl;
	cout << "NumberOfSections: " << NumberOfSections << endl;
	cout << "SizeOfOptionalHeader: " << dec << SizeOfOptionalHeader << endl;
	cout << "AddressOfEntryPoint: " << hex << AddressOfEntryPoint << endl;
	cout << "ImageBase: " << hex << ImageBase << endl;
	cout << "SizeOfImage: " << dec << SizeOfImage << "  " << hex << SizeOfImage << endl;
	
	
	
	uchar *imageBuff = new uchar[SizeOfImage];
	memset(imageBuff, 0, SizeOfImage);
	FileToImageBuff(buff,imageBuff);
	FILE *ff = fopen("notepad1.exe", "wb");
	int res2 = fwrite(imageBuff, SizeOfImage, 1, ff);
	fclose(ff);


	std::cout << "Hello World!"<<endl;



	uchar *fileBuff = new uchar[len];
	memset(fileBuff, 0, len);
	ImageToFileBuff(imageBuff, fileBuff);
	FILE *fff = fopen("notepad2.exe", "wb");
	int res3 = fwrite(fileBuff, len, 1, ff);
	fclose(fff);
	





}



