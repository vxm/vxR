#include <functional>
#include <memory>

#include "Vector.h"

#include "IsoGeometry.h"

using namespace vxCore;

IsoGeometry::IsoGeometry()
{
	m_bb->set({0,0,0},1);
}
