#include "vxPlane.h"
#include "MathUtils.h"

using namespace vxCore;

vxPlane::vxPlane()
{
}

vxColor vxPlane::color() const
{
	return m_color;
}

void vxPlane::setColor(const vxColor &color)
{
	m_color = color;
}


v3s vxPlane::pointA() const
{
	return m_pointA;
}

void vxPlane::setPointA(const v3s &point1)
{
	m_pointA = point1;
}

v3s vxPlane::pointB() const
{
	return m_pointB;
}

void vxPlane::setPointB(const v3s &point2)
{
	m_pointB = point2;
}

v3s vxPlane::pointC() const
{
	return m_pointC;
}

void vxPlane::setPointC(const v3s &point3)
{
	m_pointC = point3;
}

scalar vxPlane::y() const
{
	return m_y;
}

void vxPlane::setY(scalar y)
{
	m_y = y;
}

scalar vxPlane::z() const
{
	return m_z;
}

void vxPlane::setZ(scalar z)
{
	m_z = z;
}

scalar vxPlane::d() const
{
	return m_d;
}

void vxPlane::setD(scalar d)
{
	m_d = d;
}

bool vxPlane::throwRay(const vxRay &ray) const
{
	if(hasCollision(ray))
	{
		return 1;
	}
	
	return 0;
}

int vxPlane::throwRay(const vxRay &ray, vxCollision &collide) const
{
	if(m_type==vxPlane::type::kY)
	{
		const auto& p = MU::rectAndYPlane(ray.direction(), m_y-ray.origin().y());
		if((ray.origin()-p).follows(ray.direction()))
		{
			collide.setNormal(v3s::constY);
			collide.setPosition(p+ray.origin());
			collide.setU((scalar)0.5);
			collide.setV((scalar)0.5);
			collide.setColor(m_color);
			return 1;
		}
	}
	if(m_type==vxPlane::type::kFree)
	{
		auto&& n = MU::normal(m_pointA,m_pointB,m_pointC);
		auto&& p = MU::rectAndPlane(ray,m_pointA,m_pointB,m_pointC);
		if((ray.origin()-p).follows(ray.direction()))
		{
			collide.setNormal(n);
			collide.setPosition(p+ray.origin());
			collide.setU((scalar)0.5);
			collide.setV((scalar)0.5);
			collide.setColor(vxColor::white);
			return 1;
		}
	}
	
	return 0;
}

bool vxPlane::hasCollision(const vxRay &ray) const
{
	if(m_type==vxPlane::type::kY)
	{
		const auto&& p = MU::rectAndYPlane(ray.direction(), m_y);
		if(ray.incidence(ray.origin()-p)<0)
		{
			return true;
		}
	}
	if(m_type==vxPlane::type::kFree)
	{
		auto&& n = MU::normal(m_pointA,m_pointB,m_pointC);
		auto&& p = MU::rectAndPlane(ray,m_pointA,m_pointB,m_pointC);
		if(ray.incidence(ray.origin()-p)<0)
		{
			return true;
		}
	}
	
	return false;
}

scalar vxPlane::x() const
{
	return m_x;
}

void vxPlane::setX(scalar x)
{
	m_x = x;
}
