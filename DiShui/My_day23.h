#pragma once

#include<Windows.h>
#include<iostream>
#include<string.h>

/*!
 * @brief 该方法读取文件数据.
 * @param filepath 文件路径.
 * @param fileBuffer 存放文件数据指针的指针.
 * @return 0表示失败，否者返回读取到的字节数.
*/
DWORD ReadFile(IN LPCSTR filepath, OUT LPVOID* fileBuffer);

/*!
 * @brief 该方法在.text节添加ShellCode.
 * @param filePath 文件路径.
 * @return 0表示失败，否则返回1.
*/
DWORD AddShellCode(IN LPCSTR filePath);

/*!
 * @brief 该方法把数据流写入文件中.
 * @param buffer 数据流.
 * @param size   写入字节数.
 * @param filePath 文件路径.
 * @return 0表示失败，否者返回1.
*/
DWORD FwritrFile(IN LPVOID buffer, IN size_t size, OUT LPCSTR filePath);

/*!
 * @brief 该方法在.data节添加ShellCode.
 * @param filePath 文件路径.
 * @return 0表示失败，否则返回1.
*/
bool AddShellCodeInData(IN LPCSTR filePath);

/*!
 * @brief 该方法在.rdata节添加ShellCode.
 * @param filePath 文件路径.
 * @return 0表示失败，否则返回1.
*/
bool AddShellCodeRdata(IN LPCSTR filePath);

/*!
 * @brief 该方法新增一个节并添加ShellCode.
 * @param filePath 文件路径.
 * @return 0表示失败，否则返回1.
*/
bool AddSection(IN LPCSTR filePath);

/*!
 * @brief 扩大最后一个节并添加ShellCode.
 * @param filePath 文件路径.
 * @return 0表示失败，否则返回1.
*/
bool ExpandSection(IN LPCSTR filePath);

/*!
 * @brief 打印数据目录.
 * @param filePath 文件路径.
 * @return 0表示失败，否则返回1.
*/
bool ImageData(IN LPCSTR filePath);

/*!
 * @brief 合并节.
 * @param filePath  文件路径.
 * @return 0表示失败，否则返回1.
*/
bool MergeSection(IN LPCSTR filePath);

//DWORD RVA_FOV(DWORD RVA, LPVOID fileBuffer);