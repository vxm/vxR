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


/*
	std::cout << "building 10 ints value 0" << std::endl;
	vxList<int> ints(1000000l, 0);
	
	std::cout << "Size of list is " << ints.size() << std::endl;
	
	std::cout << "Removing at postition 5" << std::endl;
	ints.removeAt(4);
	
	std::cout << "Now size of list is " << ints.size() << std::endl;
	
	std::cout << "looking for 5" << std::endl;
	ints.find(5);

	std::cout << "At position 3" << std::endl;
	auto v = ints.at(5);
	
	if(v!=nullptr)
	{
		std::cout << "Assigning value 5" << std::endl;
		*v = 5;
		
		std::cout << "Looking for 5" << std::endl;
		auto f = ints.find(5);
		
		std::cout << "this is what I found add:  " << *f << std::endl;
	}
	
	return 1;*/
