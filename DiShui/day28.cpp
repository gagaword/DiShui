#include "day28.h"


using namespace std;
/**
 * @brief 该方法通过导入表注入DLL
 * @param filePath 注入目标
 * @param InjectionDll 要注入的DLL
 * @param functionName DLL中导出函数名称
 * @return 
*/

unsigned int align_up(unsigned int value, unsigned int alignment) {
	return (value + alignment - 1) & ~(alignment - 1);

}

bool MovImportAndInjectionDll(LPCSTR filePath, LPCSTR InjectionDll, LPCSTR functionName)
{
	
	if (!filePath || !InjectionDll || !functionName)return false;
	

	// 读取数据
	LPVOID fileBuffer = nullptr;
	DWORD ReadFileSize = ReadFile(filePath, &fileBuffer);
	if (!ReadFileSize)return false;
	

	// PE信息
	PEHeaders peheaders;
	if (!GetPeheadersInfo(fileBuffer, peheaders)) return false;
	
	// 导入表大小
	DWORD importSize = peheaders.optionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;

	// 追加0x200字节
	DWORD addByte = importSize + 0x100;
	BYTE* NewFileBuffer = (BYTE*)realloc(fileBuffer, ReadFileSize + addByte);
	memset(ReadFileSize + NewFileBuffer, 0, addByte);

	// 更新PE信息
	PEHeaders newPeheaders;
	if (!GetPeheadersInfo(NewFileBuffer, newPeheaders))
	{
		return false;
	}

	// 导入表信息
	PIMAGE_IMPORT_DESCRIPTOR importAddress = (PIMAGE_IMPORT_DESCRIPTOR)(DWORD*)((BYTE*)NewFileBuffer + RvaToFoa(newPeheaders.optionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress, NewFileBuffer));

	// 获取程序对齐信息
	DWORD SectionAlign = newPeheaders.optionalHeader->SectionAlignment;
	DWORD FileAlign = newPeheaders.optionalHeader->FileAlignment;

	// 获取上一个节的VirtualAddress和PointerToRawData
	PIMAGE_SECTION_HEADER LastSection = newPeheaders.sectionHeader + (newPeheaders.fileHeader->NumberOfSections - 1);

	// 新增节表
	// 确定位置
	PIMAGE_SECTION_HEADER EndSectionAddress = (PIMAGE_SECTION_HEADER)((BYTE*)newPeheaders.sectionHeader + (newPeheaders.fileHeader->NumberOfSections * IMAGE_SIZEOF_SECTION_HEADER));

	// 节表头
	PIMAGE_SECTION_HEADER StratSectionAddress = IMAGE_FIRST_SECTION(newPeheaders.ntHeaders);

	// 复制节数据到新节表
	memcpy_s((BYTE*)EndSectionAddress, IMAGE_SIZEOF_SECTION_HEADER, (BYTE*)StratSectionAddress, IMAGE_SIZEOF_SECTION_HEADER);
	const char name[] = ".GaGa!!!";
	memcpy_s((BYTE*)EndSectionAddress->Name, IMAGE_SIZEOF_SHORT_NAME, (BYTE*)name, sizeof(name) - 1);

	// 修正PE字段
	newPeheaders.fileHeader->NumberOfSections += 0x1;
	newPeheaders.optionalHeader->SizeOfImage += addByte;

	// 修正节表属性
	EndSectionAddress->Misc.VirtualSize = addByte;
	EndSectionAddress->SizeOfRawData = addByte;
	EndSectionAddress->VirtualAddress = align_up(LastSection->VirtualAddress + LastSection->Misc.VirtualSize, SectionAlign);
	EndSectionAddress->PointerToRawData = align_up(LastSection->PointerToRawData + LastSection->SizeOfRawData, FileAlign);
	EndSectionAddress->Characteristics = IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE;


	// 移动导入表到新节
	BYTE* NewSectionAddress = (BYTE*)NewFileBuffer + ReadFileSize;
	memcpy(NewSectionAddress,(BYTE*)importAddress, importSize);

	// 新增导入表
	PIMAGE_IMPORT_DESCRIPTOR NewEndSectionAddress = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD*)((BYTE*)NewFileBuffer + ReadFileSize + importSize - sizeof(_IMAGE_IMPORT_DESCRIPTOR)));
	// 复制头导入表信息到新导入表
	memcpy(NewEndSectionAddress, (BYTE*)importAddress, sizeof(_IMAGE_IMPORT_DESCRIPTOR));

	// 复制DLL名称
	BYTE* DllNameAddress = (BYTE*)NewEndSectionAddress + sizeof(_IMAGE_IMPORT_DESCRIPTOR) * 2;
	DWORD NameSize = strlen(InjectionDll) + 1;
	strcpy_s((char*)DllNameAddress, NameSize, InjectionDll);

	// 名称RVA
	DWORD NameRva = FoaToRva(DllNameAddress - NewFileBuffer, NewFileBuffer);
	NewEndSectionAddress->Name = NameRva;

	// 创建一个BY_NAME
	PIMAGE_IMPORT_BY_NAME By_NameAddress = (PIMAGE_IMPORT_BY_NAME)((DWORD*)((BYTE*)DllNameAddress + NameSize));
	memset(By_NameAddress, 0, 0x2);
	DWORD FuncNameSize = strlen(functionName) + 0x1;
	strcpy_s((char*)By_NameAddress + 0x2, FuncNameSize, functionName);

	DWORD By_Name_RVA = FoaToRva((BYTE*)By_NameAddress - NewFileBuffer, NewFileBuffer);

	// 创建一个长度为8的INT和IAT
	BYTE* IntAndIatAddress = (BYTE*)By_NameAddress + FuncNameSize + 0x2;
	*(DWORD*)IntAndIatAddress = By_Name_RVA;

	// IAT
	// 因为是DWORD*类型，所以加0x2就相等于加了0x8
	BYTE* IatAddress = IntAndIatAddress + 0x8;
	*((DWORD*)IatAddress) = By_Name_RVA;

	// 修复新导入表的INT和IAT
	DWORD INT_RVA = FoaToRva(IntAndIatAddress - NewFileBuffer, NewFileBuffer);
	DWORD IAT_RVA = FoaToRva(IatAddress - NewFileBuffer, NewFileBuffer);
	NewEndSectionAddress->OriginalFirstThunk = INT_RVA;
	NewEndSectionAddress->FirstThunk = IAT_RVA;

	// 修正目录项
	DWORD NewSectionRva = FoaToRva(NewSectionAddress - NewFileBuffer, NewFileBuffer);
	newPeheaders.optionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = NewSectionRva;
	newPeheaders.optionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size += sizeof(_IMAGE_IMPORT_DESCRIPTOR);


	// 存盘
	FwritrFile(NewFileBuffer, ReadFileSize + addByte, filePath);

	return false;
}
