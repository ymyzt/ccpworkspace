#include <iostream>
#include "PEFile.h"
#include <fstream>
using namespace std;
PEFile::PEFile()
{
}

PEFile::~PEFile()
{
}
int PEFile::readFile(const char *path) {
	/*FILE *f = fopen(path, "rb");
	if (f == NULL) {
		std::cout << "file open fail!!" << std::endl;
		return 0;
	}
	fseek(f, 0, SEEK_END);
	long  len = ftell(f);
	uchar * buff = new uchar[len];
	memset(buff, 0, len);
	fseek(f, 0, SEEK_SET);
	int res = fread(buff, len, 1, f);
	fclose(f);*/
	ifstream  file;
	file.open(path, ios::binary | ios::in);
	if (!file.is_open()) {
		cout << "打开失败!" << endl;
		return 0;
	}
	file.seekg(0, ios::end);
	fileSize = file.tellg();
	file.seekg(0, ios::beg);
	//fileBuff << file;
	if (fileSize <= 0) {
		cout << "文件长度错误!" << endl;
		return 0;
	}
	fileBuff = new uchar[fileSize];
	if (fileBuff == NULL) {
		cout << "分配空间失败!" << endl;
		return 0;
	}
	file >> fileBuff;
	return 1;
}