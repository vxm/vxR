#include "vxClock.h"

using namespace vxCore;
scalar vxClock::m_time = 0.0;
scalar vxClock::m_step = 1.0;
scalar vxClock::m_start = 0.0;
scalar vxClock::m_end = 1.0;

vxClock::vxClock()
{
	
}

scalar vxClock::time()
{
	return m_time;
}

void vxClock::setTime(const scalar &time)
{
	m_time = time;
}

void vxClock::reset()
{
	m_time = m_start;
}

void vxClock::stepTime()
{
	m_time += m_step;
}

scalar vxClock::step()
{
	return m_step;
}

void vxClock::setStep(const scalar &step)
{
	m_step = step;
}

scalar vxClock::start()
{
	return m_start;
}

void vxClock::setStart(const scalar &start)
{
	m_start = start;
}

scalar vxClock::end()
{
	return m_end;
}

void vxClock::setEnd(const scalar &end)

{
	vxClock::m_end = end;
}
