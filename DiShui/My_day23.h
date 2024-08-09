#pragma once

#include<Windows.h>
#include<iostream>
#include<string.h>


DWORD ReadFile(IN const char* filepath, OUT LPVOID* fileBuffer);

DWORD AddShellCode(IN LPVOID filePath);

DWORD FwritrFile(IN LPVOID buffer, IN size_t size, OUT const char* filePath);