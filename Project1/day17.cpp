#include<iostream>

void poniter()
{
	// 一级指针
	int x = 10;
	int* p = &x; // p保存的x的地址
	printf("%p %d\n", p, *p);

	// 二级指针
	int a = 30;
	int* aP = &a; // aP保存了a的地址
	int** aPP = &aP; // aPP保存了aP的地址
	// *aPP取到的是aP保存的数据，而aP保存的数据就是a的地址，所以需要在取值一次
	// aPP保存的是aP的地址，而不是aP保存的数据。
	// 
	printf("aPP--->%p *aPP--->%p *(*(aPP))--->%d\n", aPP, *aPP,*(*(aPP)));

	// 三级指针
	int b = 40;
	int* bP = &b;
	int** bPP = &bP;
	int*** bPPP = &bPP;

	printf("---------------------\n");
	printf("b--->%p\n", &b);
	printf("bP-->%p\n", &bP);
	printf("bPP->%p\n", &bPP);
	printf("---------------------\n");

	// bPPP-->bPP的地址
	printf("bPPP---->%p\n", bPPP);

	// *bPPP--->bPP的地址--->bP的地址
	printf("*bPPP--->%p\n", *bPPP);

	// *bPPP--->bP的地址+*---->b的地址
	printf("*(*(bPP)):%p\n", *(*(bPPP)));
	
	// b的地址---->40
	printf("*(*(*bPPP)):%d\n", *(*(*bPPP)));
	printf("---------------------\n");
	printf("---------------------\n");

	// *的反汇编与[]的关系
	int v = 20;
	int* m = &v;
	int** k = &m;
	int*** s = &k;
	printf("%p\n", m);

	printf("%d\n", *m);
	printf("%d\n", *(m + 0));
	printf("%d\n", m[0]);

	printf("---------------------\n");

	printf("%d\n", *(*k));
	printf("%d\n", *(*k + 0));
	printf("%d",k[0][0]);

}
int main()
{
	poniter();
	return 0;
}