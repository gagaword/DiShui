#include "my_head.h"

#define FALSE 0
#define TRUE 1
#define DEBUG 1
#define PRINTFLOGSTRING(VALUES) if((DEBUG) == 1){printf("%s\n",VALUES);}
#define PRINTFLOG(VALUES) if((DEBUG) == 1){printf("VALUES--->%x\n",VALUES);}


const char PE[] = {
	0x4D,0x5A
};

void *openFile(FILE** _Stream, const char* FileName, const char* Mode)
{
	if (_Stream == NULL || FileName == NULL || Mode ==NULL)
	{
		PRINTFLOGSTRING("参数错误");
		return FALSE;
	}
	errno_t result = fopen_s(_Stream, FileName, Mode);
	if (result != 0)
	{
		PRINTFLOGSTRING("打开失败");
		return FALSE;
	}
	PRINTFLOG(*_Stream);
}


size_t GetFileSize(FILE* stream)
{
	if (stream == NULL)
	{
		PRINTFLOGSTRING("参数错误");
		return FALSE;
	}
	long FileSize = 0;
	fseek(stream,0,SEEK_END);
	FileSize = ftell(stream);
	fseek(stream, 0, SEEK_SET);
	return FileSize;
}

void* ReadFileData(long FileSize, size_t elemensize, FILE* stream)
{
	if (FileSize == NULL || elemensize == NULL || stream == NULL)
	{
		PRINTFLOGSTRING("参数错误");
		return FALSE;
	}
	// 分配内存
	unsigned char* FileSpaceSize = (unsigned char*)malloc(FileSize);
	// 初始化内存
	memset(FileSpaceSize, 0, FileSize);

	size_t result = fread_s(FileSpaceSize, FileSize, elemensize, FileSize, stream);
	if (result != FileSize)
	{
		PRINTFLOG(result);
		PRINTFLOGSTRING("读取可能失败，请检查");
		free(FileSpaceSize);
		return FALSE;
	}
	else
	{
		PRINTFLOG(result);
	}
	return FileSpaceSize;
}

int CheckPE(unsigned char* buuferFile)
{
	if (memcmp(buuferFile, PE, sizeof(PE)) == 0) {
		PRINTFLOGSTRING("是PE文件");
		return TRUE;
	}
	else {
		PRINTFLOGSTRING("不是PE文件");
		return FALSE;
	}
	return 0;
}
