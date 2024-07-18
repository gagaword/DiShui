#include<iostream>	

// 裸函数，不让编译器生成汇编框架
// 汇编实现两个整数相加


int __declspec(naked) sumTwo(int a, int b)
{
	// 内联汇编
	__asm 
	{
		// 保存原始栈底
		push ebp

		// 提升堆栈空间
		mov ebp,esp
		sub esp,0x40

		// 保存原始寄存器信息
		push edi
		push esi
		push ebx

		//填充堆栈空间
		lea edi,dword ptr ds:[ebp - 0x40]
		mov eax,0xcccccccc
		mov ecx,0x10
		rep stosd

		// 函数体方法执行
		mov eax,dword ptr ds:[ebp+0x8]
		add eax,dword ptr ds:[ebp+0xc]

		// 恢复原始寄存器信息
		pop ebx
		pop esi
		pop edi

		// 恢复堆栈
		mov esp,ebp
		pop ebp
		ret
	}
}
int main()
{
	// 调用裸函数
	int result =  sumTwo(1,2);
	std::cout << result << std::endl;
}