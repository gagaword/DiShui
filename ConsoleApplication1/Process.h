#pragma once

#include<Windows.h>
#include<Psapi.h>


/*
******************************
获取指定程序(PID)的句柄
dwAccess:程序的权限。
isHandle:是否继承句柄。
processPid:进程PID。
by:GaGa
date:2024.7.12
******************************
*/
HANDLE getProcessHandle(DWORD dwAccess, BOOL isHandle, DWORD processPid);


/*
******************************
获取指定程序的PID
processName:进程名称。
by:GaGa
date:2024.7.12
******************************
*/
DWORD FindProcessIDByName(const std::wstring& processName);

/*
******************************
遍历所有进程，输出进程名称以及PID
by:GaGa
date:2024.7.12
******************************
*/
void getALLProcessName();


