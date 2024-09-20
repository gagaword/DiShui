#include "day28.h"

using namespace std;
/**
 * @brief 该方法通过导入表注入DLL
 * @param filePath 注入目标
 * @param InjectionDll 要注入的DLL
 * @param functionName DLL中导出函数名称
 * @return 
*/

bool MovImportAndInjectionDll(LPCSTR filePath, LPCSTR InjectionDll, LPCSTR functionName)
{
	

	// 写入文件
	DWORD writeSize = FwritrFile(NewFileBuffer, addByte, filePath);

	return false;
}
