#include "vxRay.h"

using namespace vxCore;


vxVector3d& vxRay::direction()
{
	return m_direction;
}

vxVector3d vxRay::direction() const
{
	return m_direction;
}

void vxRay::setDirection(const vxVector3d &direction)
{
	m_direction = direction.unit();
}
vxRay::vxRay()
{}

vxRay::vxRay(const vxVector3d &direction)
	:m_direction(direction.unit())
{
}

vxRay::vxRay(const vxVector3d &origin, 
			 const vxVector3d &direction)
	:m_direction(direction.unit())
	,m_origin(origin)
{
}

vxRay::vxRay(double x, double y, double z)
	:m_direction(vxVector3d{x,y,z}.unit())
{
}

vxVector3d vxRay::origin() const
{
	return m_origin;
}

void vxRay::setOrigin(const vxVector3d &origin)
{
	m_origin = origin;
}

