#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include <math.h>
#include <chrono>
#include <sstream>

#include "Vector.h"

namespace vxCore {


using timePoint = std::chrono::time_point<std::chrono::system_clock>;
using timeDuration = std::chrono::duration<double>;


class TimeUtils
{
	public:
		TimeUtils();

	static std::string decorateTime(const timePoint &start, unsigned int dec = 0)
	{
		std::stringstream sst;
		timePoint now = std::chrono::system_clock::now();
		timeDuration elapsed_time = timeDuration(now-start);
		scalar seconds = elapsed_time.count();
		scalar pow10 = pow(10, dec);
		if (seconds<60)
		{
			scalar sCode = int(seconds*pow10);
			sst << "elapsed time is " << (scalar)(sCode/pow10) << " seconds";
		}
		else if(seconds<3600)
		{
			scalar sCode = int(seconds*pow10/60.0);
			sst << "elapsed time is " << (scalar)(sCode/pow10) << " minutes";
		}
		else if(seconds<(3600*24))
		{
			scalar sCode = int(seconds*pow10/3600.0);
			sst << "elapsed time is " << (scalar)(sCode/pow10) << " hours";
		}
		
		sst << std::endl;
	
		return sst.str();
	}

};

}
#endif // TimeUtils_H
