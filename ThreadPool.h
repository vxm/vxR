#ifndef VXTHREADPOOL_H
#define VXTHREADPOOL_H

#include <map>
#include <thread>
#include "Color.h"
#include "Box.h"

namespace vxCore {

struct uniqueInfo
{
	unsigned int id;
	Color color;
	Box box;
};

class ThreadPool
{

public:
	static std::map<std::thread::id, uniqueInfo> threads;

	ThreadPool();
	///
	/// \brief threadInfo
	/// \param id
	/// \return 
	///
	static uniqueInfo threadInfo(std::thread::id id);
	
	///
	/// \brief threadBox
	/// \param id
	/// \return 
	///
	static Box& threadBox(std::thread::id id);
};

}
#endif // VXTHREADPOOL_H
