#include "vxPlane.h"
#include "MathUtils.h"

using namespace vxCore;

vxColor vxPlane::color() const
{
	return m_color;
}

void vxPlane::setColor(const vxColor &color)
{
	m_color = color;
}

vxPlane::vxPlane()
{
}

double vxPlane::y() const
{
	return m_y;
}

void vxPlane::setY(double y)
{
	m_y = y;
}

double vxPlane::z() const
{
	return m_z;
}

void vxPlane::setZ(double z)
{
	m_z = z;
}

double vxPlane::d() const
{
	return m_d;
}

void vxPlane::setD(double d)
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
	const auto&& p = MathUtils::rectAndYPlane(ray.direction(), m_d);
	if(!std::signbit(p.z()))
	{
		collide.setNormal(vxVector3d::constY);
		collide.setPosition(p);
		collide.setU(fmod(p.x(),1.0));
		collide.setV(fmod(p.z(),1.0));
		collide.setColor(m_color);
		return 1;
	}
	
	return 0;
}

bool vxPlane::hasCollision(const vxRay &ray) const
{
	const auto p = MathUtils::rectAndYPlane(ray.direction(), -110.0);
	return !std::signbit(p.z());
}
double vxPlane::x() const
{
	return m_x;
}

void vxPlane::setX(double x)
{
	m_x = x;
}
