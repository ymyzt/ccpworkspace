#include "pch.h"
#include <iostream>
#include "PEFile.h"
#include <fstream>

using namespace std;
PEFile::PEFile(const char *path)
{
	readFile(path);
	analyseFile();
}
PEFile::~PEFile()
{
}
void PEFile::printfHeadInfo()
{
	printf("NtHeadAddre: %x\n", NtHeadAddre);
	printf("PeFileHeadAddre: %x\n", NtHead.PeFileHeadAddre);
	printf("NumberOfSections: %x\n", NumberOfSections);
	printf("SizeOfOptionalHeader: %x\n", SizeOfOptionalHeader);


	printf("PeOptionHeadAddre: %x\n", NtHead.PeOptionHeadAddre);
	printf("SizeOfHeaders: %x\n", SizeOfHeaders);
	printf("SectionHeadAddre: %x\n", SectionHeadAddre);
	
}
int PEFile::readFile(const char *path) {
	ifstream  file;
	file.open(path, ios::binary | ios::in);
	if (!file.is_open()) {
		cout << "��ʧ��!" << endl;
		return 0;
	}
	file.seekg(0, ios::end);
	fileSize = file.tellg();
	file.seekg(0, ios::beg);
	//fileBuff << file;
	if (fileSize <= 0) {
		cout << "�ļ����ȴ���!" << endl;
		return 0;
	}
	fileBuff = new uchar[fileSize];
	if (fileBuff == NULL) {
		cout << "����ռ�ʧ��!" << endl;
		return 0;
	}
	file >> fileBuff;
	return 1;
}
int PEFile::analyseFile() {
	BData bdata;
	//Dosͷ
	
	ushort MZStr =*(ushort *)bdata.getDataByBuff(fileBuff, 2);//getIntFromBuff(fileBuff, 60);	//tope
	if (MZStr != 0x5a4d) {
		cout << "����MZ��ͷ" << endl;
		return 0;
	}
	 NtHeadAddre = *(uint*)bdata.getDataByBuff(fileBuff, 0x36);
	//NTͷ
	NtHead.NtStr = *(uint*)bdata.getDataByBuff(fileBuff, NtHeadAddre);
	if (NtHead.NtStr != 0x4550) {
		cout << "����NT��ͷ" << endl;
		return 0;
	}
	//��׼peͷ20���ֽ�
	NtHead.PeFileHeadAddre = *(uint*)bdata.getDataByBuff(fileBuff, NtHeadAddre+0x4);
	NumberOfSections = *(uint*)bdata.getDataByBuff(fileBuff, NtHead.PeFileHeadAddre + 0x2);
	SizeOfOptionalHeader = *(ushort*)bdata.getDataByBuff(fileBuff, NtHead.PeFileHeadAddre + 0x10);


	//��ѡpeͷ
	NtHead.PeOptionHeadAddre = *(uint*)bdata.getDataByBuff(fileBuff, NtHeadAddre + 0x18);
	SizeOfHeaders = *(uint*)bdata.getDataByBuff(fileBuff, NtHead.PeOptionHeadAddre + 0x3c);

	//�ڱ�
	SectionHeadAddre = SizeOfHeaders - NtHeadAddre - 0x14 - SizeOfOptionalHeader;
	return 1;
}
