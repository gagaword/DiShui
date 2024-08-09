#include<iostream>
#include<Windows.h>
#include"My_day23.h"

#define FilePath "C:\\Users\\GaGa\\Desktop\\InstDrv.exe"

int main()
{
	/*LPVOID FileBuffer = NULL;

	DWORD readFileSize = ReadFile(FilePath, &FileBuffer);
	if (readFileSize == 0)
	{
		return 0;
	}*/

	DWORD AddResult = AddShellCode((LPVOID)FilePath);
	if (AddResult == 0)
	{
		return 0;
	}

	return 0;
}