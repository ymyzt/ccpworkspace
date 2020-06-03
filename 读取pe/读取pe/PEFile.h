#pragma once
#include "global.h"
class PEFile
{
public:
	PEFile();
	~PEFile();
	int readFile(const char *path);
private:
	 uchar *fileBuff;
	 uchar *imageBuff;
	 long fileSize;
	 long imageSize;

};

