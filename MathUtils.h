#ifndef MATHUTILS_H
#define MATHUTILS_H

#include<cmath>

#include "vxVector.h"
#include "vxRay.h"
#include "vxPlane.h"
#include "vxCollision.h"
#include "vxTriRef.h"

using namespace vxCore;

class MathUtils
{
	public:
		MathUtils();

	// Rationals
	constexpr static scalar ratio(scalar a, scalar b);
	constexpr static scalar ratio(scalar a, int b);
	constexpr static scalar ratio(int a, scalar b);
	constexpr static scalar ratio(int a, int b);
	
	// Speed of light
	static const scalar C;
	static const scalar PI;
	static const scalar HALF_PI;

	//Polar coordinates.
	static vxVector2d normalToCartesian(const vxVector3d& normal);
	static vxVector3d cartesianToNormal(const vxVector2d& coords);

	//Intersections
	static vxVector3d rectAndPlane(const vxVector3d&& ray,
								   const vxPlane &plane);
	static vxVector3d rectAndXPlane(const vxVector3d&& ray, scalar x);
	static vxVector3d rectAndYPlane(const vxVector3d&& ray, scalar y);
	static vxVector3d rectAndZPlane(const vxVector3d&& ray, scalar z);
	static scalar x_forRectAndYPlane(const vxVector3d&& ray, scalar y);
	static scalar z_forRectAndYPlane(const vxVector3d&& ray, scalar y);

	static vxVector3d rayAndXPlane(const vxRay& ray, scalar x);
	static vxVector3d rayAndYPlane(const vxRay& ray, scalar y);
	static vxVector3d rayAndZPlane(const vxRay& ray, scalar z);
	static vxVector3d rectAndPlane(const vxRay& ray,
										const vxVector3d& a, 
										const vxVector3d& b, 
										const vxVector3d& c);
	
	static vxVector3d rectAndPlane(const vxRay& ray, vxTriRef& t);

	//Geometry
	static scalar area(const vxVector3d &p1,
					   const vxVector3d &p2,
					   const vxVector3d &p3);
	
	//Distances
	static vxVector3d closestPointInLine(const vxVector3d &p1,
										 const vxVector3d &p2,
										 const vxVector3d &p);
	
	//Random
	static scalar getRand(scalar range);
	static scalar getBoolRand();
	static vxVector3d getSolidSphereRand(scalar radius);
	static vxVector3d getSolidSphereRand2(scalar radius);
	static vxVector3d getSolidSphereRand3(scalar radius);
	static vxVector3d getHollowSphereRand(scalar radius = 1.0);
	static vxVector3d getSphereRand(scalar radius = 1.0);
	static vxVector3d getHollowHemisphereRand(scalar radius, const vxVector3d &);
	
	// Ranges
	static bool inRange(scalar r, scalar min, scalar max);
	static scalar clamp(scalar val, scalar min, scalar max);
	static vxColor clamp(const vxColor &c1, const vxColor &min, const vxColor &max);
	static vxColor clamp(const vxColor &c1, scalar min, scalar max);

	static scalar lerp(scalar a, scalar b, scalar r);
	static vxVector3d lerp(const vxVector3d &v1, const vxVector3d &v2, scalar r);
	static scalar remap(scalar v, scalar max);
	static scalar remap(scalar v, scalar min, scalar max);
	static vxColor remap(const vxColor &col, scalar min, scalar max);
	static vxColor lerp(const vxColor &c1, const vxColor &c2, scalar r);
	
	static scalar scaleFrom01(const scalar v, const scalar min, const scalar max);
	static scalar scaleTo01(const scalar min, const scalar max, const scalar value);
};


/*
 * 	vxVector3d x(1,5,1);
	auto d = 4.45;
	auto r = MathUtils::rectAndXPlane(x, d);

	std::cout << "Rect " << x << std::endl;
	std::cout << "intersects with planeX when x = " << d << std::endl;
	std::cout << "on x = " << r << std::endl;
*/

using MU = MathUtils;

#endif // MATHUTILS_H
