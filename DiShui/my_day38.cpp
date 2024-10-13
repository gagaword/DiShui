#include "day38.h"
#include<iostream>
#include<Windows.h>

using namespace std;


void TestInsert()
{
	//12 8 5 9 17 15 13								
	/*
				12

		8				17

	5		9		15

				13

	*/

	BSortTree<int> tree;

	tree.Insert(12);
	tree.Insert(8);
	tree.Insert(5);
	tree.Insert(9);
	tree.Insert(17);
	tree.Insert(15);
	tree.Insert(13);
}

void TestSerch()
{
	//12 8 5 9 17 15 13								

	BSortTree<int> tree;

	tree.Insert(12);
	tree.Insert(8);
	tree.Insert(5);
	tree.Insert(9);
	tree.Insert(17);
	tree.Insert(15);
	tree.Insert(13);

	TreeNode<int>* p = tree.Search(17);

	printf("%x %d\n", p, p->element);
}


int main(int argc, char* argv[])
{
	
	return 0;
}