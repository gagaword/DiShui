#include "dll/DLLTest.h"
#include "my_25.h"

#define filePath "D:\\Repos\\gagaword\\MyProject\\Debug\\Dll1.dll"

int main()
{
	//PrintDataExport(filePath);

	LPCSTR name = "_add@8";
	//FunctionNameToInfo(name, filePath);

	FunctionSerialToInfo(0, filePath);
	return 0;
}

