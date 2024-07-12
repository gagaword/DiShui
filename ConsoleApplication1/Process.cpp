#include<iostream>

#include "Process.h"
#include<Windows.h>
#include<tchar.h>
#include<Psapi.h>
#include <tlhelp32.h>

HANDLE getProcessHandle(DWORD dwAccess, BOOL isHandle, DWORD processPid)
{
	if (dwAccess == NULL || isHandle == NULL || processPid == NULL)
	{
		return (HANDLE)-1;
	}
	HANDLE processHandle = OpenProcess(dwAccess, isHandle, processPid);
	if (processHandle == NULL)
	{
		std::cout << GetLastError() << std::endl;
	}
	else {
		return processHandle;
	}
}

void getALLProcessName()
{
    // 定义进程快照的句柄
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) return;

    // 定义PROCESSENTRY32结构体，用于存储进程信息
    PROCESSENTRY32 pe32;
    // 确保结构体大小正确，以便兼容32位和64位系统
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // 遍历系统中的所有进程
    if (Process32First(hProcessSnap, &pe32))
    {
        do
        {
            // 打印进程ID和进程名称
            std::wcout << L"Process ID: " << pe32.th32ProcessID << L", Name: " << pe32.szExeFile << std::endl;

        } while (Process32Next(hProcessSnap, &pe32));
    }

    // 关闭进程快照句柄
    CloseHandle(hProcessSnap);
}

// 定义一个函数，接受进程名称，返回匹配进程的PID
DWORD FindProcessIDByName(const std::wstring& processName)
{
    // 定义进程快照的句柄
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) return 0;

    // 定义PROCESSENTRY32结构体，用于存储进程信息
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // 遍历系统中的所有进程
    if (Process32First(hProcessSnap, &pe32))
    {
        do
        {
            // 检查进程名称是否匹配
            if (_wcsicmp(pe32.szExeFile, processName.c_str()) == 0)
            {
                // 匹配进程名称，返回进程ID
                CloseHandle(hProcessSnap); // 关闭进程快照句柄
                return pe32.th32ProcessID;
            }
        } while (Process32Next(hProcessSnap, &pe32));
    }

    // 没有找到匹配的进程，关闭句柄并返回0
    CloseHandle(hProcessSnap);
    return 0;
}




