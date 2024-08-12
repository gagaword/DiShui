#pragma once
#include<windows.h>
#include<iostream>
#include<string.h>

#define DEBUG 0
#define LOG(FUN_NAME,VALUES) if ((DEBUG) == 1){std::cout << FUN_NAME << "---->" << std::hex << VALUES << std::endl;}
#define IMAGE_SIZEOF_Signature 0x4

/*!
 * @brief 该方法读取一个文件的数据
 * @param FilePath 文件的路径
 * @param FileData 保存文件数据的指针的指针
 * @return 返回0表示读取失败，返回字节表示读取成功。
*/
DWORD ReadFile22(IN LPCSTR FilePath, OUT LPVOID* FileData);

/*!
 * @brief 该方法复制FileBufer到ImageBuffer中
 * @param FileBuffer FileBuffer指针
 * @param ImageBuffer ImageBuffer指针
 * @return 
*/
DWORD CopyFileBufferToImageBuffer(IN LPVOID FileBuffer, OUT LPVOID * ImageBuffer);

/*!
 * @brief 该方法复制ImageBuffer到NewBuffer中，使得NewBuffer数据存盘且可运行
 * @param ImageBuffer ImageBuffer指针
 * @param NewBuffer	  NweBuffer指针
 * @return  返回0表示复制失败，否者返回复制的大小
*/
DWORD CopyImageBufferToNewBuffer(IN LPVOID ImageBuffer, OUT LPVOID* NewBuffer);

/*!
 * @brief 该方法将内存中的数据写入到文件中
 * @param pMemBuffer 内存数据
 * @param size 要复制的大小
 * @param lpszFile 要保存文件的路径
 * @return 返回0表示写入失败，否则返回复制的大小
*/
DWORD MemeryTOFile(IN LPVOID pMemBuffer, IN size_t size, OUT LPSTR lpszFile);


/*!
 * @brief 该方法把内存偏移转为文件偏移
 * @param FileBuffer FileBuffer指针
 * @param dwRva 内存偏移
 * @return 返回0表示转换失败，否则返回文件偏移FOA
*/
DWORD RvaTOFileOffset(IN LPVOID FileBuffer, IN DWORD dwRva);