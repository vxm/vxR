#ifndef VXPLANE_H
#define VXPLANE_H

#include "Collision.h"
#include "Ray.h"
#include "TriRef.h"

namespace vxCore {

class Plane
{
	scalar m_x {0.0};
	scalar m_y {0.0};
	scalar m_z {0.0};
	scalar m_d {0.0};
	
	v3s m_pointA;
	v3s m_pointB;
	v3s m_pointC;
	
	Color m_color{Color::white};
	
	public:

		enum class type
		{
			kX,					// x is constant
			kY,					// y is constant
			kZ,					// z is constant
			kX0,				// x is 0
			kY0,				// y is 0
			kZ0,				// z is 0
			kFree				// any set of values
		};
	
	type m_type;
	
	Plane();
	
	Plane(type pType)
		:m_type(pType)
	{
		
	}
	
	Plane(scalar x,
			scalar y = 0.0,
			scalar z = 0.0,
			scalar d = 0.0)
		: m_x(x)
		, m_y(y)
		, m_z(z)
		, m_d(d)
	{
		
	}
	
	scalar x() const;
	void setX(scalar x);

	scalar y() const;
	void setY(scalar y);

	scalar z() const;
	void setZ(scalar z);

	scalar d() const;
	void setD(scalar d);
	
	//renderable interface
	bool throwRay(const Ray &ray) const;
	int throwRay(const Ray &ray, Collision &collide) const;
	bool hasCollision(const Ray &ray) const;
	
	Color color() const;
	void setColor(const Color &color);
	v3s pointA() const;
	void setPointA(const v3s &pointA);
	v3s pointB() const;
	void setPointB(const v3s &pointB);
	v3s pointC() const;
	void setPointC(const v3s &pointC);
};

}
#endif // VXPLANE_H
