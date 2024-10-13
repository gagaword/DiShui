#pragma once

#include <iostream>
#include <windows.h>

using  namespace std;

// ������
class Monster
{
public:
	int ID;
	int Level;
	//std::string Name;
	char Name[20];
public:
	Monster() {}
	Monster(int ID, int Level, const char* Name)
	{
		this->ID = ID;
		this->Level = Level;
		//this->Name = Name;
		memcpy(&this->Name, Name, strlen(Name) + 1);
	}
};

template<class T>
class TreeNode {
public:
	T element;							//��ǰ�ڵ�洢������	
	TreeNode<T>* pLeft;					//ָ�����ӽڵ��ָ��	
	TreeNode<T>* pRight;				//ָ�����ӽڵ��ָ��	

	TreeNode(T& ele) {
		//��ʼ��Node�ڵ�					
		memset(&element, 0, sizeof(TreeNode));
		//ΪԪ�ظ�ֵ					
		memcpy(&element, &ele, sizeof(T));
		pLeft = pRight = NULL;
	}
};

template<class T>
class BSortTree {
public:
	BSortTree();					//���캯��	
	~BSortTree();					//��������	
public:
	void InOrderTraverse(TreeNode<T>* pNode);					//�������	
	void PreOrderTraverse(TreeNode<T>* pNode);					//ǰ�����	
	void PostOrderTraverse(TreeNode<T>* pNode);					//�������	
	TreeNode<T>* GetRoot();					//���ظ��ڵ�	
	int GetDepth(TreeNode<T>* pNode);		//����ĳ���ڵ�ĸ߶�/���	
private:
	void Init();
	void Clear(TreeNode<T>* pNode);
private:
	TreeNode<T>* m_pRoot;					//�����ָ��	
	int size;								//����Ԫ���ܸ���	
};

template<class T>
BSortTree<T>::BSortTree()
{
	Init();
}
template<class T>
BSortTree<T>::~BSortTree() {

	//�ͷ����Խڵ�ռ�	
	Clear(m_pRoot);
}

// ��ʼ������
template<class T>
void BSortTree<T>::Init()
{

	Monster m1(1, 1, "���");
	Monster m2(2, 2, "Ұ��");
	Monster m3(3, 3, "Ұ��");
	Monster m4(4, 4, "ʿ��");
	Monster m5(5, 5, "����");
	Monster m6(6, 6, "������");
	Monster m7(7, 7, "�������");


	TreeNode<Monster>* n1 = new TreeNode<Monster>(m1);
	TreeNode<Monster>* n2 = new TreeNode<Monster>(m2);
	TreeNode<Monster>* n3 = new TreeNode<Monster>(m3);
	TreeNode<Monster>* n4 = new TreeNode<Monster>(m4);
	TreeNode<Monster>* n5 = new TreeNode<Monster>(m5);
	TreeNode<Monster>* n6 = new TreeNode<Monster>(m6);
	TreeNode<Monster>* n7 = new TreeNode<Monster>(m7);

	m_pRoot = n5;
	n5->pLeft = n4;
	n5->pRight = n6;
	n4->pLeft = n1;
	n1->pRight = n2;
	n6->pLeft = n3;
	n3->pRight = n7;
	size = 7;

	/*
					5

				4		6

			1		    3

				 2		        7

	*/
}
template<class T>
TreeNode<T>* BSortTree<T>::GetRoot()
{
	return m_pRoot;
}

template<class T>
int BSortTree<T>::GetDepth(TreeNode<T>* pNode)
{
	if (pNode == NULL)
	{
		return 0;
	}
	else
	{
		int m = GetDepth(pNode->pLeft);
		int n = GetDepth(pNode->pRight);
		return (m > n) ? (m + 1) : (n + 1);
	}
}

template<class T>
void BSortTree<T>::InOrderTraverse(TreeNode<T>* pNode)
{

	// ����������й���,�г��ֵ�����						
	// �� �� ��
	char* pName = nullptr;
	if (pNode != NULL)
	{
		InOrderTraverse(pNode->pLeft);
		pName = (char*)&pNode->element;
		pName += 0x8;
		cout << pNode->element.Name << endl;
		InOrderTraverse(pNode->pRight);
	}
}

template<class T>
void BSortTree<T>::PreOrderTraverse(TreeNode<T>* pNode)
{

	// ǰ��������й���,�г��ֵ�����	
	// �� �� ��
	char* pName = nullptr;
	if (pNode != NULL)
	{
		pName = (char*)(&pNode->element);
		pName += 0x8;
		cout << pName << endl;
		PreOrderTraverse(pNode->pLeft);
		PreOrderTraverse(pNode->pRight);
	}
}

template<class T>
void BSortTree<T>::PostOrderTraverse(TreeNode<T>* pNode)
{

	//����������й���,�г��ֵ�����
	// �� �� ��
	char* pName = nullptr;
	if (pNode != NULL)
	{
		PostOrderTraverse(pNode->pLeft);
		PostOrderTraverse(pNode->pRight);
		pName = (char*)(&pNode->element);
		pName += 0x8;
		cout << pName << endl;
	}
}

template<class T>
void BSortTree<T>::Clear(TreeNode<T>* pNode)
{
	if (pNode != NULL)
	{
		Clear(pNode->pLeft);
		Clear(pNode->pRight);
		delete pNode;
	}
}