#include "my_head.h"

#define FALSE 0
#define TRUE 1
#define DEBUG 1
#define PRINTFLOGSTRING(FunName,VALUES) if((DEBUG) == 1){printf("%s-------->%s\n", FunName, VALUES);}
#define PRINTFLOG(FunName,VALUES) if((DEBUG) == 1){printf("%s-------->%x\n", FunName, VALUES);}

const char PE[] = {
	0x4D,0x5A
};

int openFile(FILE** _Stream, const char* FileName, const char* Mode)
{
	if (_Stream == NULL || FileName == NULL || Mode ==NULL)
	{
		PRINTFLOGSTRING(__func__, "参数错误");
		return FALSE;
	}
	errno_t result = fopen_s(_Stream, FileName, Mode);
	if (result != 0)
	{
		PRINTFLOGSTRING(__func__,"打开失败");
		return FALSE;
	}
	PRINTFLOG(__func__, *(_Stream));
	return TRUE;
}

size_t GetFileSize(FILE* stream)
{
	if (stream == NULL)
	{
		PRINTFLOGSTRING(__func__, "参数错误");
		return FALSE;
	}
	long FileSize = 0;
	fseek(stream,0,SEEK_END);
	FileSize = ftell(stream);
	fseek(stream, 0, SEEK_SET);
	PRINTFLOG(__func__, FileSize);
	return FileSize;
}

void* ReadFileData(long FileSize, size_t elemensize, FILE* stream)
{
	if (FileSize == NULL || elemensize == NULL || stream == NULL)
	{
		PRINTFLOGSTRING(__FUNCSIG__, "参数错误");
		return FALSE;
	}
	// 分配内存
	unsigned char* FileSpaceSize = (unsigned char*)malloc(FileSize);
	if (FileSpaceSize == NULL)
	{
		PRINTFLOGSTRING(__func__, "分配内存错误");
	}

	// 初始化内存
	memset(FileSpaceSize, 0, FileSize);

	size_t result = fread_s(FileSpaceSize, FileSize, elemensize, FileSize, stream);
	if (result != FileSize)
	{
		PRINTFLOG(__func__, result);
		PRINTFLOGSTRING(__func__, "读取可能失败，请检查");
		free(FileSpaceSize);
		return FALSE;
	}
	else
	{
		PRINTFLOGSTRING(__func__, "读取成功!!!!");
		PRINTFLOG(__func__, result);
		return FileSpaceSize;
	}
}

int CheckPE(unsigned char* buuferFile)
{
	if (memcmp(buuferFile, PE, sizeof(PE)) == 0) {
		PRINTFLOGSTRING(__func__, "是PE文件");
		return TRUE;
	}
	else {
		PRINTFLOGSTRING(__func__, "不是PE文件");
		return FALSE;
	}
	return 0;
}

LPVOID ReadFile(const char* FilePath)
{
	// 判断参数
	if (FilePath == NULL)
	{
		std::cout << "参数错误" << std::endl;
		return NULL;
	}

	// 打开文件
	FILE* file = nullptr;
	errno_t isFile = fopen_s(&file, FilePath, "rb");
	if (isFile != 0)
	{
		std::cout << "打开文件失败" << std::endl;
		return NULL;
	}

	// 分配内存
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	unsigned char* FileSpace = (unsigned char*)malloc(fileSize);
	if (FileSpace == NULL)
	{
		std::cout << "分配内存错误" << std::endl;
		fclose(file);
		return NULL;
	}
	memset(FileSpace, 0, fileSize);
	// 读取数据
	size_t ReadFileSize = fread_s(FileSpace, fileSize, 1, fileSize, file);
	if (ReadFileSize != fileSize)
	{
		std::cout << "读取可能失败" << std::endl;
		fclose(file);
		free(FileSpace);
		return NULL;
	}
	fclose(file);
	std::cout << "文件字节:" << fileSize << std::endl;
	return FileSpace;
}