#pragma once

#include <iostream>
#include <Windows.h>

#define SUCCESS           1 // 执行成功									
#define ERROR_MY          -1 // 执行失败									
#define INDEX_IS_ERROR   -2 // 错误的索引号									
#define BUFFER_IS_EMPTY  -3 // 缓冲区已空									

using namespace std;

template <class T_ELE>
class LinkedList
{
public:
	LinkedList();
	~LinkedList();
public:
	BOOL  IsEmpty();															//判断链表是否为空 空返回1 非空返回0		
	void  Clear();																//清空链表		
	DWORD GetElement(IN DWORD dwIndex, OUT T_ELE& Element);						//根据索引获取元素		
	DWORD GetElementIndex(IN T_ELE& Element);									//根据元素获取链表中的索引		
	DWORD Insert(IN T_ELE Element);												//新增元素		
	DWORD Insert(IN DWORD dwIndex, IN T_ELE Element);							//根据索引新增元素		
	DWORD Delete(IN DWORD dwIndex);												//根据索引删除元素		
	DWORD GetSize();															//获取链表中元素的数量		
private:
	typedef struct _NODE
	{
		T_ELE  Data;
		_NODE* pNext;
	}NODE, *PNODE;
	PNODE GetIndexCurrentNode(DWORD dwIndex);						//获取索引为dwIndex的指针		
	PNODE GetIndexPreviousNode(DWORD dwIndex);						//获取索引为dwIndex的前一个节点指针		
	PNODE GetIndexNextNode(DWORD dwIndex);							//获取索引为dwIndex的后一个节点指针		
private:
	PNODE m_pList;							//链表头指针，指向第一个节点		
	DWORD m_dwLength;						//元素的数量		
};

//无参构造函数 初始化成员									
template<class T_ELE> LinkedList<T_ELE>::LinkedList()
	:m_pList(NULL), m_dwLength(0)
{

}

//析构函数 清空元素									
template<class T_ELE> LinkedList<T_ELE>::~LinkedList()
{
	Clear();
}

//判断链表是否为空									
template<class T_ELE> BOOL LinkedList<T_ELE>::IsEmpty()
{
	if (m_pList == NULL || m_dwLength == 0)
		return true;
	return false;
	
}

//清空链表									
template<class T_ELE> void LinkedList<T_ELE>::Clear()
{
	// 1. 判断链表是否为空
	if (IsEmpty())
	{
		return;
	}
	// 2. 循环删除链表中的节点		
	PNODE pnode = m_pList;
	while (pnode->pNext)
	{
		PNODE temp_Node = pnode;
		pnode = pnode->pNext;
		delete temp_Node;
	}
	// 3. 删除最后一个节点并将链表长度置为0	
	delete pnode;
	m_pList = NULL;
	m_dwLength = 0;
}

//根据索引获取元素									
template<class T_ELE> DWORD LinkedList<T_ELE>::GetElement(IN DWORD dwIndex, OUT T_ELE& Element)
{
	// 1. 判断索引是否有效		
	if (dwIndex >= m_dwLength || dwIndex < 0)
	{
		return INDEX_IS_ERROR;
	}

	// 2. 取得索引指向的节点	
	PNODE pnode = m_pList;
	int index = 0;
	while (pnode->pNext != NULL)
	{
		if (index == dwIndex)
		{
			Element = pnode->Data;
			return SUCCESS;
		}
		// 移动至下一个节点
		pnode = pnode->pNext;
		index++;
	}

	return ERROR_MY;
}

//根据元素内容获取索引									
template<class T_ELE> DWORD LinkedList<T_ELE>::GetElementIndex(IN T_ELE& Element)
{
	// 1. 判断链表是否为空
	if (IsEmpty())
	{
		return BUFFER_IS_EMPTY;
	}

	// 2. 循环遍历链表，找到与Element相同的元素		
	PNODE pnode = m_pList;
	int index = 0;
	while (pnode)
	{
		if (pnode->Data == Element)
		{
			return index;
		}
		pnode = pnode->pNext;
		index++;
	}
	return ERROR_MY;
}


//在链表尾部新增节点									
template<class T_ELE> DWORD LinkedList<T_ELE>::Insert(IN T_ELE Element)
{
	// 1. 判断链表是否为空			
	if (m_pList == NULL)
	{
		m_pList = new NODE();
		if (!m_pList)
		{
			return ERROR_MY;
		}
		m_pList->Data = Element;
		m_pList->pNext = NULL;
		m_dwLength++;

		return SUCCESS;
	}

	// 2. 如果链表中已经有元素
	PNODE pnode = m_pList;

	// 找到链表中尾元素位置
	while (pnode->pNext != NULL)
	{
		// 新增一个新的节点，使得pnode指向最后一个节点
		pnode = pnode->pNext;
	}
	PNODE new_pNode = new NODE();
	new_pNode->Data = Element;
	new_pNode->pNext = NULL;
	pnode->pNext = new_pNode;
	m_dwLength++;

	return SUCCESS;
}

//将节点新增到指定索引的位置						0 1 7 2 3 4			
template<class T_ELE> DWORD LinkedList<T_ELE>::Insert(IN DWORD dwIndex, IN T_ELE Element)
{

	//  1. 判断链表是否为空		
	if (IsEmpty())
	{
		return ERROR_MY;
	}

	//  2. 判断索引值是否有效		
	if (dwIndex > m_dwLength || dwIndex < 0)
	{
		return INDEX_IS_ERROR;
	}

	PNODE new_pnode = new NODE();
	new_pnode->Data = Element;

	//  3. 如果索引为0	
	PNODE pnode = m_pList;
	if (dwIndex == 0)
	{
		new_pnode->pNext = pnode;
		m_pList = new_pnode;
		m_dwLength++;
		return SUCCESS;
	}

	//  4. 如果索引为链表尾	
	if (dwIndex == m_dwLength)
	{
		
		// 找到链表尾
		while (pnode->pNext != NULL)
		{
			new_pnode->pNext = pnode;
			pnode->pNext = NULL;
			m_dwLength++;
			pnode = pnode->pNext;
		}
	}

	//  5. 如果索引为链表中		
	if (dwIndex > 0 && dwIndex < m_dwLength)
	{
		PNODE index_Previous_pnode = GetIndexPreviousNode(dwIndex);
		PNODE index_Current_pnode = GetIndexCurrentNode(dwIndex);
		index_Previous_pnode->pNext = new_pnode;
		new_pnode->pNext = index_Current_pnode;
		m_dwLength++;
		return SUCCESS;
	}
}

//根据索引删除节点									
template<class T_ELE> DWORD LinkedList<T_ELE>::Delete(IN DWORD dwIndex)
{
	//  1. 判断链表是否为空		
	if (IsEmpty())
	{
		return 1;
	}

	//  2. 判断索引值是否有效								
	if (dwIndex < 0 || dwIndex > m_dwLength)
	{
		return -1;
	}

	//  3. 如果链表中只有头节点，且要删除头节点		
	if (dwIndex = 0 && m_dwLength == 1)
	{
		delete m_pList;
		m_pList = NULL;
		m_dwLength--;
		return SUCCESS;
	}


	//  4. 如果要删除头节点		
	PNODE pnode = m_pList;
	if (dwIndex == 0)
	{
		PNODE new_node = pnode;
		PNODE index_next_node = GetIndexNextNode(dwIndex);
		m_pList = index_next_node;
		delete new_node;
		m_dwLength--;
		return SUCCESS;
	}

	//  5. 如果是其他情况		
	PNODE index_previous_node = GetIndexPreviousNode(dwIndex);
	PNODE index_next_node = GetIndexNextNode(dwIndex);
	PNODE index_is_node = GetIndexCurrentNode(dwIndex);
	delete index_is_node;
	index_previous_node->pNext = index_next_node;
}


//获取链表中节点的数量									
template<class T_ELE> DWORD LinkedList<T_ELE>::GetSize()
{
	return m_dwLength;
}


//获取dwIndex前面节点的地址									
template<class T_ELE>
typename LinkedList<T_ELE>::PNODE LinkedList<T_ELE>::GetIndexPreviousNode(DWORD dwIndex)
{
	if (dwIndex > m_dwLength)
	{
		return nullptr;
	}
	PNODE pnode = m_pList;
	int index = 0;
	while (pnode->pNext != NULL)
	{
		if (index == dwIndex - 1)
		{
			return pnode;
			break;
		}
		index++;
		pnode = pnode->pNext;
	}
	return nullptr;
}

//获取dwIndex节点的地址									
template<class T_ELE>
typename LinkedList<T_ELE>::PNODE LinkedList<T_ELE>::GetIndexCurrentNode(DWORD dwIndex)
{
	if (dwIndex > m_dwLength)
	{
		return 0;
	}
	PNODE pnode = m_pList;
	for (int index = 0; index <= m_dwLength - 1; index++)
	{
		if (index == dwIndex)
		{
			return pnode;
			break;
		}
		pnode = pnode->pNext;
	}
	return 0;
}

//获取dwIndex后面节点的地址									
template<class T_ELE>
typename LinkedList<T_ELE>::PNODE LinkedList<T_ELE>::GetIndexNextNode(DWORD dwIndex)
{							
	if (dwIndex > m_dwLength)
	{
		return 0;
	}
	PNODE pnode = m_pList;
	for (int index = 0; index <= m_dwLength - 1; index++)
	{
		if (index == dwIndex + 1)
		{
			return pnode;
			break;
		}
		pnode = pnode->pNext;
	}
	return 0;
}

