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


vxVector3d vxPlane::pointA() const
{
	return m_pointA;
}

void vxPlane::setPointA(const vxVector3d &point1)
{
	m_pointA = point1;
}

vxVector3d vxPlane::pointB() const
{
	return m_pointB;
}

void vxPlane::setPointB(const vxVector3d &point2)
{
	m_pointB = point2;
}

vxVector3d vxPlane::pointC() const
{
	return m_pointC;
}

void vxPlane::setPointC(const vxVector3d &point3)
{
	m_pointC = point3;
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
	if(m_type==vxPlane::type::kY)
	{
		const auto& p = MU::rectAndYPlane(ray.direction(), m_y);
		if(ray.incidence(ray.origin()-p)<=-0.001)
		{
			collide.setNormal(vxVector3d::constY);
			collide.setPosition(p);
			collide.setU(fmod(p.x(),1.0));
			collide.setV(fmod(p.z(),1.0));
			collide.setColor(m_color);
			return 1;
		}
	}
	if(m_type==vxPlane::type::kFree)
	{
		vxTriRef t(m_pointA,m_pointB,m_pointC);
		const auto& p = MU::rectAndPlane(ray,t);
		if(ray.incidence(ray.origin()-p)<=-0.001)
		{
			collide.setNormal(t.getNormal());
			collide.setPosition(p);
			collide.setU(fmod(p.x()/10.0,1.0));
			collide.setV(fmod(p.z()/10.0,1.0));
			collide.setColor(m_color);
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
		vxTriRef t(m_pointA,m_pointB,m_pointC);
		const auto& p = MU::rectAndPlane(ray,t);
		if(ray.incidence(ray.origin()-p)<0)
		{
			return true;
		}

	}

	
	return false;
}

double vxPlane::x() const
{
	return m_x;
}

void vxPlane::setX(double x)
{
	m_x = x;
}
