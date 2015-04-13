#ifndef VXSTOPWATCHER_H
#define VXSTOPWATCHER_H

#include<vector>
#include<chrono>

class vxStopwatch
{
	using timePoint = std::chrono::time_point<std::chrono::system_clock>;
	using timeDuration = std::chrono::duration<double>;

	timePoint m_start;
	std::vector<double> m_memory;
	
	public:
		vxStopwatch()
		{
			reset();
		}
		
		void reset()
		{
			m_start = std::chrono::system_clock::now();
		}
		
		void addMem()
		{
			m_memory.push_back(elapsedTime());
		}
		
		void resetMemory()
		{
			m_memory.clear();
		}
		
		double elapsedTime() const
		{
			timePoint now = std::chrono::system_clock::now();
			timeDuration elapsed_time = timeDuration(now-m_start);
			return elapsed_time.count();
		}
};

#endif // VXSTOPWATCHER_H
