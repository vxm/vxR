#include "Clock.h"

using namespace vxCore;
scalar Clock::m_time = 0.0;
scalar Clock::m_step = 1.0;
scalar Clock::m_start = 0.0;
scalar Clock::m_end = 1.0;

scalar Clock::time() { return m_time; }

void Clock::setTime(const scalar &time) { m_time = time; }

void Clock::reset() { m_time = m_start; }

void Clock::stepTime() { m_time += m_step; }

scalar Clock::step() { return m_step; }

void Clock::setStep(const scalar &step) { m_step = step; }

scalar Clock::start() { return m_start; }

void Clock::setStart(const scalar &start) { m_start = start; }

scalar Clock::end() { return m_end; }

void Clock::setEnd(const scalar &end) { Clock::m_end = end; }
