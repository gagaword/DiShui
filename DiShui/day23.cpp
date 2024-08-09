#include<iostream>
#include<Windows.h>
#include"My_day23.h"

#define FilePath "C:\\Users\\GaGa\\Desktop\\PETool 1.0.0.5.exe"

int main()
{
	/*LPVOID FileBuffer = NULL;
	DWORD readFileSize = ReadFile(FilePath, &FileBuffer);
	free((void*)FileBuffer);*/

	DWORD AddResult = AddShellCode((LPVOID)FilePath);
	
	//bool result = AddShellCodeInData((LPVOID)FilePath); 

	//bool rdata = AddShellCodeRdata((LPVOID)FilePath);

	return 0;
}