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

//int sub(int x, int y)
//{
//    return x - y;
//}