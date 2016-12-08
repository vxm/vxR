#include "Cylinder.h"

using namespace vxCore;

Cylinder::Cylinder()
{
}

scalar Cylinder::radius() const
{
	return m_radius;
}

void Cylinder::setRadius(const scalar &radius)
{
	m_radius = radius;
}

bool Cylinder::throwRay(const Ray &ray) const
{
	return Geometry::throwRay(ray);
}

int Cylinder::throwRay(const Ray &ray, Collision &col) const
{
	scalar r = 0.5;
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
		/*
		scalar t2 = (-b + disc)/(scalar(2.0)*a);
		if(t2>=0.0)
		{
			v3s twoPos = {rey.origin()+(rey.direction()*t2)};
			
			auto finalPos = MU::rectAndXPlane(ray.direction(), twoPos.x() - ray.origin().x());
			
			col.setPosition(finalPos);
			
			col.setNormal((twoPos-circleCenter).unit());
			col.setValid(true);
			return 1;
		}
		*/
	}
	
	col.setValid(false);
	return 0;
}

bool Cylinder::hasCollision(const Ray &ray) const
{
	return Geometry::hasCollision(ray);
}

