#pragma once
#include "global.h"
//#include <windows.h>
class PEFile
{
public:
	
	PEFile(const char *path);
	~PEFile();
	void printfHeadInfo();
	
private:
	int readFile(const char *path);
	int analyseFile();
	 uchar *fileBuff = NULL;
	 uchar *imageBuff = NULL;
	 long fileSize = 0;
	 long imageSize = 0;
	 //Dos头
	 uint NtHeadAddre = 0x0;

	 struct Nt {
		 uint NtStr = 0x0;
		 uint PeFileHeadAddre = 0x0;
		 uint PeOptionHeadAddre = 0x0;
	 } NtHead;
	 //标准pe
	 uint NumberOfSections = 0x0;
	 ushort SizeOfOptionalHeader = 0x0;

	 //可选pe头
	 uint SizeOfHeaders = 0x0;

	 //节表
	 uint SectionHeadAddre = 0x0;
};

