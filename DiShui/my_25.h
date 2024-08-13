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
};

/*!
 * @brief 该方法用于获取各个头.
 * @param Buffer 具有PE格式的数据流.
 * @param peheader PEheaders结构
 * @return 0表示失败,否则返回1.
*/
bool GetPeheadersInfo(LPVOID Buffer, PEHeaders peheader);

/*!
 * @brief 该方法打印提供文件的导出表数据.
 * @param filePath 文件路径.
 * @return 0表示失败，否则返回1.
*/
bool PrintDataExport(LPCSTR filePath);
