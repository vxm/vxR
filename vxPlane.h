#ifndef VXPLANE_H
#define VXPLANE_H

#include "vxCollision.h"
#include "vxRay.h"
#include "vxTriRef.h"

namespace vxCore {

class vxPlane
{
	scalar m_x {0.0};
	scalar m_y {0.0};
	scalar m_z {0.0};
	scalar m_d {0.0};
	
	v3 m_pointA;
	v3 m_pointB;
	v3 m_pointC;
	
	vxColor m_color{vxColor::white};
	
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
	
	vxPlane();
	
	vxPlane(type pType)
		:m_type(pType)
	{
		
	}
	
	vxPlane(scalar x,
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
	bool throwRay(const vxRay &ray) const;
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &ray) const;
	
	vxColor color() const;
	void setColor(const vxColor &color);
	v3 pointA() const;
	void setPointA(const v3 &pointA);
	v3 pointB() const;
	void setPointB(const v3 &pointB);
	v3 pointC() const;
	void setPointC(const v3 &pointC);
};

}
#endif // VXPLANE_H
