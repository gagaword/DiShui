#pragma once
#include<iostream>
#include<Windows.h>

// 打开文件
int openFile(FILE** _Stream,const char* FileName,const char* Mode);

// 获取文件大小
size_t GetFileSize(FILE* stream);

// 读取文件数据
void *ReadFileData(long FileSize, size_t elemensize, FILE *stream);

// 检查是否PE
int CheckPE(unsigned char* buuferFile);

// 读取文件
LPVOID ReadFile(const char* FilePath);