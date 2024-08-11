#include<iostream>
#include<Windows.h>
#include"My_day23.h"

#define FilePath "D:\\DriveLoading\\KmdManager.exe"

int main()
{
	
	/*LPVOID FileBuffer = NULL;
	DWORD readFileSize = ReadFile(FilePath, &FileBuffer);
	free((void*)FileBuffer);*/

	//DWORD AddResult = AddShellCode((LPVOID)FilePath);
	
	//bool result = AddShellCodeInData((LPVOID)FilePath); 

	//bool rdata = AddShellCodeRdata((LPVOID)FilePath);

	//bool addSection = AddSection((LPBYTE)FilePath);

	//bool expandSection = ExpandSection((LPBYTE)FilePath);

	  ImageData((LPBYTE)FilePath);
	
	return 0;
}


