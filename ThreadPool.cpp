#include "ThreadPool.h"

using namespace vxCore;


std::map<std::thread::id, uniqueInfo> ThreadPool::threads;

ThreadPool::ThreadPool()
{
}

uniqueInfo ThreadPool::threadInfo(std::thread::id id)
{
	if(threads.find(id)==std::end(threads))
	{
		threads[id].id = threads.size();
		threads[id].color = Color::indexColor(threads[id].id);
	}
	
	return threads[id];
}

Box &ThreadPool::threadBox(std::thread::id id)
{
	return threads[id].box;
}
