#include "IsoGeometry.h"

using namespace vxCore;

IsoGeometry::IsoGeometry()
{
	
}

bool IsoGeometry::throwRay(const Ray &ray) const
{
	return Geometry::throwRay(ray);
}

int IsoGeometry::throwRay(const Ray &ray, Collision &col) const
{
	if(!m_bb->throwRay(ray, col))
	{
		return 0;
	}
	
	return 1;
}

bool IsoGeometry::hasCollision(const Ray &ray) const
{
	return Geometry::hasCollision(ray);
}
