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

void inti()
{
    MessageBoxA(NULL, "Hello,World", "TIP", MB_OK);
}

void del()
{
    MessageBoxA(NULL,"Bay","TIP", MB_OK);
}

void ExportFunction()
{
    MessageBoxA(NULL,"ExportFunction", "TIP", MB_OK);
}


//int sub(int x, int y)
//{
//    return x - y;
//}