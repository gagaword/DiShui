#include<iostream>
#include "my_head.h"

#define notepadPath "C:\\Users\\GaGa\\Desktop\\eee.exe"

int main()
{
	// 通过类定义打开一个文件
	FILE* file = nullptr;
	openFile(&file, notepadPath, "rb");


	// 获取文件大小
	long fileSize = GetFileSize(file);

	// 读取文件数据
	unsigned char* result = (unsigned char*)ReadFileData(fileSize,1,file);

	// 使用临时指针指向result，不改变result指向的位置。
	unsigned char* temp = result;

	// 使用memcmp检查是否是PE文件
	CheckPE(result);

	free(result);
	return 0;
}