#include "day22.h"
#include<string.h>


/// <summary>
/// 该方法读取一个文件的数据
/// </summary>
/// <param name="FilePath">文件的路径</param>
/// <param name="FileBuffer">保存文件数据的指针的指针</param>
/// <returns>
/// 返回0表示读取失败，返回字节表示读取成功。
/// </returns>
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

/// <summary>
/// 该方法复制FileBufer到ImageBuffer中
/// </summary>
/// <param name="FileBuffer">FileBuffer指针</param>
/// <param name="ImageBuffer">ImageBuffer指针</param>
/// <returns>
/// 返回0表示失败，否则返回复制的大小
/// </returns>
//DWORD CopyFileBufferToImageBuffer(IN LPVOID FileBuffer, OUT LPVOID* ImageBuffer)
//{
//
//	 解析 FileBuffer PE 字段
//	PIMAGE_DOS_HEADER pDOS = NULL;
//	PIMAGE_NT_HEADERS32 pNT = NULL;
//	PIMAGE_FILE_HEADER pFILE = NULL;
//	PIMAGE_OPTIONAL_HEADER32 pOPTIONAL = NULL;
//	PIMAGE_SECTION_HEADER pSECTION = NULL;
//
//	 DOS头
//	pDOS = (PIMAGE_DOS_HEADER)FileBuffer;
//	LOG(__func__, pDOS->e_magic);
//
//	 复制的数据大小
//	uint64_t CopyDataSize = 0;
//
//	 开辟ImageBuffer空间
//	 ImageBuffer的空间大小在OPTIONAL.SizeOfImage字段
//	pOPTIONAL = (PIMAGE_OPTIONAL_HEADER32)((DWORD)FileBuffer + pDOS->e_lfanew + IMAGE_SIZEOF_FILE_HEADER + IMAGE_SIZEOF_Signature);
//
//	 测试信息
//	LOG(__func__, pOPTIONAL->SizeOfImage);
//
//	*ImageBuffer = malloc(pOPTIONAL->SizeOfImage);
//	if (*ImageBuffer == NULL)
//	{
//		std::cout << "内存分配错误" << std::endl;
//		return 0;
//	}
//
//	memset(*ImageBuffer, 0, pOPTIONAL->SizeOfImage);
//	 复制DOS,PE,节到ImageBuffer,这些数据在文件中是什么样在ImageBuffer中就是什么样子
//	 OPTIONAL中的 SizeOfHeaders 字段就是这些数据的大小。
//	for (int i = 0; i < pOPTIONAL->SizeOfHeaders; i++)
//	{
//		*((unsigned char*)*ImageBuffer + i) = *((unsigned char*)FileBuffer + i);
//	}
//	
//	if (DEBUG == 1)
//	{
//		// 测试信息
//		for (int i = 0; i < 0x28; i++)
//		{
//			printf("%02x ", *((unsigned char*)*ImageBuffer + i));
//		}
//	}
//	
//
//	printf("\n");
//	 通过节表中的VisualAddress字段计算该节表在内存中的偏移，进行复制，复制大小为SizeOfRawData。
//	 首节表位置
//	pNT = (PIMAGE_NT_HEADERS32)((BYTE*)FileBuffer + pDOS->e_lfanew);
//	pFILE = &pNT->FileHeader;
//	pOPTIONAL = &pNT->OptionalHeader;
//	pSECTION = (PIMAGE_SECTION_HEADER)((BYTE*)pOPTIONAL + pFILE->SizeOfOptionalHeader);
//
//	 临时变量
//	unsigned char* PIMAGE = (unsigned char*)*(ImageBuffer);
//	unsigned char* PFILE = (unsigned char*)FileBuffer;
//
//	 复制节到ImageBase
//	for (int i = 0; i < pFILE->NumberOfSections; i++)
//	{
//		printf("节文件偏移---->%x\n", pSECTION[i].PointerToRawData);
//		printf("节内存偏移---->%x\n", pSECTION[i].VirtualAddress);
//		CopyDataSize += pSECTION[i].SizeOfRawData;
//		if (pSECTION[i].PointerToRawData == 0)
//		{			
//			continue;
//		}
//	
//		for (int j = 0; j < pSECTION[i].SizeOfRawData; j++)
//		{
//			/* 两种写法，其实是一种，只是提取了一些变量使得易读*/
//			*(PIMAGE + pSECTION[i].VirtualAddress + j) = *(PFILE + pSECTION[i].PointerToRawData + j);
//			*((unsigned char*)*ImageBuffer + pSECTION[i].VirtualAddress + j) = *((unsigned char*)FileBuffer + pSECTION[i].PointerToRawData + j);
//		}
//	}
//
//	if (DEBUG == 1)
//	{
//		// 测试信息
//		for (int j = 0; j < pSECTION[1].SizeOfRawData; j++)
//		{
//			printf("%02x  ", *((unsigned char*)*ImageBuffer + pSECTION[1].VirtualAddress + j));
//		}
//	}
//
//	CopyDataSize += pOPTIONAL->SizeOfHeaders;
//	return CopyDataSize;
//}


// 经过GPT优化
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


/// <summary>
/// 该方法复制ImageBuffer到NewBuffer中，使得NewBuffer数据存盘且可运行
/// </summary>
/// <param name="ImageBuffer">ImageBuffer指针</param>
/// <param name="NewBuffer">NweBuffer指针</param>
/// <returns>
/// 返回0表示复制失败，否者返回复制的大小
/// </returns>
/// 
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
	*NewBuffer = malloc(pOPTIONAL->SizeOfImage);
	if (*NewBuffer == NULL)
	{
		std::cout << "内存分配错误" << std::endl;
		return 0;
	}
	memset(*NewBuffer, 0, pOPTIONAL->SizeOfImage);
	
	// 复制Headers到NewBuffer，复制大小SizeOfHeaders
	/*for (size_t i = 0; i < pOPTIONAL->SizeOfHeaders; i++)
	{
		*((char*)*NewBuffer + i) = *((char*)ImageBuffer + i);
	}*/
	memcpy(*NewBuffer,ImageBuffer,pOPTIONAL->SizeOfHeaders);
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
		memcpy(dest, src, pSECTION[i].SizeOfRawData);
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


/// <summary>
/// 该方法将内存中的数据写入到文件中
/// </summary>
/// <param name="pMemBuffer">内存数据</param>
/// <param name="size">要复制的大小</param>
/// <param name="lpszFile">要保存文件的路径</param>
/// <returns>
/// 返回0表示写入失败，否则返回复制的大小
/// </returns>
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

/// <summary>
/// 该方法把内存偏移转为文件偏移
/// </summary>
/// <param name="FileBuffer">FileBuffer指针</param>
/// <param name="dwRva">内存偏移</param>
/// <returns>
/// 返回0表示转换失败，否则返回文件偏移FOA
/// </returns>
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
