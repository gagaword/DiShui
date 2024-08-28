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


	// OriginalFirstThunk
	while (importAddress->OriginalFirstThunk != 0)
	{
		cout << "DLL Name: " << (char*)((BYTE*)fileBuffer + RvaToFov(importAddress->Name, fileBuffer)) << endl;
		cout << hex << uppercase << "Original First Thunk: " << importAddress->OriginalFirstThunk << endl;
		cout << hex << uppercase << "Original First Thunk FOA: " << RvaToFov(importAddress->OriginalFirstThunk, fileBuffer) << endl;
		cout << hex << uppercase << "Time/Date Stamp: " << importAddress->TimeDateStamp << endl;
		cout << hex << uppercase << "Forwarder Chain: " << importAddress->ForwarderChain << endl;
		cout << hex << uppercase << "Name(RVA): " << importAddress->Name << endl;

		DWORD* FunctionAddress = (DWORD*)((BYTE*)fileBuffer + RvaToFov(importAddress->OriginalFirstThunk, fileBuffer));
		while (*FunctionAddress != 0)
		{
			PIMAGE_IMPORT_BY_NAME pIMPORT_BY_NAME = (PIMAGE_IMPORT_BY_NAME)(DWORD*)((BYTE*)fileBuffer + RvaToFov(*FunctionAddress, fileBuffer));
			cout << hex << "Thunk RVA: " << importAddress->OriginalFirstThunk << "  ";
			cout << hex << "Thunk FOA: " << RvaToFov(importAddress->OriginalFirstThunk, fileBuffer) << "  ";
			cout << hex << "Thunk Value: " << uppercase << *FunctionAddress << "  ";
			cout << hex << "Hint: " << uppercase << pIMPORT_BY_NAME->Hint << "  ";
			cout << "API Name: " << pIMPORT_BY_NAME->Name << endl;
			FunctionAddress++;
			importAddress->OriginalFirstThunk += sizeof(DWORD);
		}
		importAddress++;
	}


	// FirstThunk
	/*while (importAddress->OriginalFirstThunk != 0)
	{
		cout << "DLL Name: " << (char*)((BYTE*)fileBuffer + RvaToFov(importAddress->Name, fileBuffer)) << endl;
		cout << hex << uppercase << "Original First Thunk: " << importAddress->OriginalFirstThunk << endl;
		cout << hex << uppercase << "Original First Thunk FOA: " << RvaToFov(importAddress->OriginalFirstThunk, fileBuffer) << endl;
		cout << hex << uppercase << "Time/Date Stamp: " << importAddress->TimeDateStamp << endl;
		cout << hex << uppercase << "Forwarder Chain: " << importAddress->ForwarderChain << endl;
		cout << hex << uppercase << "Name(RVA): " << importAddress->Name << endl;

		DWORD* FunctionAddress = (DWORD*)((BYTE*)fileBuffer + RvaToFov(importAddress->FirstThunk, fileBuffer));
		while (*FunctionAddress != 0)
		{
			PIMAGE_IMPORT_BY_NAME pIMPORT_BY_NAME = (PIMAGE_IMPORT_BY_NAME)(DWORD*)((BYTE*)fileBuffer + RvaToFov(*FunctionAddress, fileBuffer));
			cout << hex << "Thunk RVA: " << importAddress->FirstThunk << "  ";
			cout << hex << "Thunk FOA: " << RvaToFov(importAddress->FirstThunk, fileBuffer) << "  ";
			cout << hex << "Thunk Value: " << uppercase << *FunctionAddress << "  ";
			cout << hex << "Hint: " << uppercase << pIMPORT_BY_NAME->Hint << "  ";
			cout << "API Name: " << pIMPORT_BY_NAME->Name << endl;
			FunctionAddress++;
			importAddress->FirstThunk += sizeof(DWORD);
		}
		importAddress++;
	}*/
	
	return false;
}
