#pragma once

#include<Windows.h>
#include<iostream>
#include "my_25.h"



/*!
 * @brief 该方法打印提供PE文件的重定位表
 * @param filePath 文件路径
 * @return 0表示失败，否则打印数据并返回1.
*/
bool PrintBaseloc(LPCSTR filePath);

/**
 * @brief 该方法把PE文件中的导出表移动到新的节中
 * @param filePath 文件路径
 * @return 0表示失败，否则返回1.
*/
bool MoveExport(LPCSTR filePath);

/**
 * @brief 该方法移动导出表到新节
 * @param filePath 文件路径
 * @return 0表示失败，否则返回1.
*/
bool MoveBaseReloc(LPCSTR filePath);



bool AddSectionAndExportTable(LPCSTR filePath);