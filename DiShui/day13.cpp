#include<iostream>	

// 结构体定义
struct coord
{
	int x;
	int y;
	int h;
};


// 结构体定义
struct roleInfo
{
	int age;
	int sm;
	int mofa;
	int jy;
	char roleName[20];
};

// 全局变量
roleInfo testRolr;

// 结构体赋值
void roleInfoFuZhi()
{

	testRolr.age = 20;
	testRolr.mofa = 100;
	testRolr.jy = 200;
	testRolr.sm = 100;
	testRolr.roleName[0] = 1;
}

// 结构体读取
void readRoleInfo()
{
	printf("%d %d %d %d\n", testRolr.age, testRolr.mofa, testRolr.jy, testRolr.sm);
}
// 结构体作为参数时传递参数
void testStructParr(coord cd)
{
	
}

// 结构体作为返回值
coord testScructRet() 
{
	coord cd;
	cd.x = 10;
	cd.y = 20;
	cd.h = 30;
	return cd;
}


/*
	1、定义一个结构体Gamer用来存储一个游戏中的角色的信息，包括血值、等级、坐标等信息						
						
要求：						
						
	(1) 具体包含哪些信息自由设计					
						
	(2) 但这些包含的类型中，必须要有一个成员是结构体类型					
*/
struct Gamer
{
	int hp;
	int gongJiLi;
	int yiDong;
	int grade;
	coord cord;
	char roleName[20];
};

Gamer gamer;

// 2、定义一个函数，用来给这个结构体变量赋值.
void GamerInfo() 
{
	gamer.hp = 100;
	gamer.gongJiLi = 2000;
	gamer.yiDong = 300;
	gamer.grade = 20;
	gamer.roleName[0] = 'A';
	gamer.cord.x = 222;
	gamer.cord.y = 333;
	gamer.cord.h = 444;
}

//3、定义一个函数，用来显示这个结构体变量的所有成员信息.
void PrintfGamer()
{
	printf("%d %d %d %d %d %d %d %c", gamer.hp, gamer.gongJiLi, gamer.yiDong, gamer.grade, gamer.cord.x, gamer.cord.y, gamer.cord.h, gamer.roleName[0]);
}

int main()
{
	roleInfoFuZhi();
	readRoleInfo();

	// 传递参数
	coord cd;
	cd.x = 10;
	cd.y = 20;
	cd.h = 30;
	testStructParr(cd);

	// 结构体作为返回值
	coord cd2 = testScructRet();


	GamerInfo();
	PrintfGamer();
	return 0;
}