#include "vxRay.h"

using namespace vxCore;

vxRay::vxRay()
{}

vxRay::vxRay(const v3s &direction)
	:m_direction(direction.unit())
{
}

vxRay::vxRay(const v3s &origin, 
			 const v3s &direction)
	:m_direction(direction.unit())
	,m_origin(origin)
{
}

vxRay::vxRay(scalar x, scalar y, scalar z)
	:m_direction(v3s{x,y,z}.unit())
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

v3s vxRay::origin() const
{
	return m_origin;
}

void vxRay::setOrigin(const v3s &origin)
{
	m_origin = origin;
}

v3s& vxRay::direction()
{
	return m_direction;
}

v3s vxRay::direction() const
{
	return m_direction;
}

void vxRay::setDirection(const v3s &direction)
{
	m_direction = direction.unit();
}

scalar vxRay::incidence(const v3s &normal) const
{
	return cos(m_direction.angle(normal));
}

scalar vxRay::distance(const v3s &final)
{
	return m_origin.distance(final);
}

bool vxRay::isCloser(const v3s &a, const v3s &b) const
{
	return m_origin.isCloser(a, b);
}

