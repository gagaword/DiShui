#pragma once

#include<Windows.h>
#include<iostream>
#include<string.h>


DWORD ReadFile(IN const char* filepath, OUT LPVOID* fileBuffer);

DWORD AddShellCode(LPVOID fileBuffer);

DWORD FwritrFile(LPVOID buffer, size_t size, const char* filePath);