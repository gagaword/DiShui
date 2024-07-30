#include "ProcessHandle.h"
#include <windows.h>
#include <iostream>
#include <iomanip>

// 函数句柄
ProcessHandle::ProcessHandle(DWORD pid) : processID(pid), hProcess(NULL) {
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
    if (hProcess == NULL) {
        std::cerr << "Failed to open process with PID " << processID << std::endl;
    }
}

// 析构函数
ProcessHandle::~ProcessHandle() {
    if (hProcess != NULL) {
        CloseHandle(hProcess);
    }
}

// 获取句柄
HANDLE ProcessHandle::GetHandle() const {
    return hProcess;
}

// 打印句柄值
void ProcessHandle::PrintHandleValue() const {
    if (hProcess != NULL) {
        std::cout << "Handle value: 0x" << std::hex  << hProcess << std::endl;
    }
    else {
        std::cerr << "Handle is NULL." << std::endl;
    }
}