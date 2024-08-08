#include"day22.h"

#define FILEPATH "D:\\PE\\PETool 1.0.0.5.exe"

int main()
{
    LPVOID FileBuffer = NULL;
    DWORD fileSize = ReadFile(FILEPATH, &FileBuffer);

    if (fileSize == 0 || FileBuffer == NULL) {
        std::cerr << "读取文件错误" << std::endl;
        return 1;
    }

    std::cout << "读取字节数: " << fileSize << std::endl;

    LPVOID ImageBuffer = NULL;
    DWORD imageBufferSize = CopyFileBufferToImageBuffer(FileBuffer, &ImageBuffer);
    if (imageBufferSize == 0 || ImageBuffer == NULL) {
        std::cerr << "复制文件到ImageBuffer失败" << std::endl;
        free(FileBuffer);
        return 1;
    }

    std::cout << "复制成功, 复制字节数: " << imageBufferSize << std::endl;

    LPVOID NewBuffer = NULL;
    DWORD newBufferSize = CopyImageBufferToNewBuffer(ImageBuffer, &NewBuffer);
    if (newBufferSize == 0 || NewBuffer == NULL) {
        std::cerr << "复制ImageBuffer到NewBuffer失败" << std::endl;
        free(FileBuffer);
        free(ImageBuffer);
        return 1;
    }

    std::cout << "NewBuffer复制字节数: " << newBufferSize << std::endl;

    DWORD writeResult = MemeryTOFile(NewBuffer, newBufferSize, (LPSTR)"C:\\Users\\GaGa\\Desktop\\SSSSS_new.exe");
    if (writeResult == 0) {
        std::cerr << "写入文件失败" << std::endl;
        free(FileBuffer);
        free(ImageBuffer);
        free(NewBuffer);
        return 1;
    }

    std::cout << "写入成功, 字节数: " << writeResult << std::endl;

    DWORD fileOffset = RvaTOFileOffset(FileBuffer, 0x17000);
    if (fileOffset == 0) {
        std::cerr << "计算文件偏移失败" << std::endl;
    }
    else {
        std::cout << "文件偏移: " << std::hex << fileOffset << std::endl;
    }

    free(FileBuffer);
    free(ImageBuffer);
    free(NewBuffer);

    return 0;
}

//int main()
//{
//	LPVOID FileBuffer = NULL;
//	DWORD result = ReadFile(FILEPATH, &FileBuffer);	
//
//	unsigned char* temp = (unsigned char*)FileBuffer;
//	if (result == 0)
//	{
//		std::cout << "读取错误" << std::endl;
//	}
//	std::cout << "读取字节---->" << result << std::endl;
//
//	LPVOID ImageBuffer = NULL;
//	DWORD CopyResult = CopyFileBufferToImageBuffer(FileBuffer, &ImageBuffer);
//	if (CopyResult == 0)
//	{
//		std::cout << "复制失败" << std::endl;
//		return 0;
//	}
//	unsigned char* temp2 = (unsigned char*)ImageBuffer;
//
//	std::cout << "复制成功--->"  <<  CopyResult << std::endl;
//
//	
//
//	LPVOID NewBuffer = NULL;
//	DWORD CpoyNewBuffer = CopyImageBufferToNewBuffer(ImageBuffer,&NewBuffer);
//	if (CpoyNewBuffer == 0)
//	{
//		std::cout << "复制失败" << std::endl;
//		return 0;
//
//	}
//
//	std::cout << "复制字节--->" << CpoyNewBuffer << std::endl;
//	
//	unsigned char* temp3 = (unsigned char*)NewBuffer;
//
//	DWORD MemeryResult =  MemeryTOFile(NewBuffer, 42496, (LPSTR)"C:\\Users\\GaGa\\Desktop\\new.exe");
//
//	if (MemeryResult == 0)
//	{
//		printf("写入失败\n");
//		return 0;
//	}
//	std::cout << "写入成功---->" << MemeryResult << std::endl;
//
//	DWORD FOAResult = RvaTOFileOffset(FileBuffer, 0x17000);
//	std::cout << "文件偏移-->" << std::hex <<  FOAResult << std::endl;
//	return 0;
//}


