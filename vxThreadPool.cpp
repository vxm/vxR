#include "vxThreadPool.h"

using namespace vxCore;


std::map<std::thread::id, uniqueInfo> vxThreadPool::threads;

vxThreadPool::vxThreadPool()
{
}
