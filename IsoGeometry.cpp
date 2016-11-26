#include <functional>
#include <memory>

#include "Vector.h"

#include "IsoGeometry.h"

using namespace vxCore;

scalar IsoGeometry::radius() const
{
	return m_radius;
}

void IsoGeometry::setRadius(const scalar &radius)
{
	m_radius = radius;
}

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
	const scalar& r = m_radius;
	col.setValid(false);
	
	//// CAPS
	
	auto&& pos = col.position();
	auto&& levelCenter = v3s{m_bb->midXValue(),pos.y(),m_bb->midZValue()};
	//Comparing floats is unsafe.
	if(pos.y()==m_bb->maxY())
	{
		if(pos.distance(levelCenter)<r)
		{
			col.setPosition(col.position()-v3s{0,0.0001,0});
			col.setNormal(v3s::constY);
			col.setValid(true);
			return 1;
		}
	}
	
	if(fabs(pos.y()-m_bb->minY())<0.0001)
	{	
		if(pos.distance(levelCenter)<r)
		{
			col.setPosition(col.position());
			col.setNormal(v3s::constMinusY);
			col.setValid(true);
			return 1;
		}
	}
	
	//// BODY
	auto rey = ray;
	
	rey.setDirection({ray.direction().x(),
					  sZero,
					  ray.direction().z()});
	
	rey.setOrigin({ray.origin().x(),
				   sZero,
				   ray.origin().z()});
	
	auto&& circleCenter = v3s{m_bb->midXValue(),
								sZero,
								m_bb->midZValue()};
	
	auto d = rey.direction();
	auto f = rey.origin() - circleCenter;
	
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
			v3s twoPos = {rey.origin()+(rey.direction()*t1)};
			col.setNormal((twoPos-circleCenter).unit());
			
			auto finalPos = MU::rectAndZPlane(ray.direction(), twoPos.z() - ray.origin().z());
			finalPos+=ray.origin();
			
			col.setPosition(finalPos);
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
