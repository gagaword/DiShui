#include "My_day23.h"

// MessageBoxA地址
void* MessageBoxA_address = &MessageBoxA;

// PE信息
struct PEHeaders {
	PIMAGE_DOS_HEADER dosHeader;
	PIMAGE_NT_HEADERS32 ntHeaders;
	PIMAGE_FILE_HEADER fileHeader;
	PIMAGE_OPTIONAL_HEADER32 optionalHeader;
	PIMAGE_SECTION_HEADER sectionHeader;
};

// 获取PE头
bool GetPEHeaders(LPVOID fileBuffer, PEHeaders& headers) {
	if (fileBuffer == nullptr) {
		return false;
	}

	headers.dosHeader = (PIMAGE_DOS_HEADER)fileBuffer;
	if (headers.dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
		return false;
	}

	headers.ntHeaders = (PIMAGE_NT_HEADERS32)((BYTE*)fileBuffer + headers.dosHeader->e_lfanew);
	if (headers.ntHeaders->Signature != IMAGE_NT_SIGNATURE) {
		return false;
	}

	headers.fileHeader = &headers.ntHeaders->FileHeader;
	headers.optionalHeader = &headers.ntHeaders->OptionalHeader;
	headers.sectionHeader = IMAGE_FIRST_SECTION(headers.ntHeaders);

	return true;
}

// 读取文件
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


// 插入ShellCode
DWORD AddShellCode(IN LPVOID filePath)
{
	std::cout << "MessageBoxExA_Address---->" << MessageBoxA_address << std::endl;

	if (filePath == NULL)
	{
		return 0;
	}

	// 读取文件
	LPVOID fileBuffer = NULL;
	DWORD ReadFileResult = ReadFile((char*)filePath, &fileBuffer);
	if (ReadFileResult == 0)
	{
		return 0;
	}
	
	// 获取PE信息
	PEHeaders peheader;
	if (!GetPEHeaders(fileBuffer,peheader))
	{
		std::cerr << "无效PE文件" << std::endl;
		return false;
	}

	size_t NumberSections = peheader.fileHeader->NumberOfSections;
	size_t FileOffset = 0;
	size_t MemeryOffset = 0;
	size_t MiscSize = 0;

	// 修改Characteristics字段
	peheader.ntHeaders->FileHeader.Characteristics |= IMAGE_FILE_RELOCS_STRIPPED;
	std::cout << "Characteristics修改完成" << std::endl;

	// 获取.text字段
	for (int i = 0; i < NumberSections; i++)
	{
		if (strcmp((const char*)peheader.sectionHeader[i].Name,".text") == 0)
		{
			FileOffset = peheader.sectionHeader[i].PointerToRawData;
			MemeryOffset = peheader.sectionHeader[i].VirtualAddress;
			MiscSize = peheader.sectionHeader[i].Misc.VirtualSize;
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
	DWORD EntryPoin = peheader.optionalHeader->AddressOfEntryPoint + peheader.optionalHeader->ImageBase;
	
	// 硬编码
	char ShellCode[] = { 0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00, 0xE8, 0x00, 0x00, 0x00, 0x00, 0xE9, 0x00, 0x00, 0x00, 0x00 };
	int ShellLength = sizeof(ShellCode) / sizeof(ShellCode[0]);

	// 计算 E8 跳转地址和 E9 跳转地址
	DWORD E8FileOffset = ((FileOffset + MiscSize) + 0x8) + 0x5;
	DWORD E9FileOffset = E8FileOffset + 0x5;
	DWORD E8CALL = (DWORD_PTR)MessageBoxA_address - ((((E8FileOffset - FileOffset) + MemeryOffset)) + peheader.optionalHeader->ImageBase);
	DWORD E9JMP = EntryPoin - ((((E9FileOffset - FileOffset) + MemeryOffset)) + peheader.optionalHeader->ImageBase);
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


	// 确定 ShellCode 插入位置
	BYTE* address = ((BYTE*)fileBuffer + (FileOffset + MiscSize));
	// 插入ShellCode。
	for (int i = 0; i < ShellLength; i++)
	{
		address[i] = ShellCode[i];
	}

	// 打印插入的硬编码
	std::cout << "*****************************************S插入硬编码*****************************************" << std::endl;
	for (size_t i = 0; i < ShellLength; i++)
	{
		printf("%02X ", *(((BYTE*)fileBuffer + (FileOffset + MiscSize)) + i));
	}
	std::cout << "\n*****************************************E插入硬编码*****************************************" << std::endl;

	// 修改OEP,OEP使用的是相对地址
	DWORD newOEP = (MiscSize + FileOffset) - FileOffset + MemeryOffset;
	std::cout << "newOEP----->" << std::hex << newOEP << std::endl;
	peheader.optionalHeader->AddressOfEntryPoint = newOEP;

	//存盘
	DWORD writeResult = FwritrFile(fileBuffer, ReadFileResult, (char*)filePath);
	if (writeResult != 0)
	{
		std::cout << "存盘成功" << std::endl;
		return 1;
	}
}

// 路径新名称
const char* newFilePathName(const char* filepath)
{
	if (filepath == NULL)
	{
		return 0;
	}
	size_t len = strlen(filepath);
	char* newFilePath = (char*)malloc(len + 5);
	if (newFilePath == NULL)
	{
		return 0;
	}
	memset(newFilePath, 0, len + 5);
	strcpy_s(newFilePath, len + 5, filepath);
	char* newPath = strstr(newFilePath, ".exe");
	if (newPath != NULL) {
		strcpy_s(newPath, strlen("_new.exe") + 1, "_new.exe");
	}
	return newFilePath;
}

// 写入硬盘
DWORD FwritrFile(IN LPVOID buffer, IN size_t size, OUT const char* filePath)
{
	const char* newFilePath = newFilePathName(filePath);

	if (filePath == NULL || buffer == NULL || size == NULL)
	{
		return 0;
	}
	
	FILE* file = NULL;
	fopen_s(&file, newFilePath, "wb");

	if (file == NULL)
	{
		std::cout << "写入文件失败" << std::endl;
		free((void*)newFilePath);
		return 0;
	}

	size_t result = fwrite((unsigned char*)buffer, 1, size, file);
	if (result == size)
	{
		std::cout << "写入成功,写入字节数:" << std::dec <<  result << std::endl;
		std::cout << "写入文件位置:" << newFilePath << std::endl;
	}

	fclose(file);
	free((void*)newFilePath);
	return result;
}
