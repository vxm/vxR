#ifndef MATHUTILS_H
#define MATHUTILS_H

#include<vxPoint.h>
#include<vxPlane.h>
#include<vxCollision.h>

using namespace vxStorage;

class mathUtils
{
	public:
		mathUtils();

	//!! this shouldn't be like this
	//! what a shame.
	vxPoint rectAndPlane(const vxVector3d &ray, const vxPlane &plane, vxCollision &collide)
	{
		collide.initialize();

		double x, y, z;

		auto px = 0.0;

		auto t = 0.0;

		// parametric ecuation of the line solved.
		t = (x - ray.getX()) / -ray.getX();
		y = (t * -ray.getY()) + ray.getY();
		z = (t * -ray.getZ()) + ray.getZ();

		
	}	


		
};

#endif // MATHUTILS_H
