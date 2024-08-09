#include<iostream>	
#include<stdio.h>
#include<Windows.h>

#define DEBUG 1
//#define notepadPath "C:\\Windows\\system32\\notepad.exe"
#define notepadPath "C:\\Users\\GaGa\\Desktop\\eee.exe"


#define PRINTFLOG(DE,dest) if ((DE) == 1){ printf("%x\n",(dest));}
#define FALSE 0;

//constexpr auto notepadPath = "C:\\Windows\\system32\\notepad.exe";

/*
	errno_t fopen_s(
		FILE** pFile,
		const char *filename,
		const char *mode
);
*/

int main()
{
	// notepad保存的数据是指向文件指针的指针
	// 说白了就是二级指针

	FILE* notepad = NULL;

	// 打开文件
	errno_t err = fopen_s(&notepad, notepadPath, "rb");

	if (err == 0) {
		printf("notepda保存的数据:%p\n", notepad);
	}
	else {
		printf("Failed to open file: %s\n", notepadPath);
	}

	// 设置notepad指针指向文件尾
	fseek(notepad,0, SEEK_END);
	long ftellResult =  ftell(notepad);

	// 打印出ftellResult的结果
	PRINTFLOG(DEBUG, ftellResult);

	// 设置notepad指针指向文件头，当通过ftell获取到文件大小后再把notepad指针指向文件头，不然后面的读取会出错。
	fseek(notepad, 0, SEEK_SET);

	/*if (DEBUG == 1)
	{
		printf("%d", ftellResult);
	}*/

	
	// 分配内存空间
	unsigned char* notepadSpace = (unsigned char*)malloc(ftellResult);
	if (notepadSpace == NULL)
	{
		printf("Space Error");
		free(notepad);
		return FALSE;
	}

	// 读取内容，fread_s返回读取到的字节数，如果返回值不等于ftellResult，那么表示读取错误或读取失败
	size_t readNotepad = fread_s(notepadSpace, ftellResult, 1, ftellResult,notepad);
	if (readNotepad != ftellResult)
	{
		printf("read notepad.exe error\n");
		return FALSE;
	}

	// 关闭文件
	fclose(notepad);

	// 打印地址
	printf("Addres---->%p\n", notepadSpace);

	unsigned char* temp = notepadSpace; // 使用一个临时指针来遍历
	int count = 0;

	for (int i = 0; i < 200; i++) 
	{
		printf("%-4x ", *(temp++));
		count++;
		if (count == 16)
		{
			printf("\n");
			count = 0;
		}
	}

	printf("\n--------------------\n");
	// 释放内存
	//free(notepadSpace);

	// 打开目标文件
	FILE* dest = NULL;
	errno_t destExe = fopen_s(&dest, "test1.exe", "wb");
	// 写入目标文件
	size_t destNumber = fwrite(notepadSpace, 1, ftellResult, dest);
	printf("%x\n", destNumber);
	fclose(dest);
	free(notepadSpace);

	return 0;
}
