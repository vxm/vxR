#ifndef VXTHREADPOOL_H
#define VXTHREADPOOL_H

#include <map>
#include <thread>
#include "vxColor.h"
#include "vxBox.h"

namespace vxCore {

struct uniqueInfo
{
	unsigned int id;
	vxColor color;
	vxBox box;
};

class vxThreadPool
{

public:
	static std::map<std::thread::id, uniqueInfo> threads;

	vxThreadPool();
	///
	/// \brief threadInfo
	/// \param id
	/// \return 
	///
	static uniqueInfo threadInfo(std::thread::id id)
	{
		if(threads.find(id)==std::end(threads))
		{
			threads[id].id = threads.size();
			threads[id].color = vxColor::indexColor(threads[id].id);
		}
		
		return threads[id];
	}
	
	///
	/// \brief threadBox
	/// \param id
	/// \return 
	///
	static vxBox& threadBox(std::thread::id id)
	{
		return threads[id].box;
	}
};

}
#endif // VXTHREADPOOL_H
