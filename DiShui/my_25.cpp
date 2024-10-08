#include "my_25.h"
#include "My_day23.h"

#define DEBUG 0

bool GetPeheadersInfo(LPVOID Buffer, PEHeaders &peheader)
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
	if (peheader.ntHeaders->Signature != IMAGE_NT_SIGNATURE)
	{
		perror("NT头签名错误");
		return false;
	}

	// FILE Header
	peheader.fileHeader = &peheader.ntHeaders->FileHeader;
	if (DEBUG)
	{
		std::cout << "FILE HEADER-----> " << std::hex << peheader.fileHeader->Machine << std::endl;
	}

	// OPTION Header
	peheader.optionalHeader = &peheader.ntHeaders->OptionalHeader;
	if (DEBUG)
	{
		std::cout << "OPTION HEADER-----> " << std::hex << peheader.optionalHeader->Magic << std::endl;
	}

	// SECTIOn Header
	peheader.sectionHeader = (PIMAGE_SECTION_HEADER)((BYTE*)peheader.optionalHeader + peheader.fileHeader->SizeOfOptionalHeader);
	if (DEBUG)
	{
		std::cout << "SECTION HEADER-----> " << std::hex << peheader.sectionHeader->SizeOfRawData << std::endl;
	}

	// DATA HEADER
	peheader.dataHeaders = (PIMAGE_DATA_DIRECTORY)&peheader.optionalHeader->DataDirectory;
	
	// 导出表
	peheader.exportSection = (PIMAGE_EXPORT_DIRECTORY)&peheader.optionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	if (DEBUG)
	{
		std::cout << "Export VirtualAddress------> " << peheader.dataHeaders[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress << std::endl;
	}

	// 导入表
	peheader.importSection = (PIMAGE_IMPORT_DESCRIPTOR)&peheader.optionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	if (DEBUG)
	{
		std::cout << "Impotr VirtualAddress------> " << peheader.dataHeaders[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress << std::endl;
	}

	// 重定位表
	peheader.baseSection = (PIMAGE_BASE_RELOCATION)&peheader.optionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
	if (DEBUG)
	{
		std::cout << "BASE VirtualAddress------> " << peheader.dataHeaders[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress << std::endl;
	}

	// 绑定导入表
	peheader.descriptorSection = (PIMAGE_BOUND_IMPORT_DESCRIPTOR)&peheader.optionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress;
	if (DEBUG)
	{
		std::cout << "DESCRIPTOR VirtualAddress------> " << peheader.dataHeaders[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress << std::endl;
	}
	return true;
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
	PEHeaders peheader;
	bool GetPEInfo = GetPeheadersInfo(fileBuffer, peheader);
	if (!GetPEInfo)
	{
		return false;
	}
	// 导出表位置
	// 通过PEHeaders获取导出表位置
	DWORD VA = 0;
	VA = peheader.dataHeaders->VirtualAddress;
	if (DEBUG)
	{
		std::cout << std::hex << "PEHeaders Data_Exports Address------>" << VA << std::endl;
	}
	// 通过OPTIONAL获取到导出表
	//std::cout << "Optional Data_Exports Address------->"  <<  peheader.optionalHeader->DataDirectory[0].VirtualAddress << std::endl;
	// RVA转FOV
	DWORD FOV = RvaToFoa(VA, fileBuffer);
	if (DEBUG)
	{
		std::cout << std::hex << "FOV-----> " << FOV << std::endl;
	}
	// 导出表
	PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)fileBuffer + FOV);

	std::cout << "**********************Export Info*******************************" << std::endl;
	std::cout << "模块名称地址-------------> " << std::hex << std::uppercase << pExport->Name << std::endl;
	std::cout << "基址---------------------> " << pExport->Base << std::endl;
	std::cout << "函数数量-----------------> " << pExport->NumberOfFunctions << std::endl;
	std::cout << "函数名称数量-------------> " << pExport->NumberOfNames << std::endl;
	std::cout << "函数地址-----------------> " << std::hex << std::uppercase << pExport->AddressOfFunctions << std::endl;
	std::cout << "函数名称地址-------------> " << std::hex << std::uppercase << pExport->AddressOfNames << std::endl;
	std::cout << "函数名称序号地址---------> " << std::hex << std::uppercase << pExport->AddressOfNameOrdinals << std::endl;

	DWORD AddressNamesFov = RvaToFoa(pExport->AddressOfNames, fileBuffer);
	DWORD NameFOV = RvaToFoa(pExport->Name, fileBuffer);
	DWORD AddressFunction = RvaToFoa(pExport->AddressOfFunctions, fileBuffer);
	DWORD AddressNameOrdinals = RvaToFoa(pExport->AddressOfNameOrdinals, fileBuffer);

	std::cout << "名称:文件偏移----------> " << std::hex << std::uppercase<< NameFOV << std::endl;
	std::cout << "函数名称:文件偏移------> " << std::hex << std::uppercase << AddressNamesFov << std::endl;
	std::cout << "函数地址:文件偏移------> " << std::hex << std::uppercase << AddressFunction << std::endl;
	std::cout << "函数序号:文件偏移------> " << std::hex << std::uppercase << AddressNameOrdinals << std::endl;
	std::cout << "*****************************Function Info*************************************" << std::endl;

	printf("文件名称---->%s\n", ((char*)fileBuffer + NameFOV));
	// 获取信息
	DWORD* pNameArray = (DWORD*)((BYTE*)fileBuffer + AddressNamesFov);
	DWORD* pFunctionAddress = (DWORD*)((BYTE*)fileBuffer + AddressFunction);
	WORD* pNameOrdinals = (WORD*)((BYTE*)fileBuffer + AddressNameOrdinals);

	for (DWORD i = 0; i < pExport->NumberOfNames; i++) {
		std::cout << "********** Function:" << i << " ******************" << std::endl;
		DWORD functionNameFov = RvaToFoa(pNameArray[i], fileBuffer);
		WORD functionOrdinal = pNameOrdinals[i];
		DWORD functionAddresss = pFunctionAddress[i];
		DWORD functionFOV = RvaToFoa(pFunctionAddress[i], fileBuffer);
		std::cout << "Function FOV: " << std::hex << std::uppercase <<  functionFOV << std::endl;
		std::cout << "Function RVA: " << std::hex << std::uppercase << functionAddresss << std::endl;
		printf("Function Name: %s\nFunction Ordinal: %d\n", (char*)fileBuffer + functionNameFov, functionOrdinal);
	}
	return true;
}

DWORD RvaToFoa(DWORD RVA, LPVOID fileBuffer)
{
	if (RVA == NULL || fileBuffer == nullptr)return 0;
	

	PEHeaders peheader;
	if (!GetPeheadersInfo(fileBuffer, peheader))return 0;
	
	
	// 确定RVA在哪个节
	size_t numberSection = peheader.fileHeader->NumberOfSections;
	DWORD VA = 0, PR = 0;
	DWORD FOA = 0;
	size_t is = 0;
	for (size_t i = 0; i < numberSection; i++)
	{
		if (RVA >= peheader.sectionHeader[i].VirtualAddress && RVA <= peheader.sectionHeader[i].VirtualAddress + peheader.sectionHeader[i].Misc.VirtualSize)
		{
			VA = peheader.sectionHeader[i].VirtualAddress;
			PR = peheader.sectionHeader[i].PointerToRawData;
			is = 1;
			break;
		}
	}

	if (DEBUG)
	{
		std::cout << "VA------> " << VA << "\nPA------> " << PR << std::endl;
	}

	// 计算FOA
	if (is)
	{
		FOA = (RVA - VA) + PR;
		return FOA;
	}
	// 当RVA小于任何节表，RVA在头部信息中，直接返回RVA
	else
	{
		return RVA;
	}
	return 0;
}

DWORD FunctionSerialToInfo(DWORD serial, LPCSTR filePath)
{
	if (filePath == NULL)
	{
		return 0;
	}

	LPVOID fileBuffer = NULL;
	DWORD readfileResu = ReadFile(filePath, &fileBuffer);
	if (!readfileResu)
	{
		return 0;
	}

	PEHeaders peheader;
	if (!GetPeheadersInfo(fileBuffer, peheader))
	{
		return 0;
	}

	// 导出表信息
	DWORD pExport = peheader.dataHeaders[0].VirtualAddress;
	DWORD pExportFile = (DWORD)((BYTE*)fileBuffer + RvaToFoa(pExport, fileBuffer));
	PIMAGE_EXPORT_DIRECTORY pExport_DI = (PIMAGE_EXPORT_DIRECTORY)pExportFile;

	// 序列号
	WORD* pserialFOV = (WORD*)((BYTE*)fileBuffer + RvaToFoa(pExport_DI->AddressOfNameOrdinals, fileBuffer));

	DWORD* pAddressFunc = (DWORD*)((BYTE*)fileBuffer + RvaToFoa(pExport_DI->AddressOfFunctions, fileBuffer));

	// 函数名称
	DWORD* pFuncNameFOV_1 = (DWORD*)((BYTE*)fileBuffer + RvaToFoa(pExport_DI->AddressOfNames, fileBuffer));

	// 函数内存地址
	DWORD pFuncAddress = 0;
	// 函数文件地址
	DWORD pFuncAddressFile = 0;
	// 函数名称
	LPCSTR funcName = 0;

	for (size_t i = 0; i < pExport_DI->NumberOfFunctions; i++)
	{
		WORD serial_this = pserialFOV[i];
		if (serial_this == serial)
		{
			pFuncAddress = pAddressFunc[i];
			pFuncAddressFile = RvaToFoa(pAddressFunc[i], fileBuffer);
			DWORD name = RvaToFoa(pFuncNameFOV_1[i], fileBuffer);
			funcName = (char*)(BYTE*)fileBuffer + name;
		}
	}

	printf("%x\n", pFuncAddress);
	printf("%x\n", pFuncAddressFile);
	printf("%s\n", funcName);

}

DWORD algin(DWORD value, DWORD alginValue)
{
	if (alginValue == 0)
	{
		return value;
	}
	return (value + alginValue - 1) / alginValue * alginValue;
}

DWORD FoaToRva(DWORD Foa, LPVOID fileBuffer)
{
	if (Foa == NULL || fileBuffer == nullptr)
	{
		return 0;
	}
	PEHeaders peheader;
	if (!GetPeheadersInfo(fileBuffer, peheader))
	{
		return 0;
	}
	for (size_t i = 0; i < peheader.fileHeader->NumberOfSections; i++)
	{
		if (Foa >= peheader.sectionHeader[i].PointerToRawData && Foa < peheader.sectionHeader[i].SizeOfRawData + peheader.sectionHeader[i].PointerToRawData)
		{
			DWORD pointerData = peheader.sectionHeader[i].PointerToRawData;
			DWORD VA = peheader.sectionHeader[i].VirtualAddress;
			return Foa - pointerData + VA;
		}
	}
	return 0;
}

DWORD FunctionNameToInfo(LPCSTR funName, LPCSTR filePath)
{
	if (funName == nullptr || filePath == nullptr)
	{
		return 0;
	}

	LPVOID fileBuffer = nullptr;

	DWORD readfileResult = ReadFile(filePath, &fileBuffer);
	if (!readfileResult)
	{
		return 0;
	}

	PEHeaders peheader;
	if (!GetPeheadersInfo(fileBuffer, peheader))
	{
		return 0;
	}

	DWORD exportAddress = peheader.dataHeaders[0].VirtualAddress;
	exportAddress = RvaToFoa(exportAddress,fileBuffer);

	PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)(DWORD*)((BYTE*)fileBuffer + exportAddress);

	DWORD addressNamesFOV = RvaToFoa(pExport->AddressOfNames, fileBuffer);
	DWORD* arrayName = ((DWORD*)((BYTE*)fileBuffer + addressNamesFOV));

	DWORD funcAddress = RvaToFoa(pExport->AddressOfFunctions, fileBuffer);
	DWORD* funcAddressFileFov = (DWORD*)((BYTE*)fileBuffer + funcAddress);

	// 函数文件偏移
	DWORD funcFileFov = 0;
	// 函数内存地址
	DWORD funcMemeryAddress = 0;

	DWORD funcFileAddressSerial = (DWORD)RvaToFoa(pExport->AddressOfNameOrdinals, fileBuffer);
	WORD* funcFileSerial = (WORD*)((BYTE*)fileBuffer + funcFileAddressSerial);

	// 函数序列号
	DWORD funcSerial = 0;

	for (size_t i = 0; i < pExport->NumberOfFunctions; i++)
	{
		DWORD NamesAddress =  RvaToFoa(arrayName[i], fileBuffer);
		char* _func_name = (char*)((BYTE*)fileBuffer + NamesAddress);
		if (!strcmp(_func_name, funName))
		{
			std::cout << _func_name << std::endl;
			funcMemeryAddress = funcAddressFileFov[i];
			funcFileFov = RvaToFoa(funcAddressFileFov[i],fileBuffer);
			funcSerial = funcFileSerial[i];
			break;
		}
		else
		{
			std::cerr << "未找到函数" << std::endl;
			return 0;
		}
	}

	std::cout << "********************funcTion Info********************" << std::endl;
	std::cout << std::hex << funcMemeryAddress << std::endl;
	std::cout << std::hex << funcFileFov << std::endl;
	std::cout << std::hex << funcSerial << std::endl;

	return 1;
}
