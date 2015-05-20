#ifndef MathUtils_H
#define MathUtils_H

#include "vxPoint.h"
#include "vxPlane.h"
#include "vxCollision.h"

using namespace vxCore;

class MathUtils
{
	public:
		MathUtils();

	//!! this shouldn't be like this
	//! what a shame.
	inline static vxPoint rectAndPlane(const vxRayXYZ &ray, const vxPlane &plane)
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

	inline static vxPoint rectAndXPlane(const vxRayXYZ &ray, double x)
	{
		// parametric ecuation of the line solved.
		auto t = (x - ray.x()) / -ray.x();
		auto y = t * -ray.y() + ray.y();
		auto z = t * -ray.z() + ray.z();

		return vxPoint(x,y,z);
	}
	
	inline static vxPoint rectAndYPlane(const vxRayXYZ &ray, double y)
	{
		// parametric ecuation of the line solved.
		auto t = (y - ray.y()) / -ray.y();
		auto x = t * -ray.x() + ray.x();
		auto z = t * -ray.z() + ray.z();

		return vxPoint(x,y,z);
	}
	
	inline static vxPoint rectAndZPlane(const vxRayXYZ &ray, double z)
	{
		// parametric ecuation of the line solved.
		auto t = (z - ray.z()) / -ray.z();
		auto x = t * -ray.x() + ray.x();
		auto y = t * -ray.y() + ray.y();

		return vxPoint(x,y,z);
	}

	//Random
	inline static double getRand(double range)
	{
		return range*(rand()/(double)RAND_MAX);
	}

	inline static double getBoolRand()
	{
		return getRand(1.0)<.5;
	}
	

	inline static vxVector3d getHollowSphereRand(double radius)
	{
		//TODO:real random scatter point in sphere missing
		auto r = vxVector3d(getRand(1.0)-.5,getRand(1.0)-.5,getRand(1.0)-.5);
		r.setUnit();
		r=r*radius;
		return r;
	}
	
	
	inline static vxVector3d getSphereRand(double radius = 1.0)
	{
		double mrad = radius/2.0;
		//TODO:real random scatter point in sphere missing
		return vxVector3d(getRand(radius)-mrad,getRand(radius)-mrad,getRand(radius)-mrad);
	}
	
	
	inline static double clamp(double val, double min, double max)
	{
		return std::max(std::min(max, val), min);
	}
	
	inline static vxColor clamp(const vxColor &c1, const vxColor &min, const vxColor &max)
	{
		return vxColor(clamp(c1.r(), min.r(), max.r()),
					   clamp(c1.g(), min.g(), max.g()),
					   clamp(c1.b(), min.b(), max.b()));
	}
	
	inline static vxColor clamp(const vxColor &c1, double min, double max)
	{
		return vxColor(clamp(c1.r(), min, max),
					   clamp(c1.g(), min, max),
					   clamp(c1.b(), min, max));
	}
	
	inline static double remap(double v, double max)
	{
		return clamp(v*max,0.0,max);
	}
	
	inline static auto remap(double v, double min, double max)
	{
		return (v+min)*(max-min);
	}
	
	inline static vxColor remap(const vxColor &col, double min, double max)
	{
		return vxColor((remap(col.r(),min,max)),
						(remap(col.g(),min,max)),
						(remap(col.b(),min,max)));
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
