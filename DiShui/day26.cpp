#include "day26.h"
#include "My_day23.h"
#include "my_25.h"

using namespace std;

bool PrintBaseloc(LPCSTR filePath)
{
	if (!filePath)
	{
		return false;
	}

	LPVOID fileBuffer = nullptr;
	if (!ReadFile(filePath, &fileBuffer))
	{
		return false;
	}

	PEHeaders peheader{};
	if (!GetPeheadersInfo(fileBuffer, peheader))
	{
		return false;
	}

	if (peheader.optionalHeader == nullptr)
	{
		return false;
	}

	// 重定位表
	PIMAGE_BASE_RELOCATION pBASE_RELOCATION = (PIMAGE_BASE_RELOCATION)&peheader.optionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
	PIMAGE_BASE_RELOCATION PBASE = (PIMAGE_BASE_RELOCATION)((BYTE*)fileBuffer + RvaToFov(pBASE_RELOCATION->VirtualAddress, fileBuffer));
	// 数据
	while (PBASE->VirtualAddress != 0)
	{
		// 数据数量
		DWORD BaseNumer = (PBASE->SizeOfBlock - (sizeof(DWORD) * 2)) / sizeof(WORD);
		// 起始位置
		WORD* BaseAddress1 = (WORD*)((BYTE*)PBASE + sizeof(DWORD) * 2);
		cout << "*************" << hex << PBASE->VirtualAddress << "**************" << endl;
		for (size_t i = 0; i < BaseNumer; i++)
		{
			if (((*BaseAddress1 & 0xF000) >> 12) != IMAGE_REL_BASED_HIGHLOW)
			{
				break;
			}
			cout << "RVA: " << hex << uppercase << (PBASE->VirtualAddress + ((*BaseAddress1) & 0xFFF)) << "  ";
			cout << "Offset " << hex << uppercase << RvaToFov((PBASE->VirtualAddress + ((*BaseAddress1) & 0xFFF)), fileBuffer) << "  ";
			cout << "Type " << hex << uppercase << ((*BaseAddress1 & 0xF000) >> 12) << endl;
			BaseAddress1++;
		}
		
		// 移动到下一个重定位表
		PBASE = (PIMAGE_BASE_RELOCATION)((DWORD)((BYTE*)PBASE + PBASE->SizeOfBlock));
	}
	return true;
}

bool MoveExport(LPCSTR filePath)
{
	if (!filePath)	return false;

	LPVOID fileBuffer = nullptr;
	DWORD readfileResu = ReadFile(filePath, &fileBuffer);
	if (!readfileResu) return false;

	PEHeaders peheader{};
	if (!GetPeheadersInfo(fileBuffer, peheader))
	{
		free(fileBuffer);
		return false;
	}

	// 新增节
	// 追加字节
	DWORD addByte = 0x1000;
	unsigned char* newBuffer = (unsigned char*)realloc(fileBuffer, readfileResu + addByte);
	if (!newBuffer)
	{
		free(fileBuffer);
		return false;
	}
	memset(newBuffer + readfileResu, 0, addByte);

	// 更新PE信息
	PEHeaders newPehear{};

	if (!GetPeheadersInfo(newBuffer, newPehear))return false;
	if (newPehear.optionalHeader == nullptr) return false;

	// 新节表位置
	PIMAGE_SECTION_HEADER addSectionAddress = (PIMAGE_SECTION_HEADER)((BYTE*)newPehear.sectionHeader + (IMAGE_SIZEOF_SECTION_HEADER * newPehear.fileHeader->NumberOfSections));

	// 复制数据到新节表
	memcpy_s((char*)addSectionAddress, IMAGE_SIZEOF_SECTION_HEADER, (char*)newPehear.sectionHeader, IMAGE_SIZEOF_SECTION_HEADER);
	addSectionAddress->Characteristics |= IMAGE_SCN_MEM_EXECUTE;
	const char name[] = ".GaGa!!!";
	memcpy_s((char*)addSectionAddress->Name, IMAGE_SIZEOF_SHORT_NAME, (char*)name, sizeof(name) - 1);

	// 修正节表字段
	DWORD newVirtualAddress = 0;
	DWORD newPointerAddress = 0;
	DWORD newSizeOfRawData = addByte;
	DWORD newVirtualSize = addByte;

	// 获取新节表字段
	for (size_t i = 0; i < newPehear.fileHeader->NumberOfSections; i++)
	{
		if (i == newPehear.fileHeader->NumberOfSections - 1)
		{
			newPointerAddress = newPehear.sectionHeader[i].PointerToRawData + newPehear.sectionHeader[i].SizeOfRawData;
			DWORD align = (((newPehear.sectionHeader[i].Misc.VirtualSize + newPehear.sectionHeader[i].VirtualAddress) + 0xFFF) / 0x1000) * 0x1000;
			newVirtualAddress = align;
			/*cout << hex << align << endl;
			cout << newPehear.sectionHeader[i].Name << endl;*/
			break;
		}
	}

	// 修正PE字段
	newPehear.fileHeader->NumberOfSections += 0x1;
	newPehear.optionalHeader->SizeOfImage += 0x1000;

	// 修正新节表字段
	addSectionAddress->VirtualAddress = newVirtualAddress;
	addSectionAddress->Misc.VirtualSize = newVirtualSize;
	addSectionAddress->SizeOfRawData = newSizeOfRawData;
	addSectionAddress->PointerToRawData = newPointerAddress;

	
	// 导出表信息
	DWORD exportAddressFOA = RvaToFov(newPehear.dataHeaders[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress, newBuffer);
	PIMAGE_EXPORT_DIRECTORY pEXPORT = (PIMAGE_EXPORT_DIRECTORY)(newBuffer + exportAddressFOA);
	cout << "导出表位置：" <<  hex << exportAddressFOA << endl;

	// 新节起始位置
	DWORD* SectionAddress = (DWORD*)((BYTE*)newBuffer + readfileResu);
	cout << "节表起始位置FOA: " << hex << readfileResu << endl;

	// 复制 AddressOfFunctions 到新节
	memcpy(SectionAddress, newBuffer + RvaToFov(pEXPORT->AddressOfFunctions, newBuffer), pEXPORT->NumberOfFunctions * sizeof(DWORD));

	// 复制 AddressOfNameOrdinals 到新节
	WORD* Ordinals = (WORD*)((BYTE*)SectionAddress + pEXPORT->NumberOfFunctions * sizeof(DWORD));
	memcpy(Ordinals, newBuffer + RvaToFov(pEXPORT->AddressOfNameOrdinals, newBuffer), pEXPORT->NumberOfFunctions * sizeof(WORD));

	// 复制 AddressOfNames 到新节
	DWORD* NamesAddress = (DWORD*)((BYTE*)Ordinals + pEXPORT->NumberOfFunctions * sizeof(WORD));	
	memcpy(NamesAddress, newBuffer + RvaToFov(pEXPORT->AddressOfNames, newBuffer), pEXPORT->NumberOfFunctions * sizeof(DWORD));

	// 复制名字到新节
	BYTE* nameData = (BYTE*)((BYTE*)NamesAddress + pEXPORT->NumberOfFunctions * sizeof(DWORD));
	for (size_t i = 0; i < pEXPORT->NumberOfFunctions; i++)
	{
		const char* functionName = (const char*)((BYTE*)newBuffer + RvaToFov(NamesAddress[i], newBuffer));
		size_t nameLength = strlen(functionName) + 1;
		memcpy(nameData, functionName, nameLength);
		// 直接更新NamesAddress地址
		NamesAddress[i] = FoaToRva((DWORD)(nameData - newBuffer), newBuffer);
		nameData += nameLength;
	}

	// 复制模块名称到新节中
	DWORD* Module = (DWORD*)(nameData + pEXPORT->NumberOfFunctions);
	memcpy(Module, newBuffer + RvaToFov(pEXPORT->Name, newBuffer), pEXPORT->NumberOfFunctions * sizeof(DWORD));

	// 复制导出表到新节
	PIMAGE_EXPORT_DIRECTORY NewExportAddress = (PIMAGE_EXPORT_DIRECTORY)(DWORD*)((BYTE*)nameData + pEXPORT->NumberOfFunctions * sizeof(DWORD));
	memcpy(NewExportAddress, pEXPORT, sizeof(IMAGE_EXPORT_DIRECTORY));

	// 更新 IMAGE_EXPORT+DIRECTORY 字段
	NewExportAddress->AddressOfFunctions = FoaToRva((DWORD)((BYTE*)SectionAddress - newBuffer), newBuffer);
	NewExportAddress->AddressOfNameOrdinals = FoaToRva((DWORD)((BYTE*)Ordinals - newBuffer), newBuffer);
	NewExportAddress->AddressOfNames = FoaToRva((DWORD)((BYTE*)NamesAddress - newBuffer), newBuffer);
	NewExportAddress->Name = FoaToRva((DWORD)((BYTE*)Module - newBuffer), newBuffer);

	// 更新 IMAGE_DIRECTORY_ENTRY_EXPORT 指向新表中的新结构
	newPehear.dataHeaders[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress = newVirtualAddress + (DWORD)((BYTE*)NewExportAddress - (BYTE*)SectionAddress);

	// 写入文件
	if (!FwritrFile(newBuffer, readfileResu + addByte, filePath))
	{
		free(newBuffer);
		return false;
	}
	free(newBuffer);

	return true;
}

bool MoveBaseReloc(LPCSTR filePath)
{
	if (!filePath)	return false;
	LPVOID fileBuffer = nullptr;
	DWORD readfileResu = ReadFile(filePath, &fileBuffer);
	if (!readfileResu) return false;

	PEHeaders peheader{};
	if (!GetPeheadersInfo(fileBuffer, peheader))
	{
		free(fileBuffer);
		return false;
	}

	// 新增节
	// 追加字节
	DWORD addByte = 0x1000;
	unsigned char* newBuffer = (unsigned char*)realloc(fileBuffer, readfileResu + addByte);
	if (!newBuffer)
	{
		free(fileBuffer);
		return false;
	}
	memset(newBuffer + readfileResu, 0, addByte);

	// 更新PE信息
	PEHeaders newPehear{};

	if (!GetPeheadersInfo(newBuffer, newPehear))return false;
	if (newPehear.optionalHeader == nullptr) return false;

	// 新节表插入位置
	PIMAGE_SECTION_HEADER addSectionAddress = (PIMAGE_SECTION_HEADER)((BYTE*)newPehear.sectionHeader + (IMAGE_SIZEOF_SECTION_HEADER * newPehear.fileHeader->NumberOfSections));

	// 复制数据到新节表
	memcpy_s((char*)addSectionAddress, IMAGE_SIZEOF_SECTION_HEADER, (char*)newPehear.sectionHeader, IMAGE_SIZEOF_SECTION_HEADER);
	addSectionAddress->Characteristics |= IMAGE_SCN_MEM_EXECUTE;
	const char name[] = ".GaGa!!!";
	memcpy_s((char*)addSectionAddress->Name, IMAGE_SIZEOF_SHORT_NAME, (char*)name, sizeof(name) - 1);

	// 修正节表字段
	DWORD newVirtualAddress = 0;
	DWORD newPointerAddress = 0;
	DWORD newSizeOfRawData = addByte;
	DWORD newVirtualSize = addByte;

	// 获取新节表字段
	for (size_t i = 0; i < newPehear.fileHeader->NumberOfSections; i++)
	{
		if (i == newPehear.fileHeader->NumberOfSections - 1)
		{
			newPointerAddress = newPehear.sectionHeader[i].PointerToRawData + newPehear.sectionHeader[i].SizeOfRawData;
			DWORD align = (((newPehear.sectionHeader[i].Misc.VirtualSize + newPehear.sectionHeader[i].VirtualAddress) + 0xFFF) / 0x1000) * 0x1000;
			newVirtualAddress = align;
			/*cout << hex << align << endl;
			cout << newPehear.sectionHeader[i].Name << endl;*/
			break;
		}
	}

	// 修正PE字段
	newPehear.fileHeader->NumberOfSections += 0x1;
	newPehear.optionalHeader->SizeOfImage += 0x1000;

	// 修正新节表字段
	addSectionAddress->VirtualAddress = newVirtualAddress;
	addSectionAddress->Misc.VirtualSize = newVirtualSize;
	addSectionAddress->SizeOfRawData = newSizeOfRawData;
	addSectionAddress->PointerToRawData = newPointerAddress;

	// ****************************************************************

	// 重定位表位置
	PIMAGE_BASE_RELOCATION pBASE_RELOCATION = (PIMAGE_BASE_RELOCATION)&newPehear.optionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
	// 数据位置
	PIMAGE_BASE_RELOCATION pBASE_DATA = (PIMAGE_BASE_RELOCATION)((BYTE*)newBuffer + RvaToFov(pBASE_RELOCATION->VirtualAddress, newBuffer));
	DWORD* dataAddress = (DWORD*)((BYTE*)newBuffer + RvaToFov(pBASE_RELOCATION->VirtualAddress, newBuffer));
	// 数量
	DWORD DataNumer = 0;

	while (pBASE_DATA->VirtualAddress != 0)
	{
		DataNumer += pBASE_DATA->SizeOfBlock;
		pBASE_DATA = (PIMAGE_BASE_RELOCATION)((BYTE*)pBASE_DATA + pBASE_DATA->SizeOfBlock);
	}

	// 新节表位置
	DWORD* newSectionAddress = (DWORD*)((BYTE*)newBuffer + readfileResu);
	// 复制数据
	memcpy(newSectionAddress, dataAddress, DataNumer);
	
	
	// 修正目录项使其指向新的重定表位置
	DWORD newAddress = FoaToRva(((BYTE*)newSectionAddress - newBuffer), newBuffer);
	newPehear.optionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress = newAddress;
	newPehear.optionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size = DataNumer;

	// 修改基地址，修正重定位表
	newPehear.optionalHeader->ImageBase += 0x1000;
	PIMAGE_BASE_RELOCATION pNewBASE_DATA = (PIMAGE_BASE_RELOCATION)((BYTE*)newBuffer + RvaToFov(pBASE_RELOCATION->VirtualAddress, newBuffer));

	while (pNewBASE_DATA->VirtualAddress != 0)
	{
		// 数据数量
		DWORD BaseNumer = (pNewBASE_DATA->SizeOfBlock - (sizeof(DWORD) * 2)) / sizeof(WORD);
		// 起始位置
		WORD* BaseAddress1 = (WORD*)((BYTE*)pNewBASE_DATA + sizeof(DWORD) * 2);

		for (size_t i = 0; i < BaseNumer; i++)
		{
			WORD typeOffset = *BaseAddress1;
			DWORD type = (typeOffset & 0xF000) >> 12;
			DWORD offset = typeOffset & 0x0FFF;

			if (type != IMAGE_REL_BASED_HIGHLOW)
			{
				break;
			}

			// 计算需要修正的相对虚拟地址
			DWORD rvaToPatch = pNewBASE_DATA->VirtualAddress + offset;

			// 将RVA转换为文件偏移（FOA）
			DWORD fileOffsetToPatch = RvaToFov(rvaToPatch, newBuffer);

			// 获取要修正的内存位置
			DWORD* patchLocation = (DWORD*)((BYTE*)newBuffer + fileOffsetToPatch);
			*patchLocation += 0x1000;
			BaseAddress1++;
		}

		// 移动到下一个重定位表
		pNewBASE_DATA = (PIMAGE_BASE_RELOCATION)((BYTE*)pNewBASE_DATA + pNewBASE_DATA->SizeOfBlock);
	}

	if (!FwritrFile(newBuffer, (readfileResu + addByte), filePath))
	{
		return false;
	}

	free(newBuffer);
}

