#pragma once
#include <windows.h>
#include <iostream>

#ifndef PROCESSHANDLE_H
#define PROCESSHANDLE_H

class ProcessHandle {
 private:
DWORD processID;
HANDLE hProcess;

public:
    ProcessHandle(DWORD pid);
    ~ProcessHandle();
    HANDLE GetHandle() const;
    void PrintHandleValue() const;
};

#endif // PROCESSHANDLE_H


