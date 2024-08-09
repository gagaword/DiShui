#include<iostream>	

//typedef struct Student
//{
//	int age;
//	int score;
//}stu;
//
//
//stu* stu1;

struct Student
{
	int x;
	int y;
};


void test()
{
	char** arra[10] = {0};
	Student**** s;
	s = (Student****)100;
	s++;				// s = 104
	printf("%d\n", s);

	s = s + 2;			// s = 104+8=112
	printf("%d\n", s);

	s = s - 3;			// s = 112-12=100
	printf("%d\n", s);
	printf("-----------------\n");

	Student**** s1;
	Student**** s2;
	int x;
	s1 = (Student****)200;
	s2 = (Student****)100;
	x = s1 - s2;		// 25
	printf("%d\n", x);
	printf("-----------------\n");

	Student* ss;
	ss = (Student*)100;
	ss++;			// 108
	printf("%d\n", ss);

	ss = ss + 2;	// ss = 108 + 16 = 124
	printf("%d\n", ss);

	ss = ss - 3;	// ss = 124 - 24 = 100
	printf("%d\n", ss);
	printf("-----------------\n");

	Student* ss1;
	Student* ss2;
	x = 0;
	ss1 = (Student*)200;
	ss2 = (Student*)100;
	x = ss1 - ss2; // 12
	printf("%d\n", x);


	/*int* a;
	char* b;
	short* c;
	a = (int*)1;
	b = (char*)2;
	c = (short*)3;
	stu1 = (stu*)4;*/
	/*int* a = (int*)10;
	char* b = (char*)20;
	a++;
	b++;*/

	/*int** a = (int**)10;
	char** b = (char**)20;
	a++;
	b++;
	*/

	/*int* a = (int*)10;
	int* b = (int*)20;
	char* c = (char*)30;
	a = a + 5;
	b = b + 5;
	c = c + 5;
	printf("%d\n", a);
	printf("%d\n", b);
	printf("%d\n", c);*/

	/*int* a = (int*)10;
	char* b = (char*)20;
	int x = a - b;*/

	/*结果是-3，暂时不理解
	int* a = (int*)10;
	int* b = (int*)20;
	int x = a - b;
	printf("%d\n", x);
	*/

	/*int* a = (int*)10;
	int* b = (int*)20;
	if (a > b)
	{
		printf("1");
	}
	else
	{
		printf("2");
	}*/

	/*int* a = (int*)10;
	int* b = (int*)20;
	char* c = (char*)30;
	a = a + 5.5;
	b = b + 5;
	c = c + 5;
	printf("%d\n", a);
	printf("%d\n", b);
	printf("%d\n", c);*/

}
int main()
{
	test();
	return 0;
}