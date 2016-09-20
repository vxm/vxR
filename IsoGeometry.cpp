#include <functional>
#include <memory>

#include "Vector.h"

#include "IsoGeometry.h"

using namespace vxCore;

IsoGeometry::IsoGeometry()
{
	m_bb->set({0,0,0},1);
}

bool IsoGeometry::throwRay(const Ray &ray) const
{
	return Geometry::throwRay(ray);
}

int IsoGeometry::throwRay(const Ray &ray, Collision &col) const
{
	col.setValid(false);
	
	auto&& pos = col.position();
	
	scalar r = 0.5;
	
	//auto&& axisPoint = v3s{m_bb->midXValue(),m_bb->midYValue(),m_bb->midZValue()};
	
	auto&& axisPoint = v3s::zero;
	
	//Comparing floats is unsafe.
	if(pos.y()==m_bb->maxY() || pos.y()==m_bb->minY())
	{
		if(pos.distance(axisPoint)<r)
		{
			col.setNormal(v3s::constY);
			col.setValid(true);
			return 1;
		}
	}
	
	auto d = ray.direction();
	auto f = ray.origin() - axisPoint;
	
	auto a = d.dot(d);
	auto b = 2.0*f.dot(d);
	auto c = f.dot(f) - (r*r);
	
	scalar disc = (b*b)-(4.0*(a*c));
	if(disc<0.0)
	{
		col.setValid(false);
		return 0;
	}
	else
	{
		disc = sqrt(disc);
		
		scalar t1 = (-b - disc)/(2.0*a);
		if(t1>=0.0)
		{
			col.setPosition(ray.origin()+(ray.direction()*t1));
			
			col.setNormal(col.position().unit());
			col.setValid(true);
			return 1;
		}
	}

	col.setValid(false);
	return 0;
}

bool IsoGeometry::hasCollision(const Ray &ray) const
{
	return Geometry::hasCollision(ray);
}
