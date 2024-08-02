#include<iostream>	
#include<Windows.h>


/*
	1、定义一个unsiged char 类型，通过程序为第3、5、7位赋值,赋值时不能
	影响到其它位原来的值.
	(使用位操作指令、比如：& | ! ^ << >>等)

	2、判断某个位的值是否为1.
	(使用位操作指令、比如：& | ! ^ << >>等)

	3、读取第7、6、5位的值，以十进制显示(unsigned).
	(使用位操作指令、比如：& | ! ^ << >>等)
*/

// 提取指定范围的位值
unsigned int extractBits(unsigned char data, int start, int end) {
	// 掩码mask
	unsigned int mask = (1 << (end - start + 1)) - 1;
	printf("掩码---->%#x\n", mask);
	return (data >> start) & mask;
}

int main()
{
	// 0000 0000
	unsigned char x = 0;

	// 修改第3位为1,0000 0100
	x = 4 | x;
	printf("%x\n", x);

	// 在改为0
	x = x & 0;
	printf("%x\n", x);

	x = 0;
	x = 4 ^ x;
	printf("%x\n", x);
	x = 0;

	// 这种没必要了
	// 0000 0010
	x = 2 ^ x;
	x = x << 1;
	printf("%x\n", x);
	x = 0;

	// 修改第4位为1,0000 0100


	// 修改第5位为1,0000 0100
	// 0001 0000
	x = 16 | x;
	printf("%x\n", x);

	x = 0;
	x = 16 ^ x;
	printf("%x\n", x);

	// 改为0
	x = x & 0;
	printf("%x\n", x);

	x = 0;


	// 修改第7位为1,0000 0100 0100 0000
	x = 64 | x;
	printf("%x\n", x);

	x = 0;
	x = 64 ^ x;
	printf("%x\n", x);

	// 改为0
	x = x & 0;
	printf("%x\n", x);

	x = 0;

	// 判断某个位置是否是1；
	// 假设判断第四个位置是否是1
	// 0000 0000
	unsigned char data = 1;
	int values = data & 8;
	int result = 0;
	if (values != 1)
	{
		//result = MessageBoxA(NULL,"1的第四个位置不是1","消息", MB_OK);
		printf("%d的第四个位置不是1",data);
	}
	printf("%d\n", result);

	// 读取第7、6、5位的值，以十进制显示(unsigned).
	// 0111 0000
	// 1010 0101
	
	unsigned char data1 = 0x70;
	int res = data1 >> 4;
	printf("%d\n", res);
	
	unsigned char data2 = 0xA5;
	int aa = (data2 >> 4) & 2;
	printf("%d\n", aa);

	// 通用函数，使用掩码完成提取数值
	int sssss = extractBits(data2,4,6);
	printf("%d\n", sssss);
	return 0;
}