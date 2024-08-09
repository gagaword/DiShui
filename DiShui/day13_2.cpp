#include<iostream>	

// 递归函数反汇编分析
// 斐波那契数列
int FeiBo(int n)
{
	if (n <= 1)
	{
		return n;
	}
	return FeiBo(n - 1) + FeiBo(n - 2);
}

void printFeiBo(int n)
{

	if (n <= 1)
	{
		printf("%d", n);
	}
	int k = 0, j = 1;
	int result = 0;
	printf("%d %d ", k,j);
	for (int i = 2; i <= n; i++)
	{
		result = k + j;
		k = j;
		j = result;
		printf("%d ", result);
	}
}
int main()
{
	int resu = FeiBo(9);
	printf("%d \n", resu);

	printFeiBo(6);
	return 0;
}