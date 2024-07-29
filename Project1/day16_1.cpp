#include<iostream>


// 指针数组
void pointerArray()
{
	// int类型，存储5个int类型的数据
	int array[] = { 1,2,3,4,5 };
	int arrayLength = sizeof(array) / sizeof(array[0]);
	for (int i = 0; i < arrayLength; i++)
	{
		printf("%d ", *(array + i));
	}

	printf("\n--------------------------------------\n");

	// int类型的指针，存储的是int类型的指针(听君一席话如听一席话......)
	int a = 10, b = 20, c = 30, d = 40, e = 50;
	int* arr[] = { &a, &b, &c, &d, &e}; // 存储的是a,b,c,d,e的地址。这些地址分别指向了10，20，30，40，50
	int arrLength = sizeof(arr) / sizeof(arr[0]);

	for (int i = 0; i < arrLength; i++)
	{
		printf("%p---->%p---->%d ", (arr + i), *(arr + i), *(*(arr + i)));
	}
	printf("\n--------------------------------------\n");

	// 最常用的指针数组：
	// p1已经是char指针了，不需要使用取值符&了。
	char* p1 = (char*)"if";
	char* p2 = (char*)"for";
	char* p3 = (char*)"while";
	char* p4 = (char*)"switch";
	
	char* keyword[] = { p1,p2,p3,p4 };
	// keyword保存的已经是这些字符串的地址了，通过*[keyword[i]]取到的是字符串的首字符。
	// 切记不可再通过*[keyword[i]]去取值，因为这个时候取到的是字符串，可能会造成内存无法访问。
	// 这里与上面那个int不同的就是，int我是通过两个变量去给指针数组进行的赋值，才会导致有两个地址
	// 而这里的char是直接把字符串的地址放到了指针数组中所有只会有一层地址。
	// p1的数据直接指向了字符串"if"再把这个p1的数据放到keyword中
	for (int i = 0; i < sizeof(keyword) / sizeof(keyword[0]); i++)
	{
		printf("%p---->%s\n", keyword[i], (keyword[i]));
	}

	printf("\n--------------------------------------\n");

	// 这个和上面那个是等价的
	char* keyword2[] =
	{
		(char*)"if",
		(char*)"for",
		(char*)"while",
		(char*)"switch"
	};
	for (int i = 0; i < 4; i++)
	{
		printf("%p---->%s\n", keyword2[i],keyword2[i]);
	}

}

// 定义结构体
typedef struct Student
{
	int x;
	int y;
}stu;

// 结构体指针
void structArry()
{
	stu* s1;
	s1 = (stu*)20; // s1保存的是0x00000014这个地址(20-->0x14)
	printf("s1----->%p\n", s1);

	s1++; // 28
	printf("s1++--->%p\n", s1);

	stu s2;
	s2.x = 10;
	s2.y = 20;
	printf("\n--------------------------------------\n");

	// ptrS2保存了s2的地址,就是结构体第一个成员的地址
	stu* ptrS2 = &s2;
	printf("%d\n", ptrS2->x);
	printf("%d\n", ptrS2->y);
}

int main()
{
	pointerArray();
	printf("\n--------------------------------------\n");

	structArry();
	return 0;
}