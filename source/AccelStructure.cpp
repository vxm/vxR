#include "AccelStructure.h"
#include "BroadPhase.h"
#include "Bvh.h"

using namespace vxCore;

namespace
{

struct Backend
{
	const char *name;
	AccelStructureHandle (*make)();
};

// One line per backend.
const Backend kBackends[] = {
	{"broadphase", [] { return AccelStructureHandle(new BroadPhase()); }},
	{"bvh", [] { return AccelStructureHandle(new Bvh()); }},
};

} // namespace

AccelStructureHandle AccelStructure::create(const std::string &name)
{
	for (const auto &b : kBackends)
	{
		if (name == b.name)
		{
			return b.make();
		}
	}
	return nullptr;
}

std::vector<std::string> AccelStructure::names()
{
	std::vector<std::string> ret;
	for (const auto &b : kBackends)
	{
		ret.emplace_back(b.name);
	}
	return ret;
}

const char *AccelStructure::defaultName() { return kBackends[0].name; }
