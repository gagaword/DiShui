#include<iostream>
#include"day28.h"

// ÒþÊ½µ÷ÓÃ
extern "C" __declspec(dllexport) void ExportFunction();
int main()
{
	ExportFunction();
	return 0;
}