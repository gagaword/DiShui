#include<iostream>
#include"day28.h"
#include<Windows.h>

#define filePath "C:\\Users\\GaGa\\Desktop\\jinshan2007_CrackMe\\CrackMe.exe"
#define DllName "Dll1.dll"
#define FuncName "ExportFunction"

// 隐式调用
//extern "C" __declspec(dllexport) void ExportFunction();

// 声明函数指针
typedef void (*LPTest)();

int main()
{
	MovImportAndInjectionDll(filePath, DllName, FuncName);

	//ExportFunction();
	//LPTest function;
	//// 加载dll
	//HMODULE dllAddress = LoadLibrary(L"D:\\Repos\\gagaword\\MyProject\\DiShui\\dll\\Dll1.dll");
	//if (dllAddress == 0)	return 0;
	//// 获取函数地址
	//function = (LPTest)GetProcAddress(dllAddress, "ExportFunction");
	//// 调用函数
	//function();
	//// 释放dll
	//FreeLibrary(dllAddress);
	return 0;
}