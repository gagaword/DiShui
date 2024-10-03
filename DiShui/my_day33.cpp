#include<iostream>
#include<Windows.h>

using namespace std;

//使用模版实现swap(x, y)函数，功能：交换x, y的值.

template<class T, class E>
void swapA(T &x, E &y)
{
	T temp = x;
	x = y;
	y = temp;
}

class Student
{
public:
	int x;
	int y;
public:
	Student();
	~Student();

private:

};

Student::Student()
{
	this->x = 10;
	this->y = 20;
}

Student::~Student()
{
	cout << "析构函数执行" << endl;
}

// 2、冒泡排序：对结构体或者类进行排序，如果不能实现，找出问题所在.
template<class T>
void shorMao(T &array, int length)
{
	int k = 0;
	int j = 0;
	for (k = 0; k < length - 1; k++)
	{
		for (j = 0; j < length  - k - 1; j++)
		{
			if (array[j] > array[j + 1])
			{
				int temp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = temp;
			}
		}
	}
}

// 二分查找
int FindValues(int array[], int length, int FindValue)
{
	int strat = 0;
	int end = length - 1;
	while (strat <= end)
	{
		int temp = (strat + end) / 2;
		if (array[temp] > FindValue)
		{
			end = temp - 1;
		}
		else if (array[temp] < FindValue)
		{
			strat = temp + 1;
		}
		else {
			return temp;
		}
	}
	return -1;
}

template<class T, class E>
int Find(T array,int length, E findValue)
{
	int strat = 0;
	int end = length - 1;
	while (strat <= end)
	{
		int temp = (strat + end) / 2;
		if (array[temp] > findValue)
		{
			end = temp - 1;
		}
		else if (array[temp] < findValue)
		{
			strat = temp + 1;
		}
		else {
			return temp;
		}
	}
	return -1;
}

int main()
{
	int x = 1;
	short y = 44;
	swapA(x, y);

	//cout << x << " " << y << endl;

	int array[] = { 212,12,11,44,55	,6 };

	int length = sizeof(array) / sizeof(array[0]);
	
	shorMao(array, length);

	for (int i : array)
	{
		cout << i << endl;
	}
	
	cout << Find(array, length, 12) << endl;

	Student stu1;
	Student stu2;
	Student stu3;

	Student stu[] = { stu1 , stu2, stu3 };
	//shorMao(stu, 3);

	// 观察下面两个Sort方法的反汇编代码(看内存地址和内容)：
	int arr[] = { 2,6,1,5,4 };

	char arr1[] = { 2,6,1,5,4 };

	shorMao(arr, 5);

	shorMao(arr1, 5);

	return 0;
}