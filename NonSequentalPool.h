#ifndef VXNONSEQUENTALPOOL_H
#define VXNONSEQUENTALPOOL_H

//, Args...>
#include <vector>
#include <iostream>

template<class T>
class iterator : public std::iterator<std::forward_iterator_tag,T*>
{
	T* n;
	
public :
	
	iterator(T* temp) 
		: n(temp) 
	{

	}
	
	iterator (const iterator& myitr) 
		: n(myitr.n) 
	{
	
	}
	
	iterator& operator++()
	{
		n++;
		return *this;
	}
	
	bool operator == (const iterator& rhs) 
	{
		return n == rhs.n;
	}
	
	bool operator != (const iterator& rhs) 
	{
		return n != rhs.n;
	}
	
	T& operator*()
	{
		return *n;
	}
};

template<class T>
class vxNonSequentalPool
{
	unsigned int m_size{0};
	unsigned int m_capacity{100};
	T* m_pps{nullptr};

public:

	vxNonSequentalPool()
	{}
	
	T& back()
	{
		return *(end()-1);
	}
	
	template <class... Args>
	void emplace_back(Args&&... args )
	{
		if(m_pps==nullptr)
		{
			m_pps = (T*)malloc(m_capacity);
		}

		//back() = T(Args);
	};
	
	iterator<T> begin()
	{
		return iterator<T>(m_pps);
	}
	
	iterator<T> end()
	{
		return iterator<T>(m_pps+m_size);
	}
	
	T& operator[](unsigned int idx)
	{
		return *m_pps+idx;
	}
	
};

#endif // VXNONSEQUENTALPOOL_H
