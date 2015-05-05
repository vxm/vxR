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
	inline static vxPoint rectAndPlane(const vxVector3d &ray, const vxPlane &plane)
	{
		switch(plane.m_type)
		{
			case vxPlane::type::kZ:
				return rectAndXPlane(ray, plane.z());
			break;
			case vxPlane::type::kX:
				return rectAndXPlane(ray, plane.x());
			break;
			case vxPlane::type::kY:
				return rectAndXPlane(ray, plane.y());
			break;
			default:
			break;
		}
		
	}	

	inline static vxPoint rectAndXPlane(const vxVector3d &ray, double x)
	{
		// parametric ecuation of the line solved.
		auto t = (x - ray.x()) / -ray.x();
		auto y = t * -ray.y() + ray.y();
		auto z = t * -ray.z() + ray.z();

		return vxPoint(x,y,z);
	}
	
	inline static vxPoint rectAndYPlane(const vxVector3d &ray, double y)
	{
		// parametric ecuation of the line solved.
		auto t = (y - ray.y()) / -ray.y();
		auto x = t * -ray.x() + ray.x();
		auto z = t * -ray.z() + ray.z();

		return vxPoint(x,y,z);
	}
	
	inline static vxPoint rectAndZPlane(const vxVector3d &ray, double z)
	{
		// parametric ecuation of the line solved.
		auto t = (z - ray.z()) / -ray.z();
		auto x = t * -ray.x() + ray.x();
		auto y = t * -ray.y() + ray.y();

		return vxPoint(x,y,z);
	}

	//Random
	double getRand()
	{
		return (rand()/(double)RAND_MAX);
	}

	double getBoolRand()
	{
		return getRand()<.5;
	}

};


/*
 * 	vxVector3d x(1,5,1);
	auto d = 4.45;
	auto r = MathUtils::rectAndXPlane(x, d);

	std::cout << "Rect " << x << std::endl;
	std::cout << "intersects with planeX when x = " << d << std::endl;
	std::cout << "on x = " << r << std::endl;
*/

#endif // MathUtils_H
