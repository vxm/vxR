#ifndef VXTHREADPOOL_H
#define VXTHREADPOOL_H

#import <map>
#import <thread>
#import "vxColor.h"

namespace vxCore {

struct uniqueInfo
{
	unsigned int id;
	vxColor color;
};

class vxThreadPool
{
	static std::map<std::thread::id, uniqueInfo> threads;

public:
	vxThreadPool();

	static uniqueInfo threadInfo(std::thread::id id)
	{
		if(threads.find(id)==std::end(threads))
		{
			threads[id].id = threads.size();
			threads[id].color = vxColor::indexColor(threads[id].id);
		}
		
		return threads[id];
	}
};

}
#endif // VXTHREADPOOL_H
