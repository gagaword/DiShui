#include <iostream>
#include<Windows.h>
#include"Process.h"
#include <tchar.h>
#include <tlhelp32.h>
#include "ProcessHandle.h"

int main()
{
    //getProcessHandle();
    //getALLProcessName();


    // 测试函数
    std::wstring processName = L"WeChat.exe";
    DWORD processID = FindProcessIDByName(processName);

    if (processID != 0)
    {
        std::wcout << L"Found process: " << processName << L" with PID: " << processID << std::endl;
    }
    else
    {
        std::wcout << L"Process not found: " << processName << std::endl;
    }

    ProcessHandle processHandle(processID);

    if (processHandle.GetHandle() != NULL) {
        std::cout << "Process handle for PID " << processID << " obtained successfully." << std::endl;
        // 使用processHandle.GetHandle()在多处使用句柄
        processHandle.PrintHandleValue();
    }
    else {
        std::cerr << "Failed to get process handle for PID " << processID << std::endl;
    }


    return 0;
}