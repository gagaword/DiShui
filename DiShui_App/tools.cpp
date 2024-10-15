#include "tools.h"

// GPT�汾
void __cdecl OutputDebugStringF(const char* format, ...)
{
	char strBuffer[4096];  // ʹ��ջ�ϻ�����
	va_list vlArgs;

	va_start(vlArgs, format);
	vsnprintf(strBuffer, sizeof(strBuffer), format, vlArgs);  // ����ȫ�ĸ�ʽ������
	va_end(vlArgs);

	// ��ӻ��з�
	strcat_s(strBuffer, sizeof(strBuffer), "\n");

	// ��������Դ���
	OutputDebugStringA(strBuffer);
}

// ��ˮ�汾
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

