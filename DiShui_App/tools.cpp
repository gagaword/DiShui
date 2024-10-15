#include "tools.h"

// GPT版本
void __cdecl OutputDebugStringF(const char* format, ...)
{
	char strBuffer[4096];  // 使用栈上缓冲区
	va_list vlArgs;

	va_start(vlArgs, format);
	vsnprintf(strBuffer, sizeof(strBuffer), format, vlArgs);  // 更安全的格式化函数
	va_end(vlArgs);

	// 添加换行符
	strcat_s(strBuffer, sizeof(strBuffer), "\n");

	// 输出到调试窗口
	OutputDebugStringA(strBuffer);
}

// 滴水版本
/*
void __cdecl OutputDebugStringF(const char* format, ...)
{
	va_list vlArgs;
	char* strBuffer = (char*)GlobalAlloc(GPTR, 4096);

	va_start(vlArgs, format);
	_vsnprintf(strBuffer, 4096 - 1, format, vlArgs);
	va_end(vlArgs);
	strcat(strBuffer, "\n");
	OutputDebugStringA(strBuffer);
	GlobalFree(strBuffer);
	return;
}
*/

