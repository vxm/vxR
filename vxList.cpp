#include "vxList.h"


using namespace vxCore;

template<class T> vxList<T>::vxList()
	:m_last(nullptr)
	,m_first(nullptr)
{
	
}

template<class T> vxList<T>::vxList(unsigned int initSize, const T &value)
	:m_last(nullptr)
	,m_first(nullptr)
{
	for(unsigned int i=0; i<initSize; ++i)
	{
		createNode(value);
	}
}

template<class T> unsigned int vxList<T>::size()
{
	unsigned int k=0;
	for(vxListNode<T>* it = m_first;
		it != nullptr; it=it->m_next)
	{
		k++;
	}
	
	return k;
}

template<class T> T *vxList<T>::find(const T &value)
{
	T* found{nullptr};
	for(vxListNode<T>* it = m_first;
		it != nullptr; it=it->m_next)
	{
		if(it->value == value)
		{
			return &it->value;
		}
	}
	
	return found;
}

template<class T> void vxList<T>::remove(vxListNode<T> *tr)
{
	if(tr==nullptr)
	{
		return;
	}
	
	vxListNode<T>* prev = tr->m_prev;
	vxListNode<T>* next = tr->m_next;
	
	delete tr;
	
	prev->m_next = next;
	next->m_prev = prev;
}

template<class T> void vxList<T>::removeAt(const unsigned int idx)
{
	unsigned int k=0;
	
	for(vxListNode<T>* it = m_first;
		it != nullptr; it=it->m_next)
	{
		if(k==idx)
		{
			remove(it);
			return;
		}
		
		k++;
	}
}

template<class T> T *vxList<T>::at(const unsigned int idx)
{
	unsigned int k=0;
	
	T* found{nullptr};
	
	for(vxListNode<T>* it = m_first;
		it != nullptr; it=it->m_next)
	{
		if(k==idx)
		{
			return &it->value;
		}
		
		k++;
	}
	
	return found;
}

template<class T> vxListNode<T> *vxList<T>::createNode(const T &value)
{
	vxListNode<T>* newNode = new vxListNode<T>;
	
	newNode->value = value;
	
	if(m_first==nullptr)
	{
		m_first = newNode;
		m_last = newNode;
		
		return m_first;
	}
	
	m_last->m_next = newNode;
	
	m_last->m_next->m_prev = m_last;
	
	m_last = newNode;
	
	return newNode;
}
