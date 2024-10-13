#pragma once

#include <iostream>
#include <Windows.h>

#define SUCCESS           1 // ִ�гɹ�									
#define ERROR_MY          -1 // ִ��ʧ��									
#define INDEX_IS_ERROR   -2 // �����������									
#define BUFFER_IS_EMPTY  -3 // �������ѿ�									

using namespace std;

template <class T_ELE>
class LinkedList
{
public:
	LinkedList();
	~LinkedList();
public:
	BOOL  IsEmpty();															//�ж������Ƿ�Ϊ�� �շ���1 �ǿշ���0		
	void  Clear();																//�������		
	DWORD GetElement(IN DWORD dwIndex, OUT T_ELE& Element);						//����������ȡԪ��		
	DWORD GetElementIndex(IN T_ELE& Element);									//����Ԫ�ػ�ȡ�����е�����		
	DWORD Insert(IN T_ELE Element);												//����Ԫ��		
	DWORD Insert(IN DWORD dwIndex, IN T_ELE Element);							//������������Ԫ��		
	DWORD Delete(IN DWORD dwIndex);												//��������ɾ��Ԫ��		
	DWORD GetSize();															//��ȡ������Ԫ�ص�����		
private:
	typedef struct _NODE
	{
		T_ELE  Data;
		_NODE* pNext;
	}NODE, *PNODE;
	PNODE GetIndexCurrentNode(DWORD dwIndex);						//��ȡ����ΪdwIndex��ָ��		
	PNODE GetIndexPreviousNode(DWORD dwIndex);						//��ȡ����ΪdwIndex��ǰһ���ڵ�ָ��		
	PNODE GetIndexNextNode(DWORD dwIndex);							//��ȡ����ΪdwIndex�ĺ�һ���ڵ�ָ��		
private:
	PNODE m_pList;							//����ͷָ�룬ָ���һ���ڵ�		
	DWORD m_dwLength;						//Ԫ�ص�����		
};

//�޲ι��캯�� ��ʼ����Ա									
template<class T_ELE> LinkedList<T_ELE>::LinkedList()
	:m_pList(NULL), m_dwLength(0)
{

}

//�������� ���Ԫ��									
template<class T_ELE> LinkedList<T_ELE>::~LinkedList()
{
	Clear();
}

//�ж������Ƿ�Ϊ��									
template<class T_ELE> BOOL LinkedList<T_ELE>::IsEmpty()
{
	if (m_pList == NULL || m_dwLength == 0)
		return true;
	return false;
	
}

//�������									
template<class T_ELE> void LinkedList<T_ELE>::Clear()
{
	// 1. �ж������Ƿ�Ϊ��
	if (IsEmpty())
	{
		return;
	}
	// 2. ѭ��ɾ�������еĽڵ�		
	PNODE pnode = m_pList;
	while (pnode->pNext)
	{
		PNODE temp_Node = pnode;
		pnode = pnode->pNext;
		delete temp_Node;
	}
	// 3. ɾ�����һ���ڵ㲢����������Ϊ0	
	delete pnode;
	m_pList = NULL;
	m_dwLength = 0;
}

//����������ȡԪ��									
template<class T_ELE> DWORD LinkedList<T_ELE>::GetElement(IN DWORD dwIndex, OUT T_ELE& Element)
{
	// 1. �ж������Ƿ���Ч		
	if (dwIndex >= m_dwLength || dwIndex < 0)
	{
		return INDEX_IS_ERROR;
	}

	// 2. ȡ������ָ��Ľڵ�	
	PNODE pnode = m_pList;
	int index = 0;
	while (pnode->pNext != NULL)
	{
		if (index == dwIndex)
		{
			Element = pnode->Data;
			return SUCCESS;
		}
		// �ƶ�����һ���ڵ�
		pnode = pnode->pNext;
		index++;
	}

	return ERROR_MY;
}

//����Ԫ�����ݻ�ȡ����									
template<class T_ELE> DWORD LinkedList<T_ELE>::GetElementIndex(IN T_ELE& Element)
{
	// 1. �ж������Ƿ�Ϊ��
	if (IsEmpty())
	{
		return BUFFER_IS_EMPTY;
	}

	// 2. ѭ�����������ҵ���Element��ͬ��Ԫ��		
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


//������β�������ڵ�									
template<class T_ELE> DWORD LinkedList<T_ELE>::Insert(IN T_ELE Element)
{
	// 1. �ж������Ƿ�Ϊ��			
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

	// 2. ����������Ѿ���Ԫ��
	PNODE pnode = m_pList;

	// �ҵ�������βԪ��λ��
	while (pnode->pNext != NULL)
	{
		// ����һ���µĽڵ㣬ʹ��pnodeָ�����һ���ڵ�
		pnode = pnode->pNext;
	}
	PNODE new_pNode = new NODE();
	new_pNode->Data = Element;
	new_pNode->pNext = NULL;
	pnode->pNext = new_pNode;
	m_dwLength++;

	return SUCCESS;
}

//���ڵ�������ָ��������λ��						0 1 7 2 3 4			
template<class T_ELE> DWORD LinkedList<T_ELE>::Insert(IN DWORD dwIndex, IN T_ELE Element)
{

	//  1. �ж������Ƿ�Ϊ��		
	if (IsEmpty())
	{
		return ERROR_MY;
	}

	//  2. �ж�����ֵ�Ƿ���Ч		
	if (dwIndex > m_dwLength || dwIndex < 0)
	{
		return INDEX_IS_ERROR;
	}

	PNODE new_pnode = new NODE();
	new_pnode->Data = Element;

	//  3. �������Ϊ0	
	PNODE pnode = m_pList;
	if (dwIndex == 0)
	{
		new_pnode->pNext = pnode;
		m_pList = new_pnode;
		m_dwLength++;
		return SUCCESS;
	}

	//  4. �������Ϊ����β	
	if (dwIndex == m_dwLength)
	{
		
		// �ҵ�����β
		while (pnode->pNext != NULL)
		{
			new_pnode->pNext = pnode;
			pnode->pNext = NULL;
			m_dwLength++;
			pnode = pnode->pNext;
		}
	}

	//  5. �������Ϊ������		
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

//��������ɾ���ڵ�									
template<class T_ELE> DWORD LinkedList<T_ELE>::Delete(IN DWORD dwIndex)
{
	//  1. �ж������Ƿ�Ϊ��		
	if (IsEmpty())
	{
		return 1;
	}

	//  2. �ж�����ֵ�Ƿ���Ч								
	if (dwIndex < 0 || dwIndex > m_dwLength)
	{
		return -1;
	}

	//  3. ���������ֻ��ͷ�ڵ㣬��Ҫɾ��ͷ�ڵ�		
	if (dwIndex = 0 && m_dwLength == 1)
	{
		delete m_pList;
		m_pList = NULL;
		m_dwLength--;
		return SUCCESS;
	}


	//  4. ���Ҫɾ��ͷ�ڵ�		
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

	//  5. ������������		
	PNODE index_previous_node = GetIndexPreviousNode(dwIndex);
	PNODE index_next_node = GetIndexNextNode(dwIndex);
	PNODE index_is_node = GetIndexCurrentNode(dwIndex);
	delete index_is_node;
	index_previous_node->pNext = index_next_node;
}


//��ȡ�����нڵ������									
template<class T_ELE> DWORD LinkedList<T_ELE>::GetSize()
{
	return m_dwLength;
}


//��ȡdwIndexǰ��ڵ�ĵ�ַ									
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

//��ȡdwIndex�ڵ�ĵ�ַ									
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

//��ȡdwIndex����ڵ�ĵ�ַ									
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

