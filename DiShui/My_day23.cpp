#include "My_day23.h"
#include "day22.h"

#define AddByte 0x1000
#define DEBUG 1

#define align_1000(VALUE) VALUE = ((VALUE + 0xFFF) / 0x1000) * 0x1000

// MessageBoxA地址
void* MessageBoxA_address = &MessageBoxA;

// 硬编码
BYTE ShellCode[] = { 0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00, 0xE8, 0x00, 0x00, 0x00, 0x00, 0xE9, 0x00, 0x00, 0x00, 0x00 };
int ShellLength = sizeof(ShellCode) / sizeof(ShellCode[0]);

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
DWORD ReadFile(IN LPCSTR filepath, OUT LPVOID* fileBuffer)
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

	printf("文件字节------> %X \n", fileSize);

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
	printf("读取字节------> %X \n", freadResult);
	return freadResult;
}

// 插入ShellCode
DWORD AddShellCode(IN LPCSTR filePath)
{	
	std::cout << "MessageBoxExA_Address---->" << MessageBoxA_address << std::endl;

	if (filePath == NULL)
	{
		return 0;
	}

	// 读取文件
	LPVOID fileBuffer = NULL;
	DWORD ReadFileResult = ReadFile((char*)filePath, &fileBuffer);
	if (ReadFileResult == 0) return 0;
	
	
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
	errno_t SizeRawData = 0;

	// 获取.text字段
	for (int i = 0; i < NumberSections; i++)
	{
		if (strcmp((const char*)peheader.sectionHeader[i].Name,".text") == 0)
		{
			FileOffset = peheader.sectionHeader[i].PointerToRawData;
			MemeryOffset = peheader.sectionHeader[i].VirtualAddress;
			MiscSize = peheader.sectionHeader[i].Misc.VirtualSize;
			SizeRawData = peheader.sectionHeader[i].SizeOfRawData;
			break;
		}
	}

	if (FileOffset == 0)
	{
		std::cerr << "未找到.text" << std::endl;
		return 0;
	}
	if ((int)(SizeRawData - MiscSize) < ShellLength)
	{
		std::cerr << "text空白区域空间不足" << std::endl;
		return false;
	}
	std::cout << "FileOffset---->" << std::hex << FileOffset << std::endl;
	std::cout << "MemeryOffset---->" << std::hex << MemeryOffset << std::endl;
	std::cout << "MiscSize---->" << std::hex << MiscSize << std::endl;

	// 修改随机基地址
	peheader.ntHeaders->FileHeader.Characteristics |= IMAGE_FILE_RELOCS_STRIPPED;
	std::cerr << "固定基地址修改完成" << std::endl;

	// OEP地址
	DWORD EntryPoin = peheader.optionalHeader->AddressOfEntryPoint + peheader.optionalHeader->ImageBase;
	
	// 计算 E8 跳转地址和 E9 跳转地址
	DWORD E8FileOffset = ((FileOffset + MiscSize) + 0x8) + 0x5;
	DWORD E9FileOffset = E8FileOffset + 0x5;
	DWORD E8CALL = (DWORD_PTR)MessageBoxA_address - ((((E8FileOffset - FileOffset) + MemeryOffset)) + peheader.optionalHeader->ImageBase);
	DWORD E9JMP = EntryPoin - ((((E9FileOffset - FileOffset) + MemeryOffset)) + peheader.optionalHeader->ImageBase);
	std::cout << "E8FileOffset---->" << std::hex << E8FileOffset << std::endl;
	std::cout << "E9FileOffset---->" << std::hex << E9FileOffset << std::endl;
	std::cout << "EntryPoin------->" << std::hex << EntryPoin << std::endl;
	printf("E8CALL----> %X \nE9JMP-----> %X\n", E8CALL, E9JMP);
	
	// 修正ShellCode
	*(PDWORD)(ShellCode + 9) = E8CALL;
	*(PDWORD)(ShellCode + 0xE) = E9JMP;
	
	// 确定 ShellCode 插入位置
	BYTE* address = ((BYTE*)fileBuffer + (FileOffset + MiscSize));
	printf("ShellCode插入位置-------->%p\n", address);
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

// 在data节添加ShellCode
bool AddShellCodeInData(IN LPCSTR filePath)
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
	if (!GetPEHeaders(fileBuffer, peheader))
	{
		std::cerr << "无效PE文件" << std::endl;
		return false;
	}

	size_t NumberSections = peheader.fileHeader->NumberOfSections;
	size_t FileOffset = 0;
	size_t MemeryOffset = 0;
	size_t MiscSize = 0;
	errno_t SizeRawData = 0;
	
	
	// 获取.data字段
	for (int i = 0; i < NumberSections; i++)
	{
		if (strcmp((const char*)peheader.sectionHeader[i].Name, ".data") == 0)
		{
			peheader.sectionHeader->Characteristics |= peheader.sectionHeader->Characteristics;
			FileOffset = peheader.sectionHeader[i].PointerToRawData;
			MemeryOffset = peheader.sectionHeader[i].VirtualAddress;
			MiscSize = peheader.sectionHeader[i].Misc.VirtualSize;
			SizeRawData = peheader.sectionHeader[i].SizeOfRawData;
			break;
		}
	}
	if (FileOffset == 0)
	{
		std::cerr << "未找到.data" << std::endl;
		return 0;
	}
	if ((int)(SizeRawData - MiscSize) < ShellLength)
	{
		std::cerr << "data空白区域空间不足" << std::endl;
		return false;
	}
	std::cout << "FileOffset---->" << std::hex << FileOffset << std::endl;
	std::cout << "MemeryOffset---->" << std::hex << MemeryOffset << std::endl;
	std::cout << "MiscSize---->" << std::hex << MiscSize << std::endl;

	// 修改Characteristics字段
	peheader.ntHeaders->FileHeader.Characteristics |= IMAGE_FILE_RELOCS_STRIPPED;
	std::cout << "Characteristics已改为固定基址" << std::endl;

	// OEP地址
	DWORD EntryPoin = peheader.optionalHeader->AddressOfEntryPoint + peheader.optionalHeader->ImageBase;

	// 计算 E8 跳转地址和 E9 跳转地址
	DWORD E8FileOffset = ((FileOffset + MiscSize) + 0x8) + 0x5;
	DWORD E9FileOffset = E8FileOffset + 0x5;
	DWORD E8CALL = (DWORD_PTR)MessageBoxA_address - ((((E8FileOffset - FileOffset) + MemeryOffset)) + peheader.optionalHeader->ImageBase);
	DWORD E9JMP = EntryPoin - ((((E9FileOffset - FileOffset) + MemeryOffset)) + peheader.optionalHeader->ImageBase);
	std::cout << "E8FileOffset---->" << std::hex << E8FileOffset << std::endl;
	std::cout << "E9FileOffset---->" << std::hex << E9FileOffset << std::endl;
	std::cout << "EntryPoin------->" << std::hex << EntryPoin << std::endl;
	printf("E8CALL----> %X \nE9JMP-----> %X\n", E8CALL, E9JMP);

	// 修正ShellCode
	*(PDWORD)(ShellCode + 9) = E8CALL;
	*(PDWORD)(ShellCode + 0xE) = E9JMP;

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

	return true;
}

// 添加ShellCode到Rdata节
bool AddShellCodeRdata(IN LPCSTR filePath)
{
	std::cout << "MessageBoxExA_Address---->" << MessageBoxA_address << std::endl;

	if (filePath == NULL)
	{
		return false;
	}

	// 读取文件
	LPVOID fileBuffer = NULL;
	DWORD ReadFileResult = ReadFile((char*)filePath, &fileBuffer);
	if (ReadFileResult == 0)
	{
		return false;
	}

	// 获取PE信息
	PEHeaders peheader;
	if (!GetPEHeaders(fileBuffer, peheader))
	{
		std::cerr << "无效PE文件" << std::endl;
		return false;
	}

	size_t NumberSections = peheader.fileHeader->NumberOfSections;
	size_t FileOffset = 0;
	size_t MemeryOffset = 0;
	size_t MiscSize = 0;
	errno_t SizeRawData = 0;

	// 获取.rdata字段
	for (int i = 0; i < NumberSections; i++)
	{
		if (strcmp((const char*)peheader.sectionHeader[i].Name, ".rdata") == 0)
		{
			peheader.sectionHeader[i].Characteristics |= peheader.sectionHeader->Characteristics;
			FileOffset = peheader.sectionHeader[i].PointerToRawData;
			MemeryOffset = peheader.sectionHeader[i].VirtualAddress;
			MiscSize = peheader.sectionHeader[i].Misc.VirtualSize;
			SizeRawData = peheader.sectionHeader[i].SizeOfRawData;
			break;
		}
	}
	if (FileOffset == 0)
	{
		std::cerr << "未找到.rdata" << std::endl;
		return 0;
	}
	if ((int)(SizeRawData - MiscSize) < ShellLength)
	{
		std::cerr << "rdata空白区域空间不足" << std::endl;
		return false;
	}
	std::cout << "FileOffset---->" << std::hex << FileOffset << std::endl;
	std::cout << "MemeryOffset---->" << std::hex << MemeryOffset << std::endl;
	std::cout << "MiscSize---->" << std::hex << MiscSize << std::endl;

	// 修改Characteristics字段
	peheader.ntHeaders->FileHeader.Characteristics |= IMAGE_FILE_RELOCS_STRIPPED;
	std::cout << "Characteristics已改为固定基址" << std::endl;

	// OEP地址
	DWORD EntryPoin = peheader.optionalHeader->AddressOfEntryPoint + peheader.optionalHeader->ImageBase;

	// 计算 E8 跳转地址和 E9 跳转地址
	DWORD E8FileOffset = ((FileOffset + MiscSize) + 0x8) + 0x5;
	DWORD E9FileOffset = E8FileOffset + 0x5;
	DWORD E8CALL = (DWORD_PTR)MessageBoxA_address - ((((E8FileOffset - FileOffset) + MemeryOffset)) + peheader.optionalHeader->ImageBase);
	DWORD E9JMP = EntryPoin - ((((E9FileOffset - FileOffset) + MemeryOffset)) + peheader.optionalHeader->ImageBase);
	std::cout << "E8FileOffset---->" << std::hex << E8FileOffset << std::endl;
	std::cout << "E9FileOffset---->" << std::hex << E9FileOffset << std::endl;
	std::cout << "EntryPoin------->" << std::hex << EntryPoin << std::endl;
	printf("E8CALL----> %X \nE9JMP-----> %X\n", E8CALL, E9JMP);

	// 修正ShellCode
	*(PDWORD)(ShellCode + 9) = E8CALL;
	*(PDWORD)(ShellCode + 0xE) = E9JMP;

	// 确定 ShellCode 插入位置
	BYTE* address = ((BYTE*)fileBuffer + FileOffset + MiscSize);
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
		return true;
	}

	return true;
}

// 该方法新增一个节并添加ShellCode
bool AddSection(IN LPCSTR filePath)
{
	if (filePath == nullptr)
	{
		std::cerr << "Error: 参数错误" << std::endl;
		return false;
	}
	
	LPVOID fileBuffer = NULL;
	errno_t ReadFileResult = ReadFile((char*)filePath, &fileBuffer);
	if (ReadFileResult == 0) 
	{
		std::cerr << "Error: 读取文件失败" << std::endl;
		return false;
	}

	if (DEBUG == 1) for (size_t i = 0; i < 0x20; i++) printf("%X ", *((BYTE*)fileBuffer + i));
	
	// 获取PE信息
	PEHeaders peheaders;
	bool PeResult = GetPEHeaders(fileBuffer, peheaders);
	if (PeResult == 0)
	{
		std::cerr << "Error: 获取PE头信息失败" << std::endl;
		return false;
	}

	// 修改随机基地址
	peheaders.ntHeaders->FileHeader.Characteristics |= IMAGE_FILE_RELOCS_STRIPPED;
	std::cerr << "固定基地址修改完成" << std::endl;
	
	// 文件大小，文件尾插入数据
	size_t fileSize = peheaders.optionalHeader->SizeOfHeaders;
	size_t NumberSection = peheaders.fileHeader->NumberOfSections;
	size_t FileOffset = 0, MemeryOffset = 0, MiscSize = 0;
	DWORD SizeRawData = 0;

	for (size_t i = 0; i < NumberSection; i++)
	{
		fileSize += peheaders.sectionHeader[i].SizeOfRawData;
		if (i == NumberSection - 1)
		{
			FileOffset = peheaders.sectionHeader[i].PointerToRawData;
			MemeryOffset = peheaders.sectionHeader[i].VirtualAddress;
			MiscSize = peheaders.sectionHeader[i].Misc.VirtualSize;
			SizeRawData = peheaders.sectionHeader[i].SizeOfRawData;
			std::cout << "Name---->" << peheaders.sectionHeader[i].Name << std::endl;
		}
	}
	
	printf("文件原始大小------> %X \n", fileSize);
	if (fileSize < ReadFileResult)
	{
		std::cerr << "文件尾部有未知数据，为了你的文件安全不在新增节" << std::endl;
		return 0;
	}

	// 填充信息
	unsigned char hexData[40] = {
		0x2E, 0x74, 0x65, 0x78, 0x74, 0x00, 0x00, 0x00,  0x9A, 0xBA, 0x01, 0x00,  0x00, 0x10, 0x00, 0x00,
		0x00, 0xC0, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x60
	};
	unsigned char Name[] = { 0x47,0x61, 0x47,0x61 };

	// 重新分配内存以包含追加的字节
	size_t newSize = fileSize + AddByte;
	printf("文件最新大小------> %X \n", newSize);

	unsigned char* newBuffer = (unsigned char*)realloc(fileBuffer, newSize);
	if (!newBuffer) {
		perror("内存分配错误");
		return false;
	}
	memset(newBuffer + fileSize, 0, AddByte);

	// 更新PE信息;
	PEHeaders newPeHeaders;
	if (!GetPEHeaders(newBuffer, newPeHeaders)) {
		std::cerr << "Error: 更新PE头信息失败" << std::endl;
		free(newBuffer);
		return false;
	}

	size_t newFileOffset = 0, newMemeryOffset = 0, newMiscSize = 0;
	DWORD newSizeRawData = 0;

	size_t newNumberSeciton = newPeHeaders.fileHeader->NumberOfSections;
	for (size_t i = 0; i < newNumberSeciton; i++)
	{
		if (i == newNumberSeciton - 1)
		{
			newFileOffset = newPeHeaders.sectionHeader[i].PointerToRawData;
			newMemeryOffset = newPeHeaders.sectionHeader[i].VirtualAddress;
			newMiscSize = newPeHeaders.sectionHeader[i].Misc.VirtualSize;
			newSizeRawData = newPeHeaders.sectionHeader[i].SizeOfRawData;
		}
	}

	if (DEBUG == 1)
	{
		std::cout << "newFileOffset----> " << std::hex << newFileOffset << std::endl;
		std::cout << "newMemeryOffset----> " << std::hex << newMemeryOffset << std::endl;
		std::cout << "newMiscSize-------> " << std::hex << newMiscSize << std::endl;
		std::cout << "newSizeRawData-------> " << std::hex << newSizeRawData << std::endl;
	}
	
	// 新增节表
	// 确定位置
	DWORD addByte = AddByte;
	
	// 计算内存偏移和文件偏移
	DWORD VirtualAddress = newMiscSize + newMemeryOffset;
	if (VirtualAddress % (newPeHeaders.optionalHeader->SectionAlignment) != 0) align_1000(VirtualAddress);
	std::cout << "NewSection----> VirtualAddress----> " << std::hex << VirtualAddress << std::endl;
	DWORD PointerToRawData = newFileOffset + newSizeRawData;
	std::cout << "NewSection----> PointerToRawData--> " << std::hex << PointerToRawData << std::endl;

	// 修正hexData
	memcpy_s(&hexData[1], sizeof(Name), Name, sizeof(Name));
	memcpy_s(&hexData[8], sizeof(addByte), &addByte, sizeof(addByte));
	memcpy_s(&hexData[12], sizeof(VirtualAddress), &VirtualAddress, sizeof(VirtualAddress));
	memcpy_s(&hexData[16],sizeof(addByte), &addByte,sizeof(addByte));
	memcpy_s(&hexData[20],sizeof(PointerToRawData), &PointerToRawData, sizeof(PointerToRawData));

	printf("********************************节表信息*********************************\n");
	for (int i = 0; i < 0x28; i++)
	{
		printf("%02X ", hexData[i]);
	}
	printf("\n");

	// 插入节表
	DWORD address = (newPeHeaders.dosHeader->e_lfanew + 0x04 + IMAGE_SIZEOF_FILE_HEADER + newPeHeaders.fileHeader->SizeOfOptionalHeader + newPeHeaders.fileHeader->NumberOfSections * 0x28);
	BYTE* sectionAddres = (BYTE*)newBuffer + address;
	if (DEBUG == 1)
	{
		std::cout << "新增节表偏移--------> " << address << std::endl;
		std::cout << "新增节表位置--------> " << (void*)sectionAddres << std::endl;
	}
	
	// 数据插入
	for (size_t i = 0; i < 0x28; i++)
	{
		sectionAddres[i] = hexData[i];
	}

	// 修改PE信息
	newPeHeaders.fileHeader->NumberOfSections += 0x1;
	newPeHeaders.optionalHeader->SizeOfImage += 0x1000;

	// OEP地址
	DWORD EntryPoin = newPeHeaders.optionalHeader->AddressOfEntryPoint + newPeHeaders.optionalHeader->ImageBase;

	// 计算 E8 跳转地址和 E9 跳转地址
	DWORD E8FileOffset = (PointerToRawData + 0x8) + 0x5;
	DWORD E9FileOffset = E8FileOffset + 0x5;
	DWORD E8CALL = (DWORD_PTR)MessageBoxA_address - ((((E8FileOffset - PointerToRawData) + VirtualAddress)) + newPeHeaders.optionalHeader->ImageBase);
	DWORD E9JMP = EntryPoin - ((((E9FileOffset - PointerToRawData) + VirtualAddress)) + newPeHeaders.optionalHeader->ImageBase);

	if (DEBUG == 1)
	{
		std::cout << "E8FileOffset----> " << std::hex << E8FileOffset << std::endl;
		std::cout << "E9FileOffset----> " << std::hex << E9FileOffset << std::endl;
		std::cout << "EntryPoin-------> " << std::hex << EntryPoin << std::endl;
		printf("E8CALL----> %X \nE9JMP-----> %X\n", E8CALL, E9JMP);
	}
	
	// 修正ShellCode
	*(PDWORD)(ShellCode + 9) = E8CALL;
	*(PDWORD)(ShellCode + 0xE) = E9JMP;

	// 确定 ShellCode 插入位置
	BYTE* Newaddress = ((BYTE*)newBuffer + fileSize);
	// 插入ShellCode。
	for (int i = 0; i < ShellLength; i++)
	{
		Newaddress[i] = ShellCode[i];
	}

	// 修正OEP
	DWORD newOEP = (fileSize - PointerToRawData) + VirtualAddress;
	newPeHeaders.optionalHeader->AddressOfEntryPoint = newOEP;
	if (DEBUG == 1)
	{
		std::cerr << "fileSize--------> " << fileSize << std::endl;
		std::cout << "newOEP-----> " << std::hex << newOEP << std::endl;
	}
	

	// 存盘
	FwritrFile(newBuffer, newSize, (char*)filePath);
	free(newBuffer);
	return true;
}

// 扩展节
bool ExpandSection(IN LPCSTR filePath)
{
	if (filePath == NULL)
	{
		perror("Para Error");
		return false;
	}

	// 读取文件
	LPVOID fileBufer = nullptr;
	size_t readFileReuslt = ReadFile((char*)filePath, &fileBufer);
	if (readFileReuslt == 0)
	{
		perror("ReadFile Error");
		return false;
	}

	// PE信息
	PEHeaders peheader;
	if (!GetPEHeaders(fileBufer,peheader))
	{
		perror("PE Info Error");
		return false;
	}

	// 固定基地址
	peheader.fileHeader->Characteristics |= IMAGE_FILE_RELOCS_STRIPPED;
	if (DEBUG) std::cerr << "基地址已固定" << std::endl;


	// 文件真实大小,最后一节
	long FileSize = peheader.optionalHeader->SizeOfHeaders;
	size_t numberSection = peheader.fileHeader->NumberOfSections;

	//最后一节字段属性
	size_t FileOffset = 0, MemeryOffset = 0, MiscSize = 0;
	errno_t sizeData = 0;

	// 使用循环获取到最后一节
	for (size_t i = 0; i < numberSection; i++)
	{
		FileSize += peheader.sectionHeader[i].SizeOfRawData;
		if (i == numberSection - 1)
		{
			FileOffset = peheader.sectionHeader[i].PointerToRawData;
			MemeryOffset = peheader.sectionHeader[i].VirtualAddress;
			MiscSize = peheader.sectionHeader[i].Misc.VirtualSize;
			sizeData = peheader.sectionHeader[i].SizeOfRawData;
			//std::cout << "SectionName-------> " << peheader.sectionHeader[i].Name << std::endl;
			break;
		}
	}
	if (DEBUG)
	{
		std::cout << "FileOffset--------> " << std::hex << FileOffset << std::endl;
		std::cout << "MemeryOffset------> " << std::hex << MemeryOffset << std::endl;
		std::cout << "MiscSize----------> " << std::hex << MiscSize << std::endl;
		std::cout << "sizeData----------> " << std::hex << sizeData << std::endl;
		std::cout << "FileSize----------> " << std::hex << FileSize << std::endl;
	}
	
	if (FileSize < readFileReuslt)
	{
		perror("注意:末尾节尾部有多余数据,为了您的文件安全不在扩展节");
		return false;
	}

	// 节尾
	DWORD EndSectionAddress = FileOffset + sizeData;
	if(DEBUG) std::cout << "EndSectionAddress---->" << EndSectionAddress << std::endl;

	// 扩展0x200字节
	DWORD newBufferSize = readFileReuslt + 0x200;
	unsigned char* newBuffer = (unsigned char*)realloc(fileBufer, newBufferSize);
	if (newBuffer == NULL)
	{
		perror("malloc error");
		return false;
	}
	memset(newBuffer + readFileReuslt, 0, 0x200);

	// 更新PE信息
	PEHeaders newPeheader;
	if (!GetPEHeaders(newBuffer, newPeheader))
	{
		return false;
	}

	//最后一节字段属性
	long newFileSize = newPeheader.optionalHeader->SizeOfHeaders;
	size_t newNumbersection = newPeheader.fileHeader->NumberOfSections;
	size_t newFileOffset = 0, newMemeryOffset = 0, newMiscSize = 0;
	errno_t newsizeData = 0;

	// 使用循环获取到最后一节
	size_t j = 0;
	for ( j = 0; j < newNumbersection; j++)
	{
		newFileSize += newPeheader.sectionHeader[j].SizeOfRawData;
		if (j == numberSection - 1)
		{
			newFileOffset = newPeheader.sectionHeader[j].PointerToRawData;
			newMemeryOffset = newPeheader.sectionHeader[j].VirtualAddress;
			newMiscSize = newPeheader.sectionHeader[j].Misc.VirtualSize;
			newsizeData = newPeheader.sectionHeader[j].SizeOfRawData;
			//std::cout << "SectionName-------> " << peheader.sectionHeader[j].Name << std::endl;
			break;
		}
	}

	// 修正节表属性
	newPeheader.sectionHeader[j].Characteristics |= newPeheader.sectionHeader->Characteristics;
	if (DEBUG) std::cout << "节表已改为可执行" << std::endl;
	
	newPeheader.sectionHeader[j].SizeOfRawData += 0x200;
	newPeheader.sectionHeader[j].Misc.VirtualSize += 0x200;
	newPeheader.optionalHeader->SizeOfImage += 0x200;

	// ShellCode位置
	BYTE* shellCodeAddress = (BYTE*)newBuffer + newFileSize;

	// 修正E8 E9
	DWORD OepAddress = newPeheader.optionalHeader->AddressOfEntryPoint + newPeheader.optionalHeader->ImageBase;
	DWORD E8OFFSET = ((DWORD)EndSectionAddress + 0x8) + 0x5;
	DWORD E9OFFSET = E8OFFSET + 0x5;
	DWORD E8_VA = (((E8OFFSET - newFileOffset) + newMemeryOffset) + newPeheader.optionalHeader->ImageBase);
	DWORD E9_VA = (((E9OFFSET - newFileOffset) + newMemeryOffset) + newPeheader.optionalHeader->ImageBase);
	DWORD E8CALL = (DWORD)MessageBoxA_address - (((E8OFFSET - newFileOffset) + newMemeryOffset) + newPeheader.optionalHeader->ImageBase);
	DWORD E9JMP = OepAddress - (((E9OFFSET - newFileOffset) + newMemeryOffset) + newPeheader.optionalHeader->ImageBase);

	if (DEBUG)
	{
		std::cout << "E8_VA-------> " << std::hex << E8_VA << std::endl;
		std::cout << "E9_VA-------> " << std::hex << E9_VA << std::endl;
		std::cout << "E8OFFSET----> " << std::hex << E8OFFSET << std::endl;
		std::cout << "E9OFFSET----> " << std::hex << E9OFFSET << std::endl;
		std::cout << "OepAddress-------> " << std::hex << OepAddress << std::endl;
		printf("E8CALL----> %X \nE9JMP-----> %X\n", E8CALL, E9JMP);
	}
	

	// 更新ShellCode
	*(LPDWORD)(ShellCode + 9) = E8CALL;
	*(LPDWORD)(ShellCode + 0xE) = E9JMP;

	// 添加ShellCode
	for (size_t i = 0; i < ShellLength; i++)
	{
		shellCodeAddress[i] = ShellCode[i];
	}

	// 修正OEP
	DWORD newOEP = (newFileSize - newFileOffset) + newMemeryOffset;
	newPeheader.optionalHeader->AddressOfEntryPoint = newOEP;
	if (DEBUG == 1)
	{
		std::cerr << "fileSize--------> " << newFileSize << std::endl;
		std::cout << "newOEP-----> " << std::hex << newOEP << std::endl;
	}
	

	// 存盘
	DWORD writeResult = FwritrFile(newBuffer, newBufferSize, (char*)filePath);

	return true;
}

// 打印数据目录
bool ImageData(IN LPCSTR filePath)
{
	if (filePath == NULL)
	{
		perror("ParrError");
		return false;
	}

	// 读取文件
	LPVOID fileBuffer = NULL;
	DWORD readFileResult = ReadFile((char*)filePath, &fileBuffer);
	if (readFileResult == 0)	return false;

	// PE信息
	//_IMAGE_DATA_DIRECTORY
	PEHeaders peheader;
	if (!GetPEHeaders(fileBuffer, peheader))	return false;
	std::cout << "数据目录大小----> " << peheader.optionalHeader->NumberOfRvaAndSizes << std::endl;
	PIMAGE_DATA_DIRECTORY data = (PIMAGE_DATA_DIRECTORY)&peheader.optionalHeader->DataDirectory;
	std::cout << "********************RVA******************" << std::endl;
	for (size_t i = 0; i < peheader.optionalHeader->NumberOfRvaAndSizes; i++)
	{
		printf("%08X\n", (DWORD)data[i].VirtualAddress);
	}

	std::cout << "********************Size******************" << std::endl;
	for (size_t i = 0; i < peheader.optionalHeader->NumberOfRvaAndSizes; i++)
	{
		printf("%08X\n", data[i].Size);
	}
	return true;
}

// 合并节
bool MergeSection(IN LPCSTR filePath)
{
	if (filePath == NULL)	return false;

	LPVOID fileBuffer = nullptr;
	if (!ReadFile((char*)filePath, &fileBuffer))	return false;

	LPVOID ImageBuffer = nullptr;
	if (!CopyFileBufferToImageBuffer(fileBuffer, &ImageBuffer))	return false;

	PEHeaders peheader;
	if (!GetPEHeaders(ImageBuffer, peheader))	return false;
	
	DWORD VS = peheader.optionalHeader->SizeOfImage - peheader.sectionHeader->VirtualAddress;
	//DWORD S = peheader.optionalHeader->SizeOfImage - peheader.optionalHeader->SizeOfHeaders;
	//std::cout << "VS " << VS  << std::endl;
	DWORD SR = VS;

	peheader.sectionHeader->Misc.VirtualSize = VS;
	peheader.sectionHeader->SizeOfRawData = SR;
	peheader.sectionHeader->Characteristics |= 0xFFFFFFFF;
	peheader.fileHeader->NumberOfSections = 0x1;

	LPVOID newBuffer = NULL;
	if (!CopyImageBufferToNewBuffer(ImageBuffer, &newBuffer))	return false;

	FwritrFile(newBuffer, VS + peheader.optionalHeader->SizeOfHeaders, (char*)filePath);
	return true;
}

// 文件新路径
LPCSTR newFilePathName(LPCSTR filepath)
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
	char* newPath = strstr(newFilePath, ".dll");
	if (newPath != NULL) {
		strcpy_s(newPath, strlen("_new.dll") + 1, "_new.dll");
	}
	return newFilePath;
}

// 写入文件
DWORD FwritrFile(IN LPVOID buffer, IN size_t size, OUT LPCSTR filePath)
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
		std::cout << "写入成功,写入字节数:" << std::hex << result << std::endl;
		std::cout << "写入文件位置:" << newFilePath << std::endl;
	}

	fclose(file);
	free((void*)newFilePath);
	return result;
}