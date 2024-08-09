#pragma once

#include<windows.h>
#include<iostream>
#include<string.h>

#define DEBUG 0
#define LOG(FUN_NAME,VALUES) if ((DEBUG) == 1){std::cout << FUN_NAME << "---->" << std::hex << VALUES << std::endl;}
#define IMAGE_SIZEOF_Signature 0x4

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
/// <param name="FileBuffer">FileBuffer指针</param>
/// <param name="ImageBuffer">ImageBuffer指针</param>
/// <returns>
/// 返回0表示失败，否则返回复制的大小
/// </returns>
DWORD CopyFileBufferToImageBuffer(IN LPVOID FileBuffer, OUT LPVOID* ImageBuffer);


/// <summary>
/// 该方法复制ImageBuffer到NewBuffer中，使得NewBuffer数据存盘且可运行
/// </summary>
/// <param name="ImageBuffer">ImageBuffer指针</param>
/// <param name="NewBuffer">NweBuffer指针</param>
/// <returns>
/// 返回0表示复制失败，否者返回复制的大小
/// </returns>
DWORD CopyImageBufferToNewBuffer(IN LPVOID ImageBuffer, OUT LPVOID* NewBuffer);


/// <summary>
/// 该方法将内存中的数据写入到文件中
/// </summary>
/// <param name="pMemBuffer">内存数据</param>
/// <param name="size">要复制的大小</param>
/// <param name="lpszFile">要保存文件的路径</param>
/// <returns>
/// 返回0表示写入失败，否则返回复制的大小
/// </returns>
DWORD MemeryTOFile(IN LPVOID pMemBuffer, IN size_t size, OUT LPSTR lpszFile);


/// <summary>
/// 该方法把内存偏移转为文件偏移
/// </summary>
/// <param name="FileBuffer">FileBuffer指针</param>
/// <param name="dwRva">内存偏移</param>
/// <returns>
/// 返回0表示转换失败，否则返回文件偏移FOA
/// </returns>
DWORD RvaTOFileOffset(IN LPVOID FileBuffer, IN DWORD dwRva);