#pragma once

#include <Windows.h>
#include <iostream>

// PE信息结构
struct PEHeaders {
	PIMAGE_DOS_HEADER dosHeader;
	PIMAGE_NT_HEADERS32 ntHeaders;
	PIMAGE_FILE_HEADER fileHeader;
	PIMAGE_OPTIONAL_HEADER32 optionalHeader;
	PIMAGE_SECTION_HEADER sectionHeader;
	PIMAGE_DATA_DIRECTORY dataHeaders;
};


/*!
 * @brief 该方法用于获取各个头.
 * @param Buffer 具有PE格式的数据流.
 * @param peheader PEheaders结构
 * @return 0表示失败,否则返回1.
*/
bool GetPeheadersInfo(LPVOID Buffer, PEHeaders &peheader);

/*!
 * @brief 该方法打印提供文件的导出表数据.
 * @param filePath 文件路径.
 * @return 0表示失败，否则返回1.
*/
bool PrintDataExport(LPCSTR filePath);

/*!
 * @brief 该方法根据提供的函数序号打印出函数信息,函数序号从0开始
 * @param serial 函数序号
 * @param filePath 文件路径
 * @return 0表示失败，否则返回1.
*/
DWORD FunctionSerialToInfo(DWORD serial, LPCSTR filePath);

/*!
 * @brief 该方法通过方法名称获取方法信息
 * @param funName 方法名称
 * @return 0表示失败，否则返回方法地址.
*/
DWORD FunctionNameToInfo(LPCSTR funName, LPCSTR filePath);

/*!
 * @brief 该方法把RVA转为FOV
 * @param RVA RVA
 * @param fileBuffer 具有PE文件特征的数据流
 * @return 0表示失败，否则返FOV
*/
DWORD RvaToFov(DWORD RVA, LPVOID fileBuffer);

/*!
 * @brief 该方法将 value 以 alginValue 对齐
 * @param value 目标值
 * @param alginValue 对齐值
 * @return 0失败，否则返回对齐后的值
*/
DWORD algin(DWORD value, DWORD alginValue);
