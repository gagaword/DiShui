#include "day36.h"
#include<iostream>
#include<Windows.h>

using namespace std;

void TestLink()
{
	LinkedList<int>* test = new LinkedList<int>;
	test->Insert(2);
	test->Insert(3);
	test->Insert(4);
	test->Insert(5);

	test->Insert(3,7);

	cout << test->GetSize() << endl;

	cout << test->GetSize() << endl;

	test->Delete(1);

}
int main(int argc, char* argv[])
{
	TestLink();
	return 0;
}