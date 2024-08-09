#include<iostream>	
#include<stdio.h>
#include<Windows.h>

#define ConstfilePath "C:\\Users\\GaGa\\Desktop\\A.exe"

LPVOID ReadFile(const char* FilePath)
{
	// 判断参数
	if (FilePath == NULL)
	{
		std::cout << "参数错误" << std::endl;
		return NULL;
	}

	// 打开文件
	FILE* file = nullptr;
	errno_t isFile = fopen_s(&file, FilePath, "rb");
	if (isFile != 0)
	{
		std::cout << "打开文件失败" << std::endl;
		return NULL;
	}

	// 分配内存
	fseek(file,0,SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	unsigned char* FileSpace = (unsigned char*)malloc(fileSize);
	if (FileSpace == NULL)
	{
		std::cout << "分配内存错误" << std::endl;
		fclose(file);
		return NULL;
	}
	memset(FileSpace,0,fileSize);
	// 读取数据
	size_t ReadFileSize = fread_s(FileSpace, fileSize, 1, fileSize, file);
	if (ReadFileSize != fileSize)
	{
		std::cout << "读取可能失败" << std::endl;
		fclose(file);
		free(FileSpace);
		return NULL;
	}
	fclose(file);
	std::cout << "文件字节:" << fileSize << std::endl;
	return FileSpace;
}

VOID PrintfNTData()
{
	LPVOID pFileBuffer = NULL;

	PIMAGE_DOS_HEADER pDosHeader = NULL;
	PIMAGE_NT_HEADERS32 pNtHeader = nullptr;
	PIMAGE_FILE_HEADER pFileHeader = NULL;
	PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = NULL;
	PIMAGE_SECTION_HEADER pSectionHeader = NULL;

	// FileData保存文件的指针
	pFileBuffer = ReadFile(ConstfilePath);

	if (!pFileBuffer)
	{
		std::cout << "读取错误" << std::endl;
		return;
	}


	/*unsigned char* temp = (unsigned char*)pFileBuffer;
	for (size_t i = 0; i < 20; i++)
	{
		printf("%x ", *(temp++));
	}*/
	
	// 判断是否PE文件
	if (*((PWORD)pFileBuffer) == IMAGE_DOS_SIGNATURE)
	{
		std::cout << "是有效MZ文件" << std::endl;
	}
	else 
	{
		std::cout << "不是有效MZ文件" << std::endl;
		free(pFileBuffer);
		return;
	}

	// 打印ODS头
	std::cout << "*****************************DOS_HEADER*****************************" << std::endl;
	pDosHeader = (PIMAGE_DOS_HEADER)pFileBuffer;
	std::cout << std::hex << pDosHeader->e_magic << std::endl;
	std::cout << std::hex << pDosHeader->e_lfanew << std::endl;

	// 打印NT头,NT头要注意加上DOS的lfanew.
	std::cout << "*****************************NT_HEADER******************************" << std::endl;
	pNtHeader = (PIMAGE_NT_HEADERS32)((DWORD)pFileBuffer + pDosHeader->e_lfanew);
	std::cout << std::hex << pNtHeader->Signature << std::endl;

	if (*((PWORD)((DWORD)pFileBuffer + pDosHeader->e_lfanew)) != IMAGE_NT_SIGNATURE)
	{
		std::cout << "不是有效PE文件" << std::endl;
		free(pFileBuffer);
		return;
	}
	else
	{
		std::cout << "有效PE文件" << std::endl;
	}


	// 打印标准PE头
	std::cout << "*****************************FILE_HEADER****************************" << std::endl;
	pFileHeader = (PIMAGE_FILE_HEADER)(((DWORD)pNtHeader)+ 0x4);
	std::cout << std::hex << "Machine:" << pFileHeader->Machine << std::endl;
	std::cout << std::hex << "NumberOfSections:" << pFileHeader->NumberOfSections << std::endl;
	std::cout << std::hex << "TimeDateStamp:" << pFileHeader->TimeDateStamp << std::endl;
	std::cout << std::hex << "SizeOfOptionalHeader:" << pFileHeader->SizeOfOptionalHeader << std::endl;
	std::cout << std::hex << "Characteristics:" << pFileHeader->Characteristics << std::endl;


	std::cout << "*****************************OPTIONAL_HEADER****************************" << std::endl;
	// 打印扩展PE头
	pOptionalHeader = (PIMAGE_OPTIONAL_HEADER)(((DWORD)pFileHeader) + IMAGE_SIZEOF_FILE_HEADER);
	std::cout << std::hex << "Magic:" << pOptionalHeader->Magic << std::endl;
	std::cout << std::hex << "SizeOfCode:" << pOptionalHeader->SizeOfCode << std::endl;
	std::cout << std::hex << "SizeOfUninitializedData:" << pOptionalHeader->SizeOfUninitializedData << std::endl;
	std::cout << std::hex << "AddressOfEntryPoint:" << pOptionalHeader->AddressOfEntryPoint << std::endl;
	std::cout << std::hex << "BaseOfCode:" << pOptionalHeader->BaseOfCode << std::endl;
	std::cout << std::hex << "BaseOfData:" << pOptionalHeader->BaseOfData << std::endl;
	std::cout << std::hex << "ImageBase:" << pOptionalHeader->ImageBase << std::endl;
	std::cout << std::hex << "SectionAlignment:" << pOptionalHeader->SectionAlignment << std::endl;
	std::cout << std::hex << "FileAlignment:" << pOptionalHeader->FileAlignment << std::endl;
	std::cout << std::hex << "DllCharacteristics:" << pOptionalHeader->DllCharacteristics << std::endl;
	std::cout << std::hex << "SizeOfStackReserve:" << pOptionalHeader->SizeOfStackReserve << std::endl;
	std::cout << std::hex << "SizeOfStackCommit:" << pOptionalHeader->SizeOfStackCommit << std::endl;
	std::cout << std::hex << "SizeOfHeapReserve:" << pOptionalHeader->SizeOfHeapReserve << std::endl;
	std::cout << std::hex << "SizeOfHeapCommit:" << pOptionalHeader->SizeOfHeapCommit << std::endl;
	std::cout << std::hex << "LoaderFlags:" << pOptionalHeader->LoaderFlags << std::endl;
	std::cout << std::hex << "NumberOfRvaAndSizes:" << pOptionalHeader->NumberOfRvaAndSizes << std::endl;
	free(pFileBuffer);
	
}
int main()
{

	PrintfNTData();

	return 0;
}