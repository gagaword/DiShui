// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "DLLTest.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
        // dll加载的时候执行这个
    case DLL_PROCESS_ATTACH:
        inti();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        // dll卸载的时候执行这个
    case DLL_PROCESS_DETACH:
        del();
        break;
    }
    return TRUE;
}

