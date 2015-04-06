#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include <chrono>
#include <sstream>

using timePoint = std::chrono::time_point<std::chrono::system_clock>;
using timeDuration = std::chrono::duration<double>;


class TimeUtils
{
	public:
		TimeUtils();

	static std::string decorateTime(const timePoint &start)
	{
		std::stringstream sst;
		timePoint now = std::chrono::system_clock::now();
		timeDuration elapsed_time = timeDuration(now-start);
		double seconds = elapsed_time.count();
		
		if (seconds<60)
		{
			sst << "elapsed time is " << int(seconds)  << " seconds";
		}
		else if(seconds<3600)
		{
			sst << "elapsed time is " << int(seconds/60.0)  << " minutes";
		}
		else if(seconds<(3600*24))
		{
			sst << "elapsed time is " << int(seconds/3600.0)  << " hours";
		}
		
		sst << std::endl;
	
		return sst.str();
	}

};

#endif // TimeUtils_H
