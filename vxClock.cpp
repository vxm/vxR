#include "vxClock.h"

using namespace vxCore;

scalar vxClock::time() const
{
	return m_time;
}

void vxClock::setTime(const scalar &time)
{
	m_time = time;
}

vxClock::vxClock()
{
	
}

