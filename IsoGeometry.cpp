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
	auto&& axisPoint = v3s{m_bb->midXValue(),pos.y(),m_bb->midZValue()};
	
	scalar r = 0.5;
	
	//Comparing floats is unsafe.
	if(pos.y()==m_bb->maxY())
	{
		if(pos.distance(axisPoint)<r)
		{
			col.setNormal(v3s::constY);
			col.setValid(true);
			return 1;
		}
	}
	
	if(pos.y()==m_bb->minY())
	{	
		if(pos.distance(axisPoint)<r)
		{
			col.setNormal(v3s::constMinusY);
			col.setValid(true);
			return 1;
		}
	}
	
	auto d = ray.direction();
	auto f = ray.origin() - axisPoint;
	
	auto a = d.dot(d);
	auto b = scalar(2.0)*f.dot(d);
	auto c = f.dot(f) - (r*r);
	
	scalar disc = (b*b)-(scalar(4.0)*(a*c));
	if(disc>=0.0)
	{
		disc = sqrt(disc);
		
		scalar t1 = (-b - disc)/(scalar(2.0)*a);
		if(t1>=0.0)
		{
			col.setPosition(ray.origin()+(ray.direction()*t1));
			col.setNormal((col.position()-axisPoint).unit());
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
