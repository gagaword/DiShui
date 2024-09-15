#include<iostream>
#include"day28.h"
#include<Windows.h>
// 隐式调用
//extern "C" __declspec(dllexport) void ExportFunction();

typedef void (*LPTest)();
int main()
{
	//ExportFunction();
	LPTest function;

	// 加载dll
	HMODULE dllAddress = LoadLibrary(L"D:\\Repos\\gagaword\\MyProject\\DiShui\\dll\\Dll1.dll");

	// 获取函数地址
	function = (LPTest)GetProcAddress(dllAddress, "ExportFunction");

	// 调用函数
	function();

	// 释放dll
	FreeLibrary(dllAddress);
	return 0;
}