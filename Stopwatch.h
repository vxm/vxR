#ifndef VXSTOPWATCHER_H
#define VXSTOPWATCHER_H

#include<vector>
#include<chrono>
#include "Vector.h"

namespace vxCore
{
class Stopwatch
{
	using timePoint = std::chrono::time_point<std::chrono::system_clock>;
	using timeDuration = std::chrono::duration<scalar>;

	timePoint m_start;
	std::vector<scalar> m_memory;
	
	public:
		Stopwatch();
		
		void reset();
		
		void addMem();
		
		void resetMemory();
		
		scalar elapsedTime() const;
};

}
#endif // VXSTOPWATCHER_H
