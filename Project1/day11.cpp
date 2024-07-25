#include<iostream>
#include <vector>

// 8位参数传递
void par8(char x, char a)
{

}

// 16位参数传递
void par16(short a, short b)
{

}

// 32位参数传递
void par32(int a, int b)
{

}

// 超过32位的变量如何存储
__int64 par64longlong()
{
	__int64 x = 0x12345678AAAAAAAA;
	return x;
}

//2、char arr[3] = {1,2,3};与 char arr[4] = {1,2,3,4};
// 哪个更节省空间，从反汇编的角度来说明你的观点
void function2()
{
	//char arr[3] = { 1,2,3 };
	char arr2[4] = { 1,2,3,4 };
}
// 局部变量的内存分配
void parLocal()
{
	char s = 1;
	char b = 2;
}

// 数组的分配
int array()
{
	/*int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	int e = 5;*/
	int array[] = { 1,2,3,4,5,6,7 };

	return 0;
}

// 3、找出下面赋值过程的反汇编代码
void Function()
{
	int x = 1;
	int y = 2;
	int r;
	int arr[10] = { 1,2,3,4,5,6,7,8,9,10 };

	r = arr[1];
	r = arr[x];
	r = arr[x + y];
	r = arr[x * 2 + y];
}


/*
桶排序
2 4 6 5 3 1 2 7
0 0 0 0 0 0 0 0
0 1 2 3 4 5 6 7

桶数组需要使用到动态分配
*/

void arraySort()
{
	int array[] = { 2,8,1,5,3,1,2,3 };
	int arrayLength = sizeof(array) / sizeof(array[0]);
	
	// 找出数组最大值
	int bigNumber = array[0];
	for (int i = 1; i < arrayLength; i++)
	{
		if (array[i] > bigNumber){
			bigNumber = array[i];
		}
	}

	printf("%d\n", bigNumber);

	// 动态分配内存
	int* array2 = new int[bigNumber + 1]{0};

	//// 动态分配内存
	//int* array2 = (int*)malloc((bigNumber + 1) * sizeof(int));
	//if (array2 == NULL)
	//{
	//	printf("Memory allocation failed\n");
	//	return;
	//}
	//// 内存初始化
	//for (int i = 0; i <= bigNumber; i++)
	//{
	//	array2[i] = 0;
	//}


	// 桶排序逻辑执行
	for (int i = 0; i < arrayLength; i++)
	{
		array2[array[i]] += 1;
	}

	// 抽象代码。。。。。
	/*for (int i = 0; i < 8; i++)
	{
		
		if (array2[i] == 1)
		{

			printf("%d ", i);
		}
		else if (array2[i] == 2)
		{
			printf("%d %d ", i, i);
		}
	}*/

	// 输出
	for (int i = 0; i <= arrayLength; i++)
	{
		for (int j = 0; j < array2[i]; j++)
		{
			printf("%d ", i);
		}
	}


	// 释放动态数组
	delete[] array2;
}

int main()
{
	__int64 number = par64longlong();
	printf("%llx\n", number);

	char i = 'a';
	short b = 'b';
	int num = 12;


	array();

	// 桶排序
	arraySort();

	// 8位参数
	par8(1,2);

	// 16位参数
	par16(2,4);

	// par32位参数
	par32(1,2);

	// 局部变量
	parLocal();

	// 空间问题
	function2();

	// 3、找出下面赋值过程的反汇编代码
	Function();
}

