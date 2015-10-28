#include "vxRay.h"

using namespace vxCore;

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

vxRay::vxRay(scalar x, scalar y, scalar z)
	:m_direction(vxVector3d{x,y,z}.unit())
{
}

vxRay::vxRay(const vxRay &ray)
	:m_direction{ray.m_direction}
	,m_origin{ray.m_origin}
{
}

vxRay::vxRay(const vxRay &&ray)
	:m_direction{std::move(ray.m_direction)}
	,m_origin{std::move(ray.m_origin)}
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

scalar vxRay::incidence(const vxVector3d &normal) const
{
	return cos(m_direction.angle(normal));
}

