#include "day37.h"
#include<iostream>
#include<Windows.h>


using namespace std;

void TreeTest()
{
	BSortTree<Monster>* monster = new BSortTree<Monster>;
	
	cout << "GetDepth:" << monster->GetDepth(monster->GetRoot()) << endl;
	cout << "========" << endl;

	monster->InOrderTraverse(monster->GetRoot());
	cout << "========" << endl;

	monster->PreOrderTraverse(monster->GetRoot());
	cout << "========" << endl;


	monster->PostOrderTraverse(monster->GetRoot());

	//monster->~BSortTree();

	
}
int main(int argc, char* argv[])
{
	TreeTest();
	return 0;
}
