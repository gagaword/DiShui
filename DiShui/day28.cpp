#include "day28.h"

using namespace std;
/**
 * @brief 该方法通过导入表注入DLL
 * @param filePath 注入目标
 * @param InjectionDll 要注入的DLL
 * @param functionName DLL中导出函数名称
 * @return 
*/

bool MovImportAndInjectionDll(LPCSTR filePath, LPCSTR InjectionDll, LPCSTR functionName)
{
	if (!filePath || !InjectionDll || !functionName) return false;
	
	// 移动导入表
	LPVOID fileBuffer = nullptr;
	DWORD fileSize = ReadFile(filePath, &fileBuffer);

	cout << "文件大小: " << hex << uppercase << fileSize << endl;

	if (fileSize == 0)		return false;
	
	PEHeaders peheaders;
	if (!GetPeheadersInfo(fileBuffer,peheaders))	return false;
	
	// 扩大最后一个节，0x1000
	DWORD addByte = fileSize + 0x1000;
	unsigned char* NewFileBuffer = (unsigned char*)realloc(fileBuffer, addByte);
	memset(NewFileBuffer + fileSize , 0, 0x1000);

	// 更新PE信息
	PEHeaders newPeheaders;
	if (!GetPeheadersInfo(NewFileBuffer, newPeheaders))
	{
		return false;
	}

	for (size_t i = 0; i < newPeheaders.fileHeader->NumberOfSections; i++)
	{
		if (i == newPeheaders.fileHeader->NumberOfSections - 1)
		{
			newPeheaders.sectionHeader[i].Misc.VirtualSize += 0x1000;
			newPeheaders.sectionHeader[i].SizeOfRawData += 0x1000;
		}
	}
	// sizeOfImage也要增加0x1000，否则程序无法正常打开
	newPeheaders.optionalHeader->SizeOfImage += 0x1000;

	// 移动导入表到扩展节

	// 扩展节位置
	BYTE* addSectionAddress = (BYTE*)NewFileBuffer + fileSize;
	// 导入表位置
	PIMAGE_IMPORT_DESCRIPTOR importAddress = (PIMAGE_IMPORT_DESCRIPTOR)(DWORD*)((BYTE*)NewFileBuffer + *(DWORD*)newPeheaders.importSection);
	BYTE* copyImportAddress = (BYTE*)NewFileBuffer + *(DWORD*)newPeheaders.importSection;

	while (importAddress->OriginalFirstThunk != 0 && importAddress->FirstThunk != 0)
	{
		// 复制 IMAGE_IMPORT_DESCRIPTOR 结构体
		memcpy(addSectionAddress, copyImportAddress, sizeof(IMAGE_IMPORT_DESCRIPTOR));
		// 更新地址
		addSectionAddress += sizeof(IMAGE_IMPORT_DESCRIPTOR);
		copyImportAddress += sizeof(IMAGE_IMPORT_DESCRIPTOR);
		importAddress++;
	}

	// 追加IMAGE_IMPORT_BY_NAME结构
	// 前两个字节为0x00
	BYTE* addImportBy_Name_two = (BYTE*)((BYTE*)NewFileBuffer + fileSize + 0x200);
	
	BYTE* addImportBy_Name = (BYTE*)((BYTE*)NewFileBuffer + fileSize + 0x202);
	size_t bufferSize = strlen(functionName) + 1;
	strcpy_s(((char*)addImportBy_Name), bufferSize, functionName);

	// IMAGE_IMPORT_BY_NAME的RVA
	DWORD IMAGE_IMPORT_BY_NAME_RVA = addImportBy_Name_two - NewFileBuffer;
	cout << IMAGE_IMPORT_BY_NAME_RVA << endl;

	// INT
	BYTE* addImportData_INT = (BYTE*)((BYTE*)NewFileBuffer + fileSize + 0x100);
	memcpy(addImportData_INT, &IMAGE_IMPORT_BY_NAME_RVA, sizeof(DWORD));

	// IAT
	BYTE* addImportData_IAT = addImportData_INT + 0x8;
	memcpy(addImportData_IAT, &IMAGE_IMPORT_BY_NAME_RVA, sizeof(DWORD));

	// 新增导入表数据
	// 导入表最后的位置
	BYTE* EndImportAddress = addSectionAddress;

	DWORD address_INT = addImportData_INT - NewFileBuffer;
	DWORD address_IAT = addImportData_IAT - NewFileBuffer;

	memcpy(EndImportAddress, &address_INT, sizeof(DWORD));
	memcpy(EndImportAddress + 0x10, &address_IAT, sizeof(DWORD));

	// DLL名称字符串
	BYTE* DllNameAddress = (BYTE*)((BYTE*)NewFileBuffer + fileSize + 0x150);
	int DllNameSize = strlen(InjectionDll) + 1;
	strcpy_s(((char*)DllNameAddress), DllNameSize, InjectionDll);

	// 复制名称RVA到新导入表
	DWORD NameRVA = DllNameAddress - NewFileBuffer;
	memcpy(EndImportAddress + 0xc, &NameRVA, sizeof(DWORD));

	// 修改目录项中导入表的RVA指向新的导入表地址
	DWORD ImportRva = FoaToRva(fileSize, NewFileBuffer);
	newPeheaders.optionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = ImportRva;

	// 写入文件
	DWORD writeSize = FwritrFile(NewFileBuffer, addByte, filePath);

	
	return false;
}
