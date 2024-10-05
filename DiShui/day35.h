#pragma once
#include <iostream>
#include <Windows.h>


#define SUCCESS           			 1 // 成功	
#define ERROR_MY            		-1 // 失败	
#define MALLOC_ERROR				-2 // 申请内存失败	
#define INDEX_ERROR		 			-3 // 错误的索引号	

using namespace std;

// Vector类实现
template<class T_ELE>
class Vector
{
public:
	Vector();
	Vector(DWORD dwSize);
	~Vector();

public:
	DWORD	at(DWORD dwIndex, OUT T_ELE* pEle);					//根据给定的索引得到元素				
	DWORD   push_back(T_ELE Element);							//将元素存储到容器最后一个位置				
	VOID	pop_back();											//删除最后一个元素				
	DWORD	insert(DWORD dwIndex, T_ELE Element);				//向指定位置新增一个元素				
	DWORD	capacity();											//返回在不增容的情况下，还能存储多少元素				
	VOID	clear();											//清空所有元素				
	BOOL	empty();											//判断Vector是否为空 返回true时为空				
	VOID	erase(DWORD dwIndex);								//删除指定元素				
	DWORD	size();												//返回Vector元素数量的大小				

private:
	BOOL	expand();
private:
	DWORD  m_dwIndex;						//下一个可用索引	
	DWORD  m_dwIncrement;					//每次增容的大小	
	DWORD  m_dwLen;							//当前容器的长度	
	DWORD  m_dwInitSize;					//默认初始化大小	
	T_ELE* m_pVector;						//容器指针	


};

// 无参构造函数
template<class T_ELE>
Vector<T_ELE>::Vector() :m_dwInitSize(100), m_dwIncrement(5) 
{

	m_pVector = new T_ELE[m_dwInitSize];
	memset(m_pVector, 0, m_dwInitSize * sizeof(T_ELE));
	m_dwLen = m_dwInitSize;
	m_dwInitSize = m_dwInitSize * sizeof(T_ELE);
	m_dwIndex = 0;}

// 有参构造函数
template<class T_ELE>
Vector<T_ELE>::Vector(DWORD dwSize) : m_dwIncrement(5)
{

	m_pVector = new T_ELE[dwSize];
	memset(m_pVector, 0, sizeof(T_ELE) * dwSize);
	m_dwLen = dwSize;
	m_dwInitSize = sizeof(T_ELE) * dwSize;
	m_dwIndex = 0;
}

// 析构函数
template<class T_ELE>
Vector<T_ELE>::~Vector()
{
	delete[] m_pVector;
	m_pVector = nullptr;
}

template<class T_ELE>
BOOL Vector<T_ELE>::expand()
{
	DWORD expand_len = m_dwLen + m_dwIncrement;
	T_ELE* temp = nullptr;
	temp = new T_ELE[expand_len];
	memset(temp, 0, sizeof(T_ELE) * expand_len);
	memcpy(temp, m_pVector, sizeof(T_ELE) * m_dwLen);
	delete[] m_pVector;
	m_pVector = temp;
	m_dwLen = expand_len;
	m_dwInitSize = sizeof(T_ELE) * expand_len;
	return 0;
}

template<class T_ELE>
DWORD Vector<T_ELE>::at(DWORD dwIndex, OUT T_ELE* pEle)
{
	if (dwIndex > m_dwIndex)
	{
		cout << "索引值错误，元素不存在！！！" << endl;
		return ERROR_MY;
	}
	*pEle = *(&m_pVector[dwIndex]);
}

template<class T_ELE>
DWORD Vector<T_ELE>::push_back(T_ELE Element)
{
	if (m_dwIndex >= m_dwLen)
	{
		expand();
	}
	memcpy(&m_pVector[m_dwIndex], &Element, sizeof(Element));
	m_dwIndex++;
	return 0;
}

template<class T_ELE>
VOID Vector<T_ELE>::pop_back()
{
	memset(&m_pVector[--m_dwIndex], 0, sizeof(T_ELE));

}

template<class T_ELE>
DWORD Vector<T_ELE>::insert(DWORD dwIndex, T_ELE Element)
{
	if (dwIndex < m_dwIndex)
	{
		cout << "该位置已有数据" << endl;
		return ERROR_MY;
	}
	memcpy(&m_pVector[dwIndex], &Element, sizeof(Element));

	return SUCCESS;
}

template<class T_ELE>
DWORD Vector<T_ELE>::capacity()
{
	return (m_dwLen - m_dwIndex) - 1;
}


template<class T_ELE>
BOOL Vector<T_ELE>::empty()
{
	
	bool isEmpty;
	int temp = 0;
	while (true)
	{
		if (*m_pVector != 0)
		{
			break;
		}
		m_pVector++;
		temp++;
	}
	
	if (temp == m_dwLen)
	{
		isEmpty =  true;

	}
	else
	{
		isEmpty = false;
	}
	return isEmpty;
}

template<class T_ELE>
VOID Vector<T_ELE>::erase(DWORD dwIndex)
{
	if (dwIndex >= m_dwIndex)
	{
		throw std::runtime_error("索引错误，元素不存在");
	
	}
	memcpy(&m_pVector[dwIndex], &m_pVector[dwIndex + 1], m_dwInitSize - sizeof(T_ELE));
}

template<class T_ELE>
DWORD Vector<T_ELE>::size()
{
	DWORD temp = 0;
	while (true)
	{
		if (*m_pVector != 0)
		{
			temp++;
		}
		else
		{
			break;
		}
		m_pVector++;
	}
	return sizeof(T_ELE) * temp;
}


template<class T_ELE>
void Vector<T_ELE>::clear()
{
	memset(m_pVector, 0, m_dwInitSize);
	m_dwIndex = 0;
}
