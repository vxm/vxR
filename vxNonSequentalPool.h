#ifndef VXNONSEQUENTALPOOL_H
#define VXNONSEQUENTALPOOL_H

//, Args...>
#include "vector"

template<class T>
class vxNonSequentalPool
{
	unsigned int m_size{0};
	unsigned int m_capacity{0};
	std::vector<T*> m_pps;
	T* last;
	
public:
	vxNonSequentalPool();

	template <class... Args>
	void emplace_back(Args&&... args )
	{
		
	}

};

#endif // VXNONSEQUENTALPOOL_H
