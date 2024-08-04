#include<iostream>
#include<Windows.h>
#include<string.h>
#include "my_head.h"

#define FilePath "C:\\Users\\GaGa\\Desktop\\A.exe"
#define VOID void


LPVOID readfile(const char* filepath)
{
	unsigned char* FileData = (unsigned char*)ReadFile(filepath);
	if (!FileData)
	{
		std::cout << "获取数据有误" << std::endl;
		return NULL;
	}
	else
	{
		std::cout << "读取成功" << std::endl;
		return FileData;
	}
}

VOID PrintfSECTION()
{
	LPVOID pFileBuffer = NULL;

	PIMAGE_DOS_HEADER pPIMAGE_DOS = nullptr;
	PIMAGE_NT_HEADERS32 pIMAGE_NT = nullptr;
	PIMAGE_FILE_HEADER pIMAGE_FILE = nullptr;
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL = nullptr;
	PIMAGE_SECTION_HEADER pSECTION = nullptr;

	// 读取文件
	pFileBuffer = readfile(FilePath);

	// 判断MZ头
	pPIMAGE_DOS = (PIMAGE_DOS_HEADER)pFileBuffer;
	if (*((PWORD)pPIMAGE_DOS) == IMAGE_DOS_SIGNATURE)
	{
		std::cout << "是MZ头" << std::endl;
	}

	// 判断PE头
	pIMAGE_NT = (PIMAGE_NT_HEADERS32)((DWORD)pFileBuffer + pPIMAGE_DOS->e_lfanew);

	if (*(PWORD)(pIMAGE_NT) == IMAGE_NT_SIGNATURE)
	{
		std::cout << "是PE头" << std::endl;
	}
	
	// 节表
	pIMAGE_FILE = &pIMAGE_NT->FileHeader;
	// 可选头的偏移量是紧跟在文件头之后的
	pIMAGE_OPTIONAL = &pIMAGE_NT->OptionalHeader;
	printf("%p\n", pIMAGE_OPTIONAL);

	// 节表紧跟在可选头之后
	pSECTION = (PIMAGE_SECTION_HEADER)((BYTE*)pIMAGE_OPTIONAL + pIMAGE_FILE->SizeOfOptionalHeader);

	unsigned char SECTION_NAME[9] = { 0 };

	// 遍历节表姓名
	for (int i = 0; i < pIMAGE_FILE->NumberOfSections; i++)
	{
		// 使用strncpy_s复制节名称并确保以null结尾
		// _TRUNCATE ((size_t)-1)
		strncpy_s((char*)SECTION_NAME, sizeof(SECTION_NAME), (const char*)pSECTION[i].Name, _TRUNCATE);
		std::cout << SECTION_NAME << std::endl;
		//printf("Section Name: %s\n", SECTION_NAME);
	}

	printf("****************************Misc.VirtualSize******************************************\n");
	// 打印节头的Misc字段，内存中大小
	for (int i = 0; i < pIMAGE_FILE->NumberOfSections; i++) {
		// 通过联合体访问Misc字段
		// Misc是一个联合体，可以通过不同的成员来访问信息
		printf("%08x\n", pSECTION[i].Misc.VirtualSize);

		//std::cout << "Misc: " << std::hex <<  pSECTION[i].Misc.VirtualSize << std::endl;
	}
	printf("*****************************SizeOfRawData********************************************\n");

	// 打印在文件中大小
	for (int i = 0; i < pIMAGE_FILE->NumberOfSections; i++) {
		// 通过联合体访问Misc字段
		// Misc是一个联合体，可以通过不同的成员来访问信息
		printf("%08x\n", pSECTION[i].SizeOfRawData);
		//std::cout << "SizeOfRawData: " << std::hex << pSECTION[i].SizeOfRawData << std::endl;
	}
	printf("******************************PointerToRawData****************************************\n");

	// 打印在文件中偏移
	for (int i = 0; i < pIMAGE_FILE->NumberOfSections; i++) {
		// 通过联合体访问Misc字段
		// Misc是一个联合体，可以通过不同的成员来访问信息
		printf("%08x\n", pSECTION[i].PointerToRawData);
		//std::cout << "PointerToRawData: " << std::hex << pSECTION[i].PointerToRawData << std::endl;
	}
	printf("*******************************Characteristics****************************************\n");

	// 打印在属性
	for (int i = 0; i < pIMAGE_FILE->NumberOfSections; i++) {
		// 通过联合体访问Misc字段
		// Misc是一个联合体，可以通过不同的成员来访问信息
		std::cout << std::hex << pSECTION[i].Characteristics << std::endl;
	}
	// 首节表+IMAGE_SIZEOF_SECTION_HEADER就是下一个节表
	/*pSECTION = (PIMAGE_SECTION_HEADER)((BYTE*)pSECTION + IMAGE_SIZEOF_SECTION_HEADER);
	printf("%s\n", pSECTION->Name);*/

}
int main()
{
	PrintfSECTION();
	return 0;
}