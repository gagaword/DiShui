#include <iostream>
#include <Windows.h>

using namespace std;


void Hello()
{
	printf("HelloWorld\n");
}

// 单继承无函数覆盖(打印Sub对象的虚函数表)
struct Base
{
public:
	virtual void Function_1()
	{
		printf("Base:Function_1...\n");
	}
	virtual void Function_2()
	{
		printf("Base:Function_2...\n");
	}
	virtual void Function_3()
	{
		printf("Base:Function_3...\n");
	}
};

struct Sub:Base
{
public:
	virtual void Function_4()
	{
		printf("Sub:Function_4...\n");
	}
	virtual void Function_5()
	{
		printf("Sub:Function_5...\n");
	}
	virtual void Function_6()
	{
		printf("Sub:Function_6...\n");
	}
};

// 虚函数-间接调用与直接调用
struct Persont
{
	int x;
	int y;

	Persont()
	{
		this->x = 1;
		this->y = 2;
	}

	// 普通函数
	void PuTongFunc()
	{
		printf("PuTongFunc()....\n");
	}

	// 虚函数
	virtual void virtualFunction_1()
	{
		printf("virtualFunction_1().....\n");
	}
};

// 单继承有函数覆盖(打印Sub对象的虚函数表)
struct Base1
{
public:
	virtual void Function_1()
	{
		printf("Base:Function_1...\n");
	}
	virtual void Function_2()
	{
		printf("Base:Function_2...\n");
	}
	virtual void Function_3()
	{
		printf("Base:Function_3...\n");
	}
};
struct Sub1 :Base1
{
public:
	virtual void Function_1()
	{
		printf("Sub:Function_1...\n");
	}
	virtual void Function_2()
	{
		printf("Sub:Function_2...\n");
	}
	virtual void Function_6()
	{
		printf("Sub:Function_6...\n");
	}
};


void Test_Persont()
{
	Persont persont;

	// 对象调用
	persont.PuTongFunc();
	//persont.virtualFunction_1();

	// 指针调用
	Persont* pPersont = &persont;
	pPersont->PuTongFunc();
	pPersont->virtualFunction_1();
	cout << sizeof(persont) << endl;

	// 通过函数指针调用虚函数
	typedef void(*FuncTion)(void);
	FuncTion fPn;
	fPn = (FuncTion)*((int*)*(int*)&persont);
	fPn();


}

// 打印sub的虚函数表
void Test_Sub()
{
	Sub sub;
	Sub* pSub = &sub;
	pSub->Function_1();
	
	typedef void(*FuncTion)(void);
	FuncTion fPn;

	// 尝试修改虚函数地址调用其他函数
	// 获取目标函数地址
	DWORD FuncAddress = (DWORD)&Hello;
	cout << hex << uppercase << FuncAddress << endl;
	fPn = (FuncTion)FuncAddress;
	fPn();


	for (int i = 0; i < 6; i++)
	{
		fPn = (FuncTion) * (((int*)*(int*)&sub) + i);
		// 虚函数地址
		cout << hex << uppercase << *(int*)(((int*)*(int*)&sub) + i) << endl;
		fPn();
	}

}

void Test_Sub1()
{
	Sub1 sub1;
	Sub1* pSub = &sub1;

	pSub->Function_1();
	pSub->Function_2();
	pSub->Function_3();
	pSub->Function_6();

	typedef void(*FuncTion)(void);
	FuncTion fPn;

	for (int i = 0; i < 4; i++)
	{
		fPn = (FuncTion) * (((int*)*(int*)&sub1) + i);
		// 虚函数地址
		cout << hex << uppercase << *(int*)(((int*)*(int*)&sub1) + i) << endl;
		fPn();
	}
}
int main()
{
	//Test_Persont();
	Test_Sub();
	//Test_Sub1();
	return 0;
}