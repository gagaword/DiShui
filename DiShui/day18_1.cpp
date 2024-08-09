#include<iostream>	

// 函数指针
// 函数指针与指针函数不同，指针函数代表了这个函数返回的是一个指针类型
// 而函数指针表示这个指针指向一个函数的首地址。
// 在C语言中，函数名也是全局变量，保存的是函数所执行的代码，这些可称为硬编码。
// 函数指针的定义：
// 函数指针指向的函数类型和参数与本指针一致，在一致的情况下可以省略强制类型转换
// 函数指针的应用：
// 把函数放到数据区，

int x = 20;

int addSum(int x, int y)
{
	return x + y;
}


unsigned char fun[] = {
	0x55,
	0x8B, 0xEC,
	0x83, 0xEC, 0x40,
	0x53,
	0x56,
	0x57,
	0x8D, 0x7D, 0xC0,
	0xB9, 0x10, 0x00, 0x00, 0x00,
	0xB8, 0xCC, 0xCC, 0xCC, 0xCC,
	0xF3, 0xAB,
	0x8B, 0x45, 0x08,
	0x03, 0x45, 0x0C,
	0x5F,
	0x5E,
	0x5B,
	0x8B, 0xE5,
	0x5D,
	0xC3,
};


int main()
{
	// 此时p指向了addSum的首地址
	int(*p)(int, int) = (int(__cdecl *)(int,int)) & addSum;
	int(*xx)(int) = (int(__cdecl *)(int))10;

	int result = p(2, 3);
	printf("%d\n", result);

	printf("%p\n", &addSum);	// 00961226
	printf("%p\n", p);			// 00961226
	printf("%d\n", x);


	int(*funP)(int, int) = (int(__cdecl *)(int,int))&fun;
	int ss = funP(2, 3);
	printf("ss--->%d\n", ss);

	/*
	* 为函数指针起个别名，Fun，函数指针类型是int(*)(int,int)
	* Fun是数据类型，p是变量名称
	typedef int (*Fun)(int, int);
	Fun p = (int (*)(int, int)) & fun;
	int xx = p(2, 3);
	printf("%d\n", xx);*/
	return 0;
}