#include<iostream>

/*
do....while...
while...
for....
*/


// 交换两个变量的值，不允许使用第三个变量和使用变量
void  function(int a, int c, int d, int e)
{
	// 第三个变量
	int t = a;
	a = c;
	c = t;
	printf("%d|%d\n", a, c);

	// 不用第三个变量
	d = d + e;
	e = d - e;
	d = d - e;
	printf("%d|%d\n", d, e);

}

// 数组中最大值

int arrayBig()
{
	int number[] = { 3,2,1,5,6 };
	int bigNumber = number[0];
	int i = 1;
	while (i <= 4)
	{
		if (number[i] > bigNumber)
		{
			bigNumber = number[i];
		}
		++i;
	}
	return bigNumber;
}

// 求数组所有数之和
int arrayAllSum()
{
	int number[] = { 2,1,3,4,5,6 };
	int i = 0;
	int sum = 0;
	while (i < 6)
	{
		sum = sum + number[i];
		++i;
	}
	return sum;
}
int main()
{
	function(1,2,1,2);

	int result = arrayBig();
	printf("%d\n", result);
	int result2 = arrayAllSum();
	printf("%d\n", result2);
	return 0;
}