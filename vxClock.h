#ifndef VXCLOCK_H
#define VXCLOCK_H

#include "vxVector.h"

#include "TimeUtils.h"

namespace vxCore
{

class vxClock
{
	static scalar m_time;
	static scalar m_step;
	static scalar m_start;
	static scalar m_end;
	
public:
	vxClock();
	
	static vxClock clock()
	{
		static vxClock cl;
		return cl;
	}
	
	static scalar time();
	static void setTime(const scalar &time);

	static void reset();
	static void stepTime();
	static scalar step();
	static void setStep(const scalar &step);
	static scalar start();
	static void setStart(const scalar &start);
	static scalar end();
	static void setEnd(const scalar &end);
};


}
#endif // VXCLOCK_H
