#include<iostream>
#include<Windows.h>

using namespace std;

void Test(int& x, int& y)
{
	x = 3;
	y = 3;

}

class Student
{
public:
	Student();
	Student(int x, int y);
	~Student();

public:
	int x;
	int y;
};

Student::Student()
{
	this->x = 1;
	this->y = 1;
}
Student::Student(int x, int y)
{
	this->x = x;
	this->y = y;
}
Student::~Student()
{
	//NULL CODE
}


void TestStudent(Student& st)
{
	Student stu;
	st = stu;
	cout << st.x << " " << st.y << endl;

}


int main()
{
	int x = 2;
	int y = 2;
	Test(x, y);
	cout << x << " " << y << endl;

	Student stu{ 3,3 };
	TestStudent(stu);
	cout << stu.x <<  " " << stu.y << endl;
	return 0;
}