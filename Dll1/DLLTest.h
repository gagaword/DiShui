#pragma once


extern "C" __declspec(dllexport) int __stdcall add(int x, int y);

extern "C" __declspec(dllexport) int __stdcall sub(int x, int y);
// 隐藏函数名称定义函数类型与普通CPP文件一致
//int sub(int x, int y );
