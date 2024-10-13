#pragma once

#include <iostream>
#include <windows.h>

using namespace std;


#define SUCCESS           			  1 // ִ�гɹ�						
#define ERROR			 -1 // ִ��ʧ��			         			

template<class T>
class TreeNode {
public:
	T element;					//��ǰ�ڵ�洢������			
	TreeNode<T>* pLeft;					//ָ�����ӽڵ��ָ��			
	TreeNode<T>* pRight;					//ָ�����ӽڵ��ָ��			
	TreeNode<T>* pParent;					//ָ�򸸽���ָ��			


	TreeNode(T& ele) {
		//��ʼ��Node�ڵ�							
		memset(&element, 0, sizeof(TreeNode));
		//ΪԪ�ظ�ֵ							
		memcpy(&element, &ele, sizeof(T));
		pLeft = pRight = pParent = NULL;
	}
	//����== �Ƚ�������Ƿ����								
	bool operator==(TreeNode<T>* node) {
		return node->element == element ? true : false;
	}
};

template<class T>
class BSortTree {
public:
	BSortTree();						//���캯��		
	~BSortTree();						//��������		
public:							//�ж����Ƿ�Ϊ��		
	bool IsEmpty();						//�����ڵ�		
	DWORD Insert(T element);						//ɾ���ڵ�		
	void Delete(T element);
	TreeNode<T>* Search(T element);						//���ҽڵ�		
	void InOrderTraverse(TreeNode<T>* pNode);						//�������		
	void PreOrderTraverse(TreeNode<T>* pNode);						//ǰ�����		
	void PostOrderTraverse(TreeNode<T>* pNode);						//�������		
private:
	TreeNode<T>* GetMaxNode(TreeNode<T>* pNode);						//��ȡ��pNodeΪ�������ڵ�		
	TreeNode<T>* GetMinNode(TreeNode<T>* pNode);						//��ȡ��pNodeΪ������С�ڵ�		
	TreeNode<T>* SearchNode(TreeNode<T>* pNode, T element);						//��ȡ��pNodeΪ������С�ڵ�		
	DWORD InsertNode(T element, TreeNode<T>* pNode);						//�����ڵ�		
	TreeNode<T>* DeleteNode(T element, TreeNode<T>* pNode);						//ɾ���ڵ�		
	void Clear(TreeNode<T>* pNode);						//������нڵ�		
private:
	TreeNode<T>* m_pRoot;						//�����ָ��		
	int size;						//����Ԫ���ܸ���		
};

template<class T>
BSortTree<T>::BSortTree()
{
	m_pRoot = NULL;
	size = 0;
}
template<class T>
BSortTree<T>::~BSortTree() {

	Clear(m_pRoot);
}
template<class T>
DWORD BSortTree<T>::Insert(T element)
{
	//������ڵ�Ϊ��								
	if (!m_pRoot)
	{
		m_pRoot = new TreeNode<T>(element);
		size++;
		return SUCCESS;
	}
	//������ڵ㲻Ϊ��								
	return InsertNode(element, m_pRoot);
}
template<class T>
DWORD BSortTree<T>::InsertNode(T element, TreeNode<T>* pNode)
{
	//��Ԫ�ط�װ���ڵ���								
	TreeNode<T>* pNewNode = new TreeNode<T>(element);
	//���element == ��ǰ�ڵ� ֱ�ӷ���								
	if (element == pNode->element)
	{
		return SUCCESS;
	}
	//���pNode�����ӽڵ�ΪNULL ����element < ��ǰ�ڵ�								
	if (pNode->pLeft == NULL && element < pNode->element)
	{
		pNode->pLeft = pNewNode;
		pNewNode->pParent = pNode;
		size++;
		return SUCCESS;
	}
	//���pNode�����ӽڵ�ΪNULL ����element > ��ǰ�ڵ�								
	if (pNode->pRight == NULL && element > pNode->element) {
		pNode->pRight = pNewNode;
		pNewNode->pParent = pNode;
		size++;
		return SUCCESS;
	}
	//���element<��ǰ�ڵ� �ҵ�ǰ�ڵ����������Ϊ��								
	if (element < pNode->element)
	{
		InsertNode(element, pNode->pLeft);
	}
	else
	{
		InsertNode(element, pNode->pRight);
	}
	return SUCCESS;
}

template<class T>
void BSortTree<T>::Clear(TreeNode<T>* pNode)
{
	if (pNode != NULL)
	{
		Clear(pNode->pLeft);
		Clear(pNode->pRight);
		delete pNode;
		pNode = NULL;
	}
}

template<class T>
bool BSortTree<T>::IsEmpty()
{
	return size == 0 ? true : false;
}

template<class T>
TreeNode<T>* BSortTree<T>::Search(T element)
{
	return SearchNode(m_pRoot, element);
}
template<class T>
TreeNode<T>* BSortTree<T>::SearchNode(TreeNode<T>* pNode, T element)
{
	if (pNode == NULL)					//����ڵ�ΪNULL			
	{
		return NULL;
	}
	else if (element == pNode->element)					//������			
	{
		return pNode;
	}					//����Ƚڵ��Ԫ��С ������			
	else if (element < pNode->element)
	{
		return SearchNode(pNode->pLeft, element);
	}
	else					//����Ƚڵ��Ԫ�ش� ������			
	{
		return SearchNode(pNode->pRight, element);
	}
}

template<class T>
void BSortTree<T>::Delete(T element)
{

}

template<class T>
TreeNode<T>* BSortTree<T>::DeleteNode(T element, TreeNode<T>* pNode)
{



	return NULL;
}




