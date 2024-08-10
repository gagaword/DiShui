#pragma once

#include<Windows.h>
#include<iostream>
#include<string.h>


DWORD ReadFile(IN const char* filepath, OUT LPVOID* fileBuffer);

DWORD AddShellCode(IN LPVOID filePath);

DWORD FwritrFile(IN LPVOID buffer, IN size_t size, OUT const char* filePath);

bool AddShellCodeInData(IN LPVOID filePath);

bool AddShellCodeRdata(IN LPVOID filePath);

bool AddSection(IN LPBYTE filePath);