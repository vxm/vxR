#include <functional>
#include <memory>

#include "Vector.h"

#include "ConvexGeometry.h"

using namespace vxCore;

ConvexGeometry::ConvexGeometry()
{
	m_bb->set({0, 0, 0}, 1);

	m_type = VisibleType::kOtherOpaque;
}
