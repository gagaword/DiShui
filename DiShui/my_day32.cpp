#include "iostream"
#include "Windows.h"

using namespace std;

struct Sub
{
	int x;
	Sub()
	{
		this->x = 100;
	}
	void Fun1()
	{
		cout << "Sub_1_Fun1" << endl;
	}
	virtual void Fun2()
	{
		cout << "Sub_2_Fun2" << endl;
	}
};

struct Base1:Sub
{
	int x;
	Base1()
	{
		this->x = 200;
	}

	void Fun1()
	{
		cout << "Base1_1_Fun1" << endl;

	}
	virtual void Fun2()
	{
		cout << "Base2_Fun2" << endl;
	}
};

void Test(Sub* sub)
{
	int n = sub->x;
	cout << n << endl;

	sub->Fun1();
	sub->Fun2();
}

int main()
{
	Sub sub;
	Base1 base1;
	//Test(&base1);


	return 0;
}

