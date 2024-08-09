#include<iostream>	

void function(int x)
{
	switch (x)
	{
		case 1:
			printf("1");
			break;
		case 99:
			printf("5");
			break;
		case 98:
			printf("7");
			break;
		case 97:
			printf("9");
			break;
		case 96:
			printf("96");
			break;
	default:
		printf("Error");
		break;
	}
}

// do...while...
void doWhile(int x)
{
	do
	{
		printf("%d", x);
		++x;
	} while (x < 5);
}


// while
void whiles(int x )
{
	while (x < 5)
	{
		printf("%d", x);
		++x;
	}
}

void forr(int x) 
{
	for (int i = 0; i < x; i++)
	{
		printf("%d", x);
	}
}

int main()
{
	function(98);

	doWhile(0);

	whiles(0);

	forr(5);
	return 0;
}