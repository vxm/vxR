#ifndef MATHUTILS_H
#define MATHUTILS_H

#include<cmath>

#include "vxVector.h"
#include "vxRay.h"
#include "vxPlane.h"
#include "vxCollision.h"

using namespace vxCore;

class MathUtils
{
	public:
		MathUtils();

	// Rationals
	constexpr static double ratio(double a, double b);
	constexpr static double ratio(double a, int b);
	constexpr static double ratio(int a, double b);
	constexpr static double ratio(int a, int b);
	
	// Speed of light
	static const double C;
	static const double PI;

	//Polar coordinates.
	static vxVector2d normalToCartesian(const vxVector3d& normal);
	static vxVector3d cartesianToNormal(const vxVector2d& coords);

	//Intersections
	static vxVector3d rectAndPlane(const vxVector3d&& ray,
								   const vxPlane &plane);
	static vxVector3d rectAndXPlane(const vxVector3d&& ray, double x);
	static vxVector3d rectAndYPlane(const vxVector3d&& ray, double y);
	static vxVector3d rectAndZPlane(const vxVector3d&& ray, double z);
	static double x_forRectAndYPlane(const vxVector3d&& ray, double y);
	static double z_forRectAndYPlane(const vxVector3d&& ray, double y);

	static vxVector3d rayAndXPlane(const vxRay& ray, double x);
	static vxVector3d rayAndYPlane(const vxRay& ray, double y);
	static vxVector3d rayAndZPlane(const vxRay& ray, double z);
	vxVector3d rectAndPlane(const vxRay& ray,
										const vxVector3d&& a, 
										const vxVector3d&& b, 
										const vxVector3d&& c, 
										const vxVector3d&& d) const;
	//Random
	static double getRand(double range);
	static double getBoolRand();
	static vxVector3d getHollowSphereRand(double radius = 1.0);
	static vxVector3d getSphereRand(double radius = 1.0);
	static vxVector3d getHollowHemisphereRand(double radius, const vxVector3d &normal);
	
	// Ranges
	static bool inRange(double r, double min, double max);
	static double clamp(double val, double min, double max);
	static vxColor clamp(const vxColor &c1, const vxColor &min, const vxColor &max);
	static vxColor clamp(const vxColor &c1, double min, double max);

	static double lerp(double a, double b, double r);
	static double remap(double v, double max);
	static double remap(double v, double min, double max);
	static vxColor remap(const vxColor &col, double min, double max);
	static vxColor lerp(const vxColor &c1, const vxColor &c2, double r);

};


/*
 * 	vxVector3d x(1,5,1);
	auto d = 4.45;
	auto r = MathUtils::rectAndXPlane(x, d);

	std::cout << "Rect " << x << std::endl;
	std::cout << "intersects with planeX when x = " << d << std::endl;
	std::cout << "on x = " << r << std::endl;
*/

#endif // MATHUTILS_H
