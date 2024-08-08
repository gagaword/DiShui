#include "My_day23.h"

#define MessageBoxA_Address 0x758B0F40

DWORD ReadFile(IN const char* filepath, OUT LPVOID* fileBuffer)
{
	if (filepath == NULL)
	{
		std::cout << "参数错误" << std::endl;
		return 0;
	}
	std::cout << "打开文件:" << filepath << std::endl;

	FILE* file = NULL;
	errno_t fileResult = fopen_s(&file, filepath, "rb");
	if (fileResult != 0)
	{
		std::cout << "打开文件失败" << std::endl;
		return 0;
	}

	// 获取文件大小
	fseek(file,0,SEEK_END);
	long fileSize = ftell(file);
	fseek(file,0,SEEK_SET);

	std::cout << "文件字节:" << fileSize << std::endl;

	// 分配空间大小
	*fileBuffer = malloc(fileSize);
	if (*fileBuffer == NULL)
	{
		std::cout << "空间分配错误" << std::endl;
		fclose(file);
		return 0;
	}
	memset(*fileBuffer, 0, fileSize);

	// 读取文件
	size_t freadResult = fread_s(*fileBuffer, fileSize, 1, fileSize, file);
	if (freadResult != fileSize)
	{
		std::cout << "读取文件错误" << std::endl;
		return 0;
	}
	std::cout << "读取字节:" << freadResult << std::endl;
	return freadResult;
}

DWORD AddShellCode(LPVOID fileBuffer)
{
	// PE headers
	PIMAGE_DOS_HEADER pDOS_HEADER = NULL;
	PIMAGE_NT_HEADERS32 pNT_HEADER = NULL;
	PIMAGE_FILE_HEADER pFILE_HEADER = NULL;
	PIMAGE_OPTIONAL_HEADER32 pOPTIONAL_HEADER = NULL;
	PIMAGE_SECTION_HEADER pSECTION_HEADER = NULL;

	pDOS_HEADER = (PIMAGE_DOS_HEADER)((BYTE*)fileBuffer);
	pNT_HEADER = (PIMAGE_NT_HEADERS32)((BYTE*)fileBuffer + pDOS_HEADER->e_lfanew);
	pFILE_HEADER = (PIMAGE_FILE_HEADER)(((BYTE*)fileBuffer + pDOS_HEADER->e_lfanew) + 0x04);
	pOPTIONAL_HEADER = (PIMAGE_OPTIONAL_HEADER32)(((BYTE*)pFILE_HEADER + IMAGE_SIZEOF_FILE_HEADER));
	pSECTION_HEADER = IMAGE_FIRST_SECTION(pNT_HEADER);
	
	size_t NumberSections = pFILE_HEADER->NumberOfSections;
	size_t FileOffset = 0;
	size_t MemeryOffset = 0;
	size_t MiscSize = 0;

	// 修改Characteristics字段
	pNT_HEADER->FileHeader.Characteristics |= IMAGE_FILE_RELOCS_STRIPPED;
	
	for (int i = 0; i < NumberSections; i++)
	{
		//printf("%s\n", pSECTION_HEADER[i].Name);
		if (strcmp((const char*)pSECTION_HEADER[i].Name,".text") == 0)
		{
			FileOffset = pSECTION_HEADER[i]. PointerToRawData;
			MemeryOffset = pSECTION_HEADER[i].VirtualAddress;
			MiscSize = pSECTION_HEADER[i].Misc.VirtualSize;
			break;
		}
	}
	if (FileOffset == 0)
	{
		std::cerr << "未找到.text" << std::endl;
		return 0;
	}
	std::cout << "FileOffset---->" << std::hex << FileOffset << std::endl;
	std::cout << "MemeryOffset---->" << std::hex << MemeryOffset << std::endl;
	std::cout << "MiscSize---->" << std::hex << MiscSize << std::endl;

	// OEP地址
	DWORD EntryPoin = pOPTIONAL_HEADER->AddressOfEntryPoint + pOPTIONAL_HEADER->ImageBase;
	

	// 硬编码
	char ShellCode[20] = { 0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00, 0xE8, 0x00, 0x00, 0x00, 0x00, 0xE9, 0x00, 0x00, 0x00, 0x00 };
	int ShellLength = sizeof(ShellCode);

	// 计算 E8 跳转地址和 E9 跳转地址
	DWORD E8FileOffset = ((FileOffset + MiscSize) + 0x8) + 0x5;
	DWORD E9FileOffset = E8FileOffset + 0x5;
	DWORD E8CALL = MessageBoxA_Address - ((((E8FileOffset - FileOffset) + MemeryOffset)) + pOPTIONAL_HEADER->ImageBase);
	DWORD E9JMP = EntryPoin - ((((E9FileOffset - FileOffset) + MemeryOffset)) + pOPTIONAL_HEADER->ImageBase);
	std::cout << "E8FileOffset---->" << std::hex << E8FileOffset << std::endl;
	std::cout << "E9FileOffset---->" << std::hex << E9FileOffset << std::endl;
	std::cout << "EntryPoin------->" << std::hex << EntryPoin << std::endl;
	printf("E8CALL----> %X \nE9JMP-----> %X\n", E8CALL, E9JMP);
	

	// 将E8CALL的每个字节逐个添加到ShellCode的末尾
	ShellCode[9] = (E8CALL >> 0) & 0xFF; 
	ShellCode[10] = (E8CALL >> 8) & 0xFF;
	ShellCode[11] = (E8CALL >> 16) & 0xFF;
	ShellCode[12] = (E8CALL >> 24) & 0xFF;
	

	// 将E9JMp的每个字节逐个添加到ShellCode的末尾
	ShellCode[14] = (E9JMP >> 0) & 0xFF; 
	ShellCode[15] = (E9JMP >> 8) & 0xFF; 
	ShellCode[16] = (E9JMP >> 16) & 0xFF;
	ShellCode[17] = (E9JMP >> 24) & 0xFF;


	// 打印修改后的ShellCode以查看结果
	/*printf("Modified ShellCode: ");
	for (int i = 0; i < ShellLength; i++) {
		printf("%02X ", (unsigned char)ShellCode[i]);
	}
	printf("\n");*/
	
	// 确定 ShellCode 插入位置
	BYTE* address = ((BYTE*)fileBuffer + (FileOffset + MiscSize));

	// 插入ShellCode。
	for (int i = 0; i < ShellLength; i++)
	{
		address[i] = ShellCode[i];
	}

	/*for (int i = 0; i < ShellLength; i++)
	{
		*(((BYTE*)fileBuffer + (FileOffset + MiscSize)) + i) = ShellCode[i];
	}*/

	

	// 打印插入后的FileBuffer
	std::cout << "*****************************************S插入硬编码*****************************************" << std::endl;
	for (size_t i = 0; i < ShellLength; i++)
	{
		printf("%02X ", *(((BYTE*)fileBuffer + (FileOffset + MiscSize)) + i));
	}
	std::cout << "\n*****************************************E插入硬编码*****************************************" << std::endl;

	// 修改OEP,OEP使用的是相对地址
	DWORD newOEP = (MiscSize + FileOffset) - FileOffset + MemeryOffset;
	std::cout << "newOEP----->" << std::hex << newOEP << std::endl;
	pOPTIONAL_HEADER->AddressOfEntryPoint = newOEP;
	return 1;
}

DWORD FwritrFile(LPVOID buffer, size_t size, const char* filePath)
{
	size_t len = strlen(filePath);
	char* newFilePath = (char*)malloc(len + 5);
	if (newFilePath == NULL)
	{
		return 0;
	}
	memset(newFilePath, 0, len + 5);
	strcpy_s(newFilePath, len + 5, filePath);
	char* newPath = strstr(newFilePath, ".exe");
	if (newPath != NULL) {
		strcpy_s(newPath, strlen("_new.exe") + 1 , "_new.exe");
	}

	if (filePath == NULL || buffer == NULL || size == NULL)
	{
		free(newFilePath);
		return 0;
	}
	
	FILE* file = NULL;
	fopen_s(&file, newFilePath, "wb");
	if (file == NULL)
	{
		std::cout << "写入文件失败" << std::endl;
		free(newFilePath);
		return 0;
	}
	size_t result = fwrite((unsigned char*)buffer, 1, size, file);
	if (result == size)
	{
		std::cout << "写入成功,写入字节数:" << std::dec <<  result << std::endl;
		std::cout << "写入文件位置:" << newFilePath << std::endl;
	}
	// 关闭文件
	fclose(file);
	// 释放分配的内存
	free(newFilePath);
	return result;
}
