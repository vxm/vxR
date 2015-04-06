#ifndef MathUtils_H
#define MathUtils_H

#include<vxPoint.h>
#include<vxPlane.h>
#include<vxCollision.h>

using namespace vxStorage;

class MathUtils
{
	public:
		MathUtils();

	//!! this shouldn't be like this
	//! what a shame.
	static vxPoint rectAndPlane(const vxVector3d &ray, const vxPlane &plane)
	{
		switch(plane.m_type)
		{
			case vxPlane::type::kX:
				return rectAndXPlane(ray, plane.x());
			break;
			case vxPlane::type::kY:
				return rectAndXPlane(ray, plane.y());
			break;
			case vxPlane::type::kZ:
				return rectAndXPlane(ray, plane.z());
			break;
			default:
			break;
		}
		
	}	

	static vxPoint rectAndXPlane(const vxVector3d &ray, double x)
	{
		// parametric ecuation of the line solved.
		auto t = (x - ray.getX()) / -ray.getX();
		auto y = t * -ray.getY() + ray.getY();
		auto z = t * -ray.getZ() + ray.getZ();

		return vxPoint(x,y,z);
	}

		
};

#endif // MathUtils_H
