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
	printf("%d\n",k[0][0]);

	printf("%p\n", *(k + 2));
	printf("%p\n", k[2]);

	printf("---------------------\n");
	printf("%d\n", *(*(*s)));
	printf("%d\n", *(*(*s + 0)));
	printf("%d\n", s[0][0][0]);

	printf("---------------------\n");

	// 数组指针
	int ar[] = { 1,2,3,4,5 };
	// 这两种代码在实际使用中效果一样，但是他们的类型是不同的，&ar表取ar的地址即整个数组，而ar只是表示取数组首个元素的地址
	int(*array)[5] = (int(*)[5]) & ar;
	//int(*array)[5] = (int(*)[5])ar;

	// 这两行代码也论证了ar的地址就是数组首个元素的地址
	printf("ar的地址:%p\n", &ar);
	printf("ar[0]的地址:%p\n", &ar[0]);
	printf("ar[1]的地址:%p\n", &ar[1]);


	// array与*array保存的值是一样的，都是ar数组元素的首地址，但是在实际取值的时候还是要通过**取值
	// 这里看似是与*array是一样的值，但实际上array保存的是ar的地址，而ar的地址被看作是数组元素的首地址
	// 而*array是取ar地址指向的数据的地址(就是数组元素的首地址了，还是本身)。
	printf("arra保存的数据:%p\n", array); 
	// 此时array是数组元素首地址在添加一层取值可以取到这个元素
	printf("解引用array后数据:%p\n", *array);

	printf("%d\n", *(*(array) + 1));
	printf("*(array)[1]--->%d\n", (*array)[1]);
	printf("*(array) + 1):%d\n", *(*(array) + 4));
	printf("%d\n", sizeof(array));

	//array++; // 加4
	printf("%p\n", array);

	// 通过数组指针获取到数组元素
	printf("%d\n", *((*(array) + 2) + 1));

	// array[0]获取到数组首元素地址在通过[1]偏移取到值
	// 只能通过array[0]获取到首元素地址，如果加1的话array指向的就不是这个数组了。
	printf("%d\n", array[0][1]);
	printf("%d\n", array[0][2]);

	int arra[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
	int(*arrP)[2] = (int(*)[2]) & arra;

	printf("arra[2]:%p\n", &arra[2]);
	printf("(arrP + 1):%p\n", arrP + 1);

	printf("%d\n", *(*(arrP + 0) + 2));
	printf("%d\n", *(*(arrP + 1) + 1)); // 4
	
	printf("%d\n", *(*(arrP + 3) + 4)); // 11

	/*int xx = (*(arrP + 1)[2]);
	int ss = arrP[1][2];*/

	printf("*(arrP + 1)[2]->%d\n", (*(arrP + 1)[2]));

	// 那*(p+1)[2] 是否一定等于p[1][2]呢？ 通过反汇编进行论证。
	//printf("%d\n", arrP[1][2]);
	printf("%d\n", *(arrP + 1)[2]);

	printf("%p\n", &arra[6]);
	printf("%p\n", (arrP + 1)[2]);

	// 使用数组指针遍历一个一维数组
	int data[] = { 12,123,6,45,1,2,3123,2313,5,6,6,7,8 };
	int(*dataP)[1] = (int(*)[1]) & data;
	for (int i = 0; i < sizeof(data) / sizeof(data[0]); i++)
	{
		//printf("%d ", *(*(dataP++)));
		printf("%d ", (*(dataP)[i]));
	}
}
int main()
{
	poniter();
	return 0;
}