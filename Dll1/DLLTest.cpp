#include "pch.h"
#include "DLLTest.h"

int __stdcall add(int x, int y) {
    return x + y;
}

int __stdcall sub(int x, int y)
{
    return x - y;
}

int __stdcall imul(int x, int y)
{
    return x * y;
}

int inti()
{
    MessageBoxA(NULL, "Hello,World", "TIP", MB_OK);
    return 0;
}

int del()
{
    MessageBoxA(NULL,"Bay","TIP", MB_OK);
    return 0;
}

void ExportFunction()
{
    MessageBoxA(NULL,"ExportFunction", "TIP", MB_OK);
}


//int sub(int x, int y)
//{
//    return x - y;
//}