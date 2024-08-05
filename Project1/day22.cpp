#include "day22.h"
#include<string.h>

#define DEBUG 1
#define LOG(FUN_NAME,VALUES) if ((DEBUG) == 1){std::cout << FUN_NAME << "---->" << std::hex << VALUES << std::endl;}
#define IMAGE_SIZEOF_Signature 0x4
DWORD ReadFile(IN const char* FilePath, OUT LPVOID* FileData)
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
	// 获取ImageBuffer中 PE 需要的字段
	PIMAGE_DOS_HEADER pDOS = NULL;
	PIMAGE_NT_HEADERS32 pNT = NULL;
	PIMAGE_FILE_HEADER pFILE = NULL;
	PIMAGE_OPTIONAL_HEADER32 pOPTIONAL = NULL;
	PIMAGE_SECTION_HEADER pSECTION = NULL;

	// DOS头
	pDOS = (PIMAGE_DOS_HEADER)FileBuffer;
	LOG(__func__, pDOS->e_magic);

	// 开辟ImageBuffer空间
	// ImageBuffer的空间大小在OPTIONAL.SizeOfImage字段
	pOPTIONAL = (PIMAGE_OPTIONAL_HEADER32)((DWORD)FileBuffer + pDOS->e_lfanew + IMAGE_SIZEOF_FILE_HEADER + IMAGE_SIZEOF_Signature);

	// 测试信息
	printf("SizeOfImage--->%#x\n", pOPTIONAL->SizeOfImage);

	*ImageBuffer = malloc(pOPTIONAL->SizeOfImage);
	if (*ImageBuffer == NULL)
	{
		std::cout << "内存分配错误" << std::endl;
		return 0;
	}

	memset(*ImageBuffer, 0, pOPTIONAL->SizeOfImage);

	// 复制DOS,PE,节到ImageBuffer,这些数据在文件中是什么样在ImageBuffer中就是什么样子
	// OPTIONAL中的 SizeOfHeaders 字段就是这些数据的大小。
	for (int i = 0; i < pOPTIONAL->SizeOfHeaders; i++)
	{
		*((unsigned char*)*ImageBuffer + i) = *((unsigned char*)FileBuffer + i);
	}

	// 测试信息
	for (int i = 0; i < 0x28; i++)
	{
		printf("%02x ", *((unsigned char*)*ImageBuffer + i));
	}

	printf("\n");
	// 通过节表中的VisualAddress字段计算该节表在内存中的偏移，进行复制，复制大小为SizeOfRawData。
	// 首节表位置
	pNT = (PIMAGE_NT_HEADERS32)((BYTE*)FileBuffer + pDOS->e_lfanew);
	pFILE = &pNT->FileHeader;
	pOPTIONAL = &pNT->OptionalHeader;
	pSECTION = (PIMAGE_SECTION_HEADER)((BYTE*)pOPTIONAL + pFILE->SizeOfOptionalHeader);

	// 复制节到ImageBase
	for (int i = 0; i < pFILE->NumberOfSections; i++)
	{
		printf("节文件偏移---->%x\n", pSECTION[i].PointerToRawData);
		if (pSECTION[i].PointerToRawData == 0)
		{			
			continue;
		}
		for (int j = 0; j < pSECTION[i].SizeOfRawData; j++)
		{
			// 这里的运算简化了
			*((unsigned char*)*ImageBuffer + pSECTION[i].VirtualAddress + j) = *((unsigned char*)FileBuffer + pSECTION[i].PointerToRawData + j);
		}
	}

	// 测试信息
	/*for (int j = 0; j < pSECTION[2].SizeOfRawData; j++)
	{
		*((unsigned char*)*ImageBuffer + pSECTION[2].VirtualAddress + j) = *((unsigned char*)FileBuffer + pSECTION[2].PointerToRawData + j);
		printf("%02x  ", *((unsigned char*)*ImageBuffer + pSECTION[2].VirtualAddress + j));
	}*/
	
	return 0;
}
