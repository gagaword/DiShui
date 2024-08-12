#include <iostream>
#include <Windows.h>
#include "My_day23.h"

#include "Lib/framework.h"

#define FilePath "D:\\PE\\LordPE.exe"

//extern "C" __declspec(dllimport) int __stdcall add(int x, int y);

typedef int(__cdecl *pAdd)(int, int);

int main()
{
	//printf("%d\n", add(1, 3));

	HINSTANCE hMoudle = LoadLibrary(L"Dll1.dll");
	if (hMoudle == NULL)
	{
		std::cout << "´íÎó" << std::endl;
		return 0;
	}

	pAdd padd = (pAdd)GetProcAddress(hMoudle, (LPCSTR)1);
	if (padd == nullptr)
	{
		std::cout << padd << std::endl;
		return 0;
	}
	std::cout << "º¯ÊýµØÖ·----> " << padd << std::endl;
	printf("%d\n", padd(2, 1));

	/*LPVOID FileBuffer = NULL;
	DWORD readFileSize = ReadFile(FilePath, &FileBuffer);
	free((void*)FileBuffer);*/

	//DWORD AddResult = AddShellCode(FilePath);
	
	//bool result = AddShellCodeInData(FilePath); 

	//bool rdata = AddShellCodeRdata(FilePath);

	//bool addSection = AddSection(FilePath);

	//bool expandSection = ExpandSection(FilePath);

	//ImageData(FilePath);

	//MergeSection(FilePath);

	//printf("%d\n", Add(1, 2));

	
	return 0;
}





