#include "Sphere.h"

using namespace vxCore;

Sphere::Sphere() {}

scalar Sphere::radius() const { return m_radius; }

void Sphere::setRadius(const scalar &radius) { m_radius = radius; }

void Sphere::updateBoundingBox()
{
	m_bb->clear();
	m_bb->extend({m_radius,m_radius,m_radius});
	m_bb->extend({-m_radius,-m_radius,-m_radius});
}


bool Sphere::throwRay(const Ray &ray) const
{
	return Geometry::throwRay(ray);
}

int Sphere::throwRay(const Ray &ray, Collision &col) const
{
	col.setValid(false);
	
	//// CAPS
	
	auto&& pos = col.position();
	auto&& levelCenter = v3s{m_bb->midXValue(),m_bb->midYValue(),m_bb->midZValue()};
	auto rey = ray;
	
	rey.setDirection({ray.direction().x(),
					  sZero,
					  ray.direction().z()});
	
	rey.setOrigin({ray.origin().x(),
				   sZero,
				   ray.origin().z()});
	
	auto circleCenter = v3s{m_bb->midXValue(),
								sZero,
								m_bb->midZValue()};
	
	auto d = rey.direction();
	auto f = rey.origin() - circleCenter;
	
	auto a = d.dot(d);
	auto b = scalar(2.0)*f.dot(d);
	auto c = f.dot(f) - (m_radius*m_radius);
	
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

			col.setColor(color());
			col.setPosition(finalPos);
			col.setValid(true);
			return 1;
		}
	}
	
	col.setValid(false);
	return 0;
}

bool Sphere::hasCollision(const Ray &ray) const
{
	return Geometry::hasCollision(ray);
}
