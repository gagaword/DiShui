#pragma once
#include<iostream>

// 打开文件
void *openFile(FILE** _Stream,const char* FileName,const char* Mode);


// 获取文件大小
size_t GetFileSize(FILE* stream);

// 读取文件数据
void *ReadFileData(long FileSize, size_t elemensize, FILE *stream);