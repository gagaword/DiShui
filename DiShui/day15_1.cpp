#include<iostream>

struct student
{
	int x;
	int y;
};

void lianxi()
{
	char a = 10;
	short b = 20;
	int c = 30;

	char* pa = &a;
	short* pb = &b;
	int* pc = &c;

	char** ppa = &pa;
	short** ppb = &pb;
	int** ppc = &pc;
}

//4、完成代码，实现数组值的互换

void Function()
{
	/*int arr[] = { 1,2,3,4,5,6,7,8 };*/
	int arr[] = { 2,3,5,1,2,4,7,9,7 };
	int length = sizeof(arr) / sizeof(arr[0]);
	int tempLength = 0;

	/*if (length % 2 !=0)
	{
		tempLength = length + 1;
	}
	else {
		tempLength = length;
	}*/

	/*printf("tempLength---->%d\n", tempLength);
	printf("length-------->%d\n", sizeof(arr) / sizeof(arr[0]));*/
	
	//..此处添加代码，使用指针，将数组的值倒置
	int* begin = (arr);
	int* end = (arr + (length - 1));

	/*printf("begin--------->%d\n", *(begin));
	printf("end----------->%d\n", *(end));*/

	int i = 0;
	/*while (i < (tempLength / 2))
	{
		int temp = *(begin);
		*(begin)= *(end);
		*(end )= temp;
		++begin;
		--end;
		++i;
	}*/

	while (begin < end)
	{
		int temp = *(begin);
		*(begin) = *(end);
		*(end) = temp;
		++begin;
		--end;
		++i;
	}
	printf("------------------------\n");

	int* p = arr;
	//打印数组值的代码已经写完，不需要修改				
	/*for (int k = 0; k < 5; k++)
	{
		printf("%d\n", *(p + k));
	}*/
	for (int k = 0; k < length; k++)
	{
		printf("%d ", *(p + k));
	}
}


void test()
{
	/*不允许
	int ss = 20;
	student aa = (student)ss;*/

	/*int* a = (int*)10;
	char* b = (char*)20;
	a = (int*)b;*/

	//int* i = (int*)10;
	//int** s = &i; // s保存的是i的地址
	//int* result = *(s); //*取出s保存i地址的值放到result中，此时result保存的就是10,
	////int resu = *(result); //result保存的是10，把10作为地址是无法找到对应的内存的，所以这里会发生异常。

	//int x = 100;
	//int* a = &x; // 取x的地址保存到a中
	//int resu = *(a); //取a地址指向的数据保存到resu中。


	/*
	错误代码
	int** uuu = (int**)200;
	int* re = *(uuu);
	printf("%d\n", re);*/


	int** uuu = (int**)200;
	int*** ss = &(uuu);
	int** re = *(ss);
	printf("%p\n", re);
	short arry[5] = { 1,2,3,4,5 };


	//int* u = &arry[0]; // 取数组第一个元素的地址
	// 简写
	short* u = arry; // u保存的就是数组第一个元素的地址
	for (int i = 0; i < sizeof(arry) / sizeof(arry[0]); i++)
	{
		//int result = *(u + i);
		printf("%d ", *(u + i));
	}
}
int main()
{
	
	//test();
	//lianxi();
	Function();
	return 0;
}