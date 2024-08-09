#include<iostream>
#include<string.h>

// 分析以下结构所分配的空间大小
#pragma pack(1)
struct aa // 7
{
	int a;
	char b;
	short c;
};
#pragma pack()


// 2字节与int和short刚好对齐4+4+2=10
#pragma pack(2)
struct bb // 10
{
	int a;
	int b;
	short c;
};
#pragma pack()


#pragma pack(4)
struct cc // 4
{
	char a;
	char b;
	short c;
};
#pragma pack()


#pragma pack(8)
struct dd //12
{
	char a;
	int b;
	short c;
};
#pragma pack()

// 分析空间大小
struct S1 // 16
{
	char c;
	double i;
};

struct S4 //16
{
	int c1;
	char c2[10];
};

struct S3 //40
{
	char c1; // c1*8 s*16 c2*8 c3*8
	S1 s;
	char c2;
	double c3;
};
struct S3_1 // 16
{
	char c1; //c10000000 ssssssss c2*8 c3*8
	S1 s;
	char c2;
	char c3;
};

// 1、定义一个结构体Monster，能够存储怪的各种信息(至少有一个成员是结构体类型)。
typedef struct coordinate // 8
{
	int x;
	int y;
}coord;


typedef struct Monster // 44
{
	int id;
	int hp;
	int gongji;
	int yidong;
	char monName[20];
	coord cd;
}mons;


// 2、声明一个Monster类型的数组，长度为10.
mons monsInfo[10];

// 3、编写一个函数，为第二题中的数组赋值.
void monsInfoCopy()
{
	monsInfo[0].id = 01;
	monsInfo[0].hp = 100;
	monsInfo[0].gongji = 50;
	monsInfo[0].yidong = 90;
	strcpy_s(monsInfo[0].monName, "小怪");
	monsInfo[0].cd.x = 333;
	monsInfo[0].cd.y = 222;


	monsInfo[1].id = 02;
	monsInfo[1].hp = 200;
	monsInfo[1].gongji = 20;
	monsInfo[1].yidong = 60;
	strcpy_s(monsInfo[1].monName, "大怪");
	monsInfo[1].cd.x = 336;
	monsInfo[1].cd.y = 220;
}

// 4、编写一个函数，能够通过怪物ID，打印当前这个怪物的所有信息.
void printfMonsInfo()
{
	int id = 1;
	printf("%d %d %d %d %s %d %d",monsInfo[0].id, monsInfo[0].hp, monsInfo[0].gongji, monsInfo[0].yidong, monsInfo[0].monName, monsInfo[0].cd.x, monsInfo[0].cd.y);
}


void printfInfo(int parId)
{
	for (int i = 0; i < sizeof(monsInfo) / sizeof(monsInfo[0]); i++)
	{
		if (monsInfo[i].id == parId)
		{
			printf("%d %d %d %d %s %d %d\n", monsInfo[i].id, monsInfo[i].hp, monsInfo[i].gongji, monsInfo[i].yidong, monsInfo[i].monName, monsInfo[i].cd.x, monsInfo[i].cd.y);

		}
	}
}

int main()
{
	int arry[] = { 12,3 };
	printf("arry---->%d \n", sizeof(arry));

	printf("aa---->pack(1)%d \n", sizeof(aa));
	printf("bb---->pack(2)%d \n", sizeof(bb));
	printf("cc---->pack(4)%d \n", sizeof(cc));
	printf("dd---->pack(8)%d \n", sizeof(dd));

	printf("------------------\n");

	printf("s1---->pack(8)%d \n", sizeof(S1));
	printf("s4---->pack(8)%d \n", sizeof(S4));
	printf("s3---->pack(8)%d \n", sizeof(S3));
	printf("s3_1---->pack(8)%d \n", sizeof(S3_1));

	printf("------------------\n");

	monsInfoCopy();
	//printfMonsInfo();
	printfInfo(2);
	printf("------------------\n");

	printf("monsInfo---->%d\n", sizeof(monsInfo));
	
	printf("Monster---->%d\n", sizeof(Monster));
	printf("coord---->%d\n", sizeof(coord));
}