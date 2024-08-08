#include<iostream>
#include<Windows.h>

#include"My_day23.h"

#define FilePath "D:\\PE\\Exeinfo.exe"

int main()
{
	LPVOID FileBuffer = NULL;

	DWORD readFileSize = ReadFile(FilePath, &FileBuffer);
	if (readFileSize == 0)
	{
		return 0;
	}
	DWORD AddResult = AddShellCode(FileBuffer);
	if (AddResult == 0)
	{
		return 0;
	}
	DWORD writeResult = FwritrFile(FileBuffer, readFileSize, FilePath);
	return 0;
}