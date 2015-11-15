#ifndef VXLIST_H
#define VXLIST_H

#include <iostream>

namespace vxCore {

template<class T> class vxListNode
{
	
public:
	vxListNode()
		:m_next(nullptr)
		,m_prev(nullptr)
	{
		
	}
	
	vxListNode<T> *m_next;
	vxListNode<T> *m_prev;
	
	T value;
};

template <class T> class vxList
{
	vxListNode<T> *m_last;
	vxListNode<T> *m_first;
	
public:
	vxList();
	
	vxList(unsigned int initSize, const T& value);
	
	// non cached size
	unsigned int size();
	
	T* find(const T& value);
	
	void remove(vxListNode<T>* tr);
	
	void removeAt(const unsigned int idx);
	
	T* at(const unsigned int idx);
	
	vxListNode<T>* createNode(const T &value);
	
};

}
#endif // VXLIST_H
