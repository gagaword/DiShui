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

/*!
 * @brief 扩大最后一个节并添加ShellCode
 * @param filePath 文件路径
 * @return 0表示失败，否则反非零
*/
bool ExpandSection(IN LPBYTE filePath);

/*!
 * @brief 打印数据目录
 * @param filePath 文件路径
 * @return 0表示失败，1表示成功
*/
bool ImageData(IN LPBYTE filePath);

/*!
 * @brief 合并节
 * @param filePath  文件路径
 * @return 0成功，1失败
*/
bool MergeSection(IN LPBYTE filePath);