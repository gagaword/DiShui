#pragma once

#include <iostream>
#include <windows.h>

using namespace std;


#define SUCCESS           			  1 // 执行成功						
#define ERROR			 -1 // 执行失败			         			

template<class T>
class TreeNode {
public:
	T element;					//当前节点存储的数据			
	TreeNode<T>* pLeft;					//指向左子节点的指针			
	TreeNode<T>* pRight;					//指向右子节点的指针			
	TreeNode<T>* pParent;					//指向父结点的指针			


	TreeNode(T& ele) {
		//初始化Node节点							
		memset(&element, 0, sizeof(TreeNode));
		//为元素赋值							
		memcpy(&element, &ele, sizeof(T));
		pLeft = pRight = pParent = NULL;
	}
	//重载== 比较两结点是否相等								
	bool operator==(TreeNode<T>* node) {
		return node->element == element ? true : false;
	}
};

template<class T>
class BSortTree {
public:
	BSortTree();						//构造函数		
	~BSortTree();						//析构函数		
public:							//判断树是否为空		
	bool IsEmpty();						//新增节点		
	DWORD Insert(T element);						//删除节点		
	void Delete(T element);
	TreeNode<T>* Search(T element);						//查找节点		
	void InOrderTraverse(TreeNode<T>* pNode);						//中序遍历		
	void PreOrderTraverse(TreeNode<T>* pNode);						//前序遍历		
	void PostOrderTraverse(TreeNode<T>* pNode);						//后序遍历		
private:
	TreeNode<T>* GetMaxNode(TreeNode<T>* pNode);						//获取以pNode为根的最大节点		
	TreeNode<T>* GetMinNode(TreeNode<T>* pNode);						//获取以pNode为根的最小节点		
	TreeNode<T>* SearchNode(TreeNode<T>* pNode, T element);						//获取以pNode为根的最小节点		
	DWORD InsertNode(T element, TreeNode<T>* pNode);						//新增节点		
	TreeNode<T>* DeleteNode(T element, TreeNode<T>* pNode);						//删除节点		
	void Clear(TreeNode<T>* pNode);						//清空所有节点		
private:
	TreeNode<T>* m_pRoot;						//根结点指针		
	int size;						//树中元素总个数		
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
	//如果根节点为空								
	if (!m_pRoot)
	{
		m_pRoot = new TreeNode<T>(element);
		size++;
		return SUCCESS;
	}
	//如果根节点不为空								
	return InsertNode(element, m_pRoot);
}
template<class T>
DWORD BSortTree<T>::InsertNode(T element, TreeNode<T>* pNode)
{
	//将元素封装到节点中								
	TreeNode<T>* pNewNode = new TreeNode<T>(element);
	//如果element == 当前节点 直接返回								
	if (element == pNode->element)
	{
		return SUCCESS;
	}
	//如果pNode的左子节点为NULL 并且element < 当前节点								
	if (pNode->pLeft == NULL && element < pNode->element)
	{
		pNode->pLeft = pNewNode;
		pNewNode->pParent = pNode;
		size++;
		return SUCCESS;
	}
	//如果pNode的右子节点为NULL 并且element > 当前节点								
	if (pNode->pRight == NULL && element > pNode->element) {
		pNode->pRight = pNewNode;
		pNewNode->pParent = pNode;
		size++;
		return SUCCESS;
	}
	//如果element<当前节点 且当前节点的左子树不为空								
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
	if (pNode == NULL)					//如果节点为NULL			
	{
		return NULL;
	}
	else if (element == pNode->element)					//如果相等			
	{
		return pNode;
	}					//如果比节点的元素小 向左找			
	else if (element < pNode->element)
	{
		return SearchNode(pNode->pLeft, element);
	}
	else					//如果比节点的元素大 向右找			
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




