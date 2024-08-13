#include "my_25.h"
#include "My_day23.h"

bool GetPeheadersInfo(LPVOID Buffer, PEHeaders peheader)
{
	if (Buffer == NULL)
	{
		return false;
	}

	// DOS Header
	peheader.dosHeader = (PIMAGE_DOS_HEADER)Buffer;
	if (peheader.dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		perror("DOS头签名错误");
		return false;
	}

	// NT Header
	peheader.ntHeaders = (PIMAGE_NT_HEADERS32)((BYTE*)Buffer + peheader.dosHeader->e_lfanew);

	// FILE Header
	peheader.fileHeader = &peheader.ntHeaders->FileHeader;

	// OPTION Header
	peheader.optionalHeader = &peheader.ntHeaders->OptionalHeader;

	// SECTIOn Header
	peheader.sectionHeader = (PIMAGE_SECTION_HEADER)((BYTE*)peheader.optionalHeader + peheader.fileHeader->SizeOfOptionalHeader);

	return false;
}

bool PrintDataExport(LPCSTR filePath)
{
	if (!filePath)
	{
		perror("参数错误");
		return false;
	}
	// 读取文件到FileBuffer
	LPVOID fileBuffer = NULL;
	DWORD readFileResult = ReadFile(filePath, &fileBuffer);

	if (!readFileResult)
	{
		return false;
	}

	// 获取PE信息
	return false;
}
