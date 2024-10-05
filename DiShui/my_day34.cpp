#include<iostream>
#include<Windows.h>

using namespace std;

void Test(int& x, int& y)
{
	x = 3;
	y = 3;
}

void Test2(int* x, int* y)
{
	*x = 3;
	*y = 3;
}

class Student
{
public:
	Student();
	Student(int x);

	friend void print(Student& student);
	friend class Persone;

	friend Student operator+(const Student& _Left, const Student& _Right);
	friend Student operator-(const Student& _Left, const Student& _Right);
	friend Student operator*(const Student& _Left, const Student& _Right);
	friend Student operator/(const Student& _Left, const Student& _Right);

	friend bool operator>(const Student _Left, const Student& _Right);
	friend bool operator<(const Student _Left, const Student& _Right);
	friend bool operator>=(const Student _Left, const Student& _Right);
	friend bool operator<=(const Student _Left, const Student& _Right);


	// 输出函数，方便打印结果
	friend ostream& operator<<(ostream& os, const Student& num);

private:
	 int x;
};

Student operator+(const Student& _Left, const Student& _Right)
{
	return Student(_Left.x + _Right.x);
}
Student operator-(const Student& _Left, const Student& _Right)
{
	return Student(_Left.x - _Right.x);
}
Student operator*(const Student& _Left, const Student& _Right)
{
	return Student(_Left.x * _Right.x);
}
Student operator/(const Student& _Left, const Student& _Right)
{
	if (_Right.x == 0)
	{
		throw runtime_error("除数不能为0");
	}
	return Student(_Left.x / _Right.x);
}

bool operator>(const Student _Left, const Student& _Right)
{
	return _Left.x > _Right.x;
}

bool operator<(const Student _Left, const Student& _Right)
{
	return _Left.x < _Right.x;
}

bool operator>=(const Student _Left, const Student& _Right)
{
	return _Left.x >= _Right.x;
}

bool operator<=(const Student _Left, const Student& _Right)
{
	return _Left.x <= _Right.x;
}

// 输出友元函数重载
ostream& operator<<(ostream& os, const Student& num) {
	os << num.x;
	return os;
}


void print(Student& student)
{
	cout << student.x  << endl;
}

Student::Student()
{
	this->x = 2;
	
}
Student::Student(int x)
{
	this->x = x;
}


class Persone
{
public:
	Persone();
	void print(Student& stud);

public:
	int x;
};

void Persone::print(Student& stud)
{
	cout << hex << stud.x << endl;
}

Persone::Persone()
{
	this->x = 10;
}


void TestStudent(Student& st)
{
	Student stu;
	st = stu;
	//cout << st.x << endl;
}

int main()
{
	int x = 2;
	int y = 2;
	Test(x, y);
	cout << x << " " << y << endl;

	Test2(&x, &y);
	cout << x << " " << y << endl;


	Student stu1{ 3 };
	//TestStudent(stu);
	//cout << stu.x <<  " " << stu.y << endl;


	/*print(stu1);

	Persone per;
	per.print(stu1);*/

	Student stu2{ 1 };

	//cout << stu1 + stu2 << endl;
	printf("%d\n", stu1 < stu2);
	return 0;
}