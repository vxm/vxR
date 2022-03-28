#include "Stopwatch.h"

using namespace vxCore;

Stopwatch::Stopwatch() { reset(); }

void Stopwatch::reset() { m_start = std::chrono::system_clock::now(); }

void Stopwatch::addMem() { m_memory.emplace_back(elapsedTime()); }

void Stopwatch::resetMemory() { m_memory.clear(); }

scalar Stopwatch::elapsedTime() const
{
	timePoint now = std::chrono::system_clock::now();
	timeDuration elapsed_time = timeDuration(now - m_start);
	return elapsed_time.count();
}
