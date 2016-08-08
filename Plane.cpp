#include "Plane.h"
#include "MathUtils.h"

using namespace vxCore;

Plane::Plane()
{
}

Color Plane::color() const
{
	return m_color;
}

void Plane::setColor(const Color &color)
{
	m_color = color;
}


v3s Plane::pointA() const
{
	return m_pointA;
}

void Plane::setPointA(const v3s &point1)
{
	m_pointA = point1;
}

v3s Plane::pointB() const
{
	return m_pointB;
}

void Plane::setPointB(const v3s &point2)
{
	m_pointB = point2;
}

v3s Plane::pointC() const
{
	return m_pointC;
}

void Plane::setPointC(const v3s &point3)
{
	m_pointC = point3;
}

scalar Plane::y() const
{
	return m_y;
}

void Plane::setY(scalar y)
{
	m_y = y;
}

scalar Plane::z() const
{
	return m_z;
}

void Plane::setZ(scalar z)
{
	m_z = z;
}

scalar Plane::d() const
{
	return m_d;
}

void Plane::setD(scalar d)
{
	m_d = d;
}

bool Plane::throwRay(const Ray &ray) const
{
	if(hasCollision(ray))
	{
		return 1;
	}
	
	return 0;
}

int Plane::throwRay(const Ray &ray, Collision &collide) const
{
	if(m_type==Plane::type::kY)
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
	if(m_type==Plane::type::kFree)
	{
		auto&& n = MU::normal(m_pointA,m_pointB,m_pointC);
		auto&& p = MU::rectAndPlane(ray,m_pointA,m_pointB,m_pointC);
		if((ray.origin()-p).follows(ray.direction()))
		{
			collide.setNormal(n);
			collide.setPosition(p+ray.origin());
			collide.setU((scalar)0.5);
			collide.setV((scalar)0.5);
			collide.setColor(Color::white);
			return 1;
		}
	}
	
	return 0;
}

bool Plane::hasCollision(const Ray &ray) const
{
	if(m_type==Plane::type::kY)
	{
		const auto&& p = MU::rectAndYPlane(ray.direction(), m_y);
		if(ray.incidence(ray.origin()-p)<0)
		{
			return true;
		}
	}
	if(m_type==Plane::type::kFree)
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

scalar Plane::x() const
{
	return m_x;
}

void Plane::setX(scalar x)
{
	m_x = x;
}
