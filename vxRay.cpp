#include "vxRay.h"

using namespace vxCore;

vxRay::vxRay()
{}

vxRay::vxRay(const v3 &direction)
	:m_direction(direction.unit())
{
}

vxRay::vxRay(const v3 &origin, 
			 const v3 &direction)
	:m_direction(direction.unit())
	,m_origin(origin)
{
}

vxRay::vxRay(scalar x, scalar y, scalar z)
	:m_direction(v3{x,y,z}.unit())
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

v3 vxRay::origin() const
{
	return m_origin;
}

void vxRay::setOrigin(const v3 &origin)
{
	m_origin = origin;
}

v3& vxRay::direction()
{
	return m_direction;
}

v3 vxRay::direction() const
{
	return m_direction;
}

void vxRay::setDirection(const v3 &direction)
{
	m_direction = direction.unit();
}

scalar vxRay::incidence(const v3 &normal) const
{
	return cos(m_direction.angle(normal));
}

