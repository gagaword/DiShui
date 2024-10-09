#include "day35.h"

#include<iostream>
#include<Windows.h>

class Student
{
public:
	Student();
	~Student();

private:
	int x;
	int y;
	int z;
};

Student::Student()
{
}

Student::~Student()
{
}
void VectorTest()
{
	// auto* v = new Vectore<Student>(5);
	Vector<int>* v = new Vector<int>(5);
	v->push_back(1);
	v->push_back(2);
	v->push_back(3);
	v->push_back(4);
	v->push_back(5);
	v->push_back(6);

	v->pop_back();

	int result = 0;
	v->at(2, &result);
	cout << result << endl;

	
	v->insert(2, 9);

	cout << "还可以存储:" << v->capacity() << "个元素" << endl;

	//v->clear();

	cout << v->empty() << endl;

	cout << "还可以存储:" << v->capacity() << "个元素" << endl;

	try
	{
		v->erase(2);

	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "捕获到异常: " << e.what() << std::endl;
	}
	
	cout << v->size() << endl;
	
}

int main(int argc, char* argv[])
{
	int* i = (int*)malloc(sizeof(int)*1024);
	free(i);

	int* k = new int;
	delete k;

	VectorTest();
	return 0;
}

