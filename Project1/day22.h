#pragma once

#include<windows.h>
#include<iostream>
#include<string.h>

/// <summary>
/// 该方法读取一个文件的数据
/// </summary>
/// <param name="FilePath">文件的路径</param>
/// <param name="FileBuffer">保存文件数据的指针的指针</param>
/// <returns>
/// 返回0表示读取失败，返回字节表示读取成功。
/// </returns>
DWORD ReadFile(IN const char* FilePath, OUT LPVOID* FileData);


/// <summary>
/// 该方法复制FileBufer到ImageBuffer中
/// </summary>
/// <param name="FileBuffer">FileBuffer</param>
/// <param name="ImageBuffer">ImageBuffer</param>
/// <returns>
/// 返回0表示失败，否则返回复制的大小
/// </returns>
DWORD CopyFileBufferToImageBuffer(IN LPVOID FileBuffer, OUT LPVOID* ImageBuffer);