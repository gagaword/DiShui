#include "day22.h"
#include<string.h>

DWORD ReadFile22(IN LPCSTR FilePath, OUT LPVOID* FileData)
{
	if (FilePath == NULL)
	{
		std::cout << "参数错误" << std::endl;
		return 0;
	}

	// 打开文件
	FILE* pFile = NULL;
	errno_t FopenResult = fopen_s(&pFile, FilePath,"rb");
	if (FopenResult != 0)
	{
		std::cout << "打开文件失败" << std::endl;
		return 0;
	}

	// 获取文件大小
	fseek(pFile, 0, SEEK_END);
	long FileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	LOG(__func__, pFile);

	// 分配内存空间
	*FileData = malloc(FileSize);
	if (*FileData == NULL)
	{
		std::cout << "内存分配错误" << std::endl;
		fclose(pFile);
		return 0;
	}
	memset(*FileData, 0, FileSize);

	// 读取文件数据
	size_t FreadResult = fread_s(*FileData, FileSize, 1 ,FileSize, pFile);
	if (FreadResult != FileSize)
	{
		std::cout << "读取错误" << std::endl;
		fclose(pFile);
		free(*FileData);
		return 0;
	}
	LOG(__func__, FreadResult);
	fclose(pFile);
	return FreadResult;
}

DWORD CopyFileBufferToImageBuffer(IN LPVOID FileBuffer, OUT LPVOID* ImageBuffer)
{

	if (FileBuffer == NULL)
	{
		return 0;
	}
	
	// 解析 FileBuffer PE 字段
	PIMAGE_DOS_HEADER pDOS = (PIMAGE_DOS_HEADER)FileBuffer;
	if (pDOS->e_magic != IMAGE_DOS_SIGNATURE) {
		std::cerr << "无效的 DOS 签名" << std::endl;
		return 0;
	}

	PIMAGE_NT_HEADERS32 pNT = (PIMAGE_NT_HEADERS32)((BYTE*)FileBuffer + pDOS->e_lfanew);
	if (pNT->Signature != IMAGE_NT_SIGNATURE) {
		std::cerr << "无效的 NT 签名" << std::endl;
		return 0;
	}

	PIMAGE_OPTIONAL_HEADER32 pOPTIONAL = &pNT->OptionalHeader;
	PIMAGE_FILE_HEADER pFILE = &pNT->FileHeader;
	PIMAGE_SECTION_HEADER pSECTION = (PIMAGE_SECTION_HEADER)((BYTE*)pOPTIONAL + pFILE->SizeOfOptionalHeader);

	// 分配 ImageBuffer 空间
	*ImageBuffer = malloc(pOPTIONAL->SizeOfImage);
	if (*ImageBuffer == NULL) {
		std::cerr << "内存分配错误" << std::endl;
		return 0;
	}

	// 清空 ImageBuffer
	memset(*ImageBuffer, 0, pOPTIONAL->SizeOfImage);

	// 复制 DOS 头和 PE 头到 ImageBuffer
	memcpy(*ImageBuffer, FileBuffer, pOPTIONAL->SizeOfHeaders);
	uint64_t CopyData = pOPTIONAL->SizeOfHeaders;

	// 复制节到 ImageBuffer
	for (int i = 0; i < pFILE->NumberOfSections; i++) {
		if (pSECTION[i].SizeOfRawData == 0) {
			continue;
		}

		void* dest = (BYTE*)*ImageBuffer + pSECTION[i].VirtualAddress;
		void* src = (BYTE*)FileBuffer + pSECTION[i].PointerToRawData;

		memcpy(dest, src, pSECTION[i].SizeOfRawData);
		CopyData += pSECTION[i].SizeOfRawData;
	}

	return CopyData;
}

DWORD CopyImageBufferToNewBuffer(IN LPVOID ImageBuffer, OUT LPVOID* NewBuffer)
{
	// 注意NewBuffer是指针类型，传递的参数是NewBuffer指向的数据
	// 要操作这个数据需要对NewBuffer进行解引用。
	// 参数判断
	if (ImageBuffer == NULL)
	{
		std::cout << "参数错误" << std::endl;
		return 0;
	}

	// 解析 ImageBuffer 中 PE字段
	PIMAGE_DOS_HEADER pDOS = NULL;
	PIMAGE_NT_HEADERS32 pNT = NULL;
	PIMAGE_FILE_HEADER pFILE = NULL;
	PIMAGE_OPTIONAL_HEADER32 pOPTIONAL = NULL;
	PIMAGE_SECTION_HEADER pSECTION = NULL;

	// DOS头
	pDOS = (PIMAGE_DOS_HEADER)ImageBuffer;
	// NT头
	pNT = (PIMAGE_NT_HEADERS32)((BYTE*)ImageBuffer + pDOS->e_lfanew);
	// PE头
	pFILE = &pNT->FileHeader;
	// 扩展PE头
	pOPTIONAL = &pNT->OptionalHeader;
	// 节表
	pSECTION = (PIMAGE_SECTION_HEADER)((BYTE*)pOPTIONAL + pFILE->SizeOfOptionalHeader);


	// ImageBuffer是通过文件进行拉伸后的数据
	// 在把数据缩放回去既是NweBuffer的数据
	// 通过节表结构字段进行缩放
	// 分配文件大小字节给NewBuffer
	// 复制DOS + PE + 节表到NewBuffer

	
	// NewBuffer分配空间
	// 分配空间后会把空间首地址给*NewBuffer，所以*NewBuffer保存的是空间的地址，需要再次解引用取值。
	*NewBuffer = malloc(pOPTIONAL->SizeOfHeaders + pOPTIONAL->SizeOfImage);
	if (*NewBuffer == NULL)
	{
		std::cout << "内存分配错误" << std::endl;
		return 0;
	}
	memset(*NewBuffer, 0, pOPTIONAL->SizeOfHeaders + pOPTIONAL->SizeOfImage);

	std::cout << std::hex << pSECTION->Misc.VirtualSize << std::endl;
	
	// 复制Headers到NewBuffer，复制大小SizeOfHeaders
	/*for (size_t i = 0; i < pOPTIONAL->SizeOfHeaders; i++)
	{
		*((char*)*NewBuffer + i) = *((char*)ImageBuffer + i);
	}*/
	memcpy(*NewBuffer, ImageBuffer, pOPTIONAL->SizeOfHeaders);
	uint64_t CopyData = pOPTIONAL->SizeOfHeaders;

	// 复制节到NewBuffer，复制大小为SizeOfRawData, 文件偏移为PointerToRawData,数量为FILE->NumberOfSections.
	// 内存偏移为 VirtualAddress
	for (int i = 0; i < pFILE->NumberOfSections; i++)
	{
		if (pSECTION[i].PointerToRawData == 0)
		{
			continue;
		}

		void* dest = (BYTE*)*NewBuffer + pSECTION[i].PointerToRawData;
		void* src = (BYTE*)ImageBuffer + pSECTION[i].VirtualAddress;
		memcpy(dest, src, pSECTION[i].Misc.VirtualSize);
		CopyData += pSECTION[i].SizeOfRawData;
		/*for (int j = 0; j < pSECTION[i].SizeOfRawData; j++)
		{
			*((unsigned char*)*NewBuffer + pSECTION[i].PointerToRawData + j) = *((unsigned char*)ImageBuffer + pSECTION[i].VirtualAddress + j);
		}*/
	}

	// 测试信息
	if (DEBUG == 1)
	{
		for (int j = 0; j < pSECTION[1].SizeOfRawData; j++)
		{
			printf("%02x  ", *((unsigned char*)*NewBuffer + pSECTION[1].PointerToRawData + j));
		}
	}
	return CopyData;
}

DWORD MemeryTOFile(IN LPVOID pMemBuffer, IN size_t size, OUT LPSTR lpszFile)
{
	
	FILE* pFile = NULL;
	fopen_s(&pFile, lpszFile, "wb");
	if (pFile == NULL)
	{
		std::cout << "写入文件失败" << std::endl;
		return 0;
	}
	size_t result = fwrite((unsigned char*)pMemBuffer, 1, size, pFile);
	if (result == size)
	{
		return result;
	}
	return 0;
}

DWORD RvaTOFileOffset(IN LPVOID FileBuffer, IN DWORD dwRva)
{
	// 获取 PE 结构基本信息
	PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)FileBuffer;
	PIMAGE_NT_HEADERS pNTHeaders = (PIMAGE_NT_HEADERS)((BYTE*)FileBuffer + pDOSHeader->e_lfanew);

	// 原来还能这样获取到节表头，操了，原来微软提供了啊。
	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(pNTHeaders);
	DWORD sectionCount = pNTHeaders->FileHeader.NumberOfSections;
	DWORD FileOffset = 0;

	for (size_t i = 0; i < sectionCount; i++)
	{
		if (pSectionHeader[i].PointerToRawData == 0)
		{
			continue;
		}
		if ((pSectionHeader[i].VirtualAddress) <= dwRva && (pSectionHeader[i].VirtualAddress + pSectionHeader[i].Misc.VirtualSize) >= dwRva)
		{
			FileOffset = ((dwRva - pSectionHeader[i].VirtualAddress) + pSectionHeader[i].PointerToRawData);
			return FileOffset;
		}
	}
	return 0;
}
