#include "day27.h"
#include "My_day23.h"

using namespace std;

bool PritfImport(LPCSTR filePath)
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

	PEHeaders peheaders{};
	if (!GetPeheadersInfo(fileBuffer, peheaders))
	{
		return false;
	}
	if (peheaders.optionalHeader == nullptr)
	{
		return false;
	}

	PIMAGE_IMPORT_DESCRIPTOR importAddress = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD*)((BYTE*)fileBuffer + RvaToFov(peheaders.optionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress, fileBuffer)));

	cout << "************************************INT****************************************" << endl;

	// OriginalFirstThunk----INT
	while (importAddress->OriginalFirstThunk != 0)
	{
		DWORD number = 0;
		cout << "************************************DLL Info****************************************************" << endl;
		char* dllName = (char*)((BYTE*)fileBuffer + RvaToFov(importAddress->Name, fileBuffer));
		cout << "DLL Name: " << (char*)((BYTE*)fileBuffer + RvaToFov(importAddress->Name, fileBuffer)) << endl;
		cout << hex << uppercase << "Original First Thunk: " << importAddress->OriginalFirstThunk << endl;
		cout << hex << uppercase << "Original First Thunk FOA: " << RvaToFov(importAddress->OriginalFirstThunk, fileBuffer) << endl;
		cout << hex << uppercase << "TimeDateStamp: " << importAddress->TimeDateStamp << endl;
		cout << hex << uppercase << "Forwarder Chain: " << importAddress->ForwarderChain << endl;
		cout << hex << uppercase << "Name(RVA): " << importAddress->Name << endl;

		DWORD* FunctionAddress = (DWORD*)((BYTE*)fileBuffer + RvaToFov(importAddress->OriginalFirstThunk, fileBuffer));
		while (*FunctionAddress != 0)
		{
			if ((*FunctionAddress & IMAGE_ORDINAL_FLAG))
			{
				cout << hex << "Thunk RVA: " << importAddress->OriginalFirstThunk << "  ";
				cout << hex << "Thunk FOA: " << RvaToFov(importAddress->OriginalFirstThunk, fileBuffer) << "  ";
				cout << hex << "Thunk Value: " << uppercase << *FunctionAddress << "  ";
				DWORD ordinal = *FunctionAddress & ~IMAGE_ORDINAL_FLAG32;
				cout << "Ordinal: " << hex << uppercase << ordinal << "   ";
				cout << "API Name: " << "NULL" << endl;
			}
			else {
				PIMAGE_IMPORT_BY_NAME pIMPORT_BY_NAME = (PIMAGE_IMPORT_BY_NAME)(DWORD*)((BYTE*)fileBuffer + RvaToFov(*FunctionAddress, fileBuffer));
				cout << hex << "Thunk RVA: " << importAddress->OriginalFirstThunk << "   ";
				cout << hex << "Thunk FOA: " << RvaToFov(importAddress->OriginalFirstThunk, fileBuffer) << "   ";
				cout << hex << "Thunk Value: " << uppercase << *FunctionAddress << "   ";
				cout << hex << "Hint: " << uppercase << pIMPORT_BY_NAME->Hint << "   ";
				cout << "API Name: " << pIMPORT_BY_NAME->Name << endl;
			}
			FunctionAddress++;
			importAddress->OriginalFirstThunk += sizeof(DWORD);
			number++;
		}
		cout << "Number: " << dec << number << endl;
		importAddress++;
	}

	cout << "*********************************IAT****************************************" << endl;
	// FirstThunk-----IAT
	importAddress = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD*)((BYTE*)fileBuffer + RvaToFov(peheaders.optionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress, fileBuffer)));
	while (importAddress->FirstThunk != 0)
	{
		cout << "*********************************DLL Info****************************************************" << endl;
		cout << "DLL Name: " << (char*)((BYTE*)fileBuffer + RvaToFov(importAddress->Name, fileBuffer)) << endl;
		cout << hex << uppercase << "First Thunk: " << importAddress->FirstThunk << endl;
		cout << hex << uppercase << "First Thunk FOA: " << RvaToFov(importAddress->FirstThunk, fileBuffer) << endl;
		cout << hex << uppercase << "TimeDateStamp: " << importAddress->TimeDateStamp << endl;
		cout << hex << uppercase << "Forwarder Chain: " << importAddress->ForwarderChain << endl;
		cout << hex << uppercase << "Name(RVA): " << importAddress->Name << endl;

		DWORD* FunctionAddress = (DWORD*)((BYTE*)fileBuffer + RvaToFov(importAddress->FirstThunk, fileBuffer));
		while (*FunctionAddress != 0)
		{
			if (*FunctionAddress & IMAGE_ORDINAL_FLAG32)
			{
				cout << hex << "Thunk RVA: " << importAddress->FirstThunk << "   ";
				cout << hex << "Thunk FOA: " << RvaToFov(importAddress->FirstThunk, fileBuffer) << "   ";
				cout << hex << "Thunk Value: " << uppercase << *FunctionAddress << "   ";
				DWORD ordinal = *FunctionAddress & ~IMAGE_ORDINAL_FLAG32;
				cout << "Ordinal: " << hex << uppercase << ordinal << "    ";
				cout << "API Name: " << "NULL" << endl;
			}
			else {
				PIMAGE_IMPORT_BY_NAME pIMPORT_BY_NAME = (PIMAGE_IMPORT_BY_NAME)(DWORD*)((BYTE*)fileBuffer + RvaToFov(*FunctionAddress, fileBuffer));
				cout << hex << "Thunk RVA: " << importAddress->FirstThunk << "   ";
				cout << hex << "Thunk FOA: " << RvaToFov(importAddress->FirstThunk, fileBuffer) << "   ";
				cout << hex << "Thunk Value: " << uppercase << *FunctionAddress << "   ";
				cout << hex << "Hint: " << uppercase << pIMPORT_BY_NAME->Hint << "   ";
				cout << "API Name: " << pIMPORT_BY_NAME->Name << endl;
			}
			FunctionAddress++;
			importAddress->FirstThunk += sizeof(DWORD);
		}
		importAddress++;
	}
	
	return false;
}

bool PrintfImportdescriptor(LPCSTR filePath)
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

	PEHeaders peheaders{};
	if (!GetPeheadersInfo(fileBuffer, peheaders))
	{
		return false;
	}
	if (peheaders.optionalHeader == nullptr)
	{
		return false;
	}

	PIMAGE_IMPORT_DESCRIPTOR importAddress = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD*)((BYTE*)fileBuffer + RvaToFov(peheaders.optionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress, fileBuffer)));
	bool isdescriprot = 0;
	while (importAddress->OriginalFirstThunk != 0 && importAddress->FirstThunk != 0)
	{
		char* dllName = (char*)((BYTE*)fileBuffer + RvaToFov(importAddress->Name, fileBuffer));
		DWORD timeDate = importAddress->TimeDateStamp;
		if (timeDate == 0)
		{
			isdescriprot = 1;
		}
		importAddress->OriginalFirstThunk += sizeof(DWORD);
		importAddress->FirstThunk += sizeof(DWORD);
		importAddress++;
	}	
	if (isdescriprot)
	{
		cout << "该程序没有绑定导入表" << endl;
	}
	return false;
}
