#include<iostream>
#include <cstdint>
#include <Windows.h>

using namespace std;

struct MyStruct
{

};

//struct Person
//{
//	void Fn_1()
//	{
//		printf("Person:Fn_1()\n");
//	}
//	void Fn_2()
//	{
//		printf("Person:Fn_2()\n");
//	}
//};


struct Test
{
	int x;
	int y;

	int add(int x, int y)
	{
		this->x = x;
		this->y = y;
		return this->x + this->y;
	}

	int sub()
	{
		return x - y;
	}

	int mul()
	{
		return x * y;
	}
	int div()
	{
		return x / y;
	}

	uintptr_t GetAddress() {
		return reinterpret_cast<uintptr_t>(this);
	}
};

struct Person
{
	int x;
	void Fn_1()
	{
		printf("Person:Fn_1()\n");
	}
	void Fn_2()
	{
		x = 10;
		printf("Person:Fn_2()%x\n");
	}
};

int main()
{
	Test test;
	test.x = 1;
	test.y = 2;

	MyStruct myStruct;
	cout << hex << sizeof(myStruct) << endl;

	cout << hex << test.GetAddress() << endl;

	test.add(2,3);

	/*Person* p = NULL;
	p->Fn_1();
	p->Fn_2();*/

	Person* p = NULL;

	p->Fn_1();
	p->Fn_2();



	return 0;
}