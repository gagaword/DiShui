#include<iostream>
#include "my_head.h"

#define notepadPath "C:\\Users\\GaGa\\Desktop\\eee.exe"

const char PE[] = {
	0x4D,0x5A
};

int main()
{
	// 通过类定义打开一个文件
	FILE* file = nullptr;
	openFile(&file, notepadPath, "rb");
	std::cout << file << std::endl;

	// 获取文件大小
	long fileSize = GetFileSize(file);
	std::cout << fileSize << std::endl;

	// 读取文件数据
	unsigned char* result = (unsigned char*)ReadFileData(fileSize,1,file);

	// 使用临时指针指向result，不改变result指向的位置。
	unsigned char* temp = result;

	// 使用memcmp检查是否是PE文件
	if (memcmp(result, PE, sizeof(PE)) == 0) {
		std::cout << "是PE文件" << std::endl;
	}
	else {
		std::cout << "不是PE文件" << std::endl;
	}

	free(result);
	return 0;
}