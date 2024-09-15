#pragma once

extern "C" __declspec(dllexport) int __stdcall add(int x, int y);

extern "C" __declspec(dllexport) int __stdcall sub(int x, int y);

extern "C" __declspec(dllexport) int __stdcall imul(int x, int y);

// 不发布
void  inti();

// 不发布
void  del();

extern "C" __declspec(dllexport) void ExportFunction();

// 隐藏函数名称定义函数类型与普通CPP文件一致
//int sub(int x, int y );
