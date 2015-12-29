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
	static v2 normalToCartesian(const v3& normal);
	static v3 cartesianToNormal(const v2& coords);

	//Intersections
	static v3 rectAndPlane(const v3& ray,
								   const vxPlane &plane);
	static v3 rectAndXPlane(const v3& ray, scalar x);
	static v3 rectAndYPlane(const v3& ray, scalar y);
	static v3 rectAndZPlane(const v3& ray, scalar z);
	static scalar x_forRectAndYPlane(const v3& ray, scalar y);
	static scalar z_forRectAndYPlane(const v3& ray, scalar y);

	static v3 rayAndXPlane(const vxRay& ray, scalar x);
	static v3 rayAndYPlane(const vxRay& ray, scalar y);
	static v3 rayAndZPlane(const vxRay& ray, scalar z);
	static v3 rectAndPlane(const vxRay& ray,
										const v3& a, 
										const v3& b, 
										const v3& c);
	
	static v3 rectAndPlane(const vxRay& ray, vxTriRef& t);

	//Geometry
	static scalar area(const v3 &p1,
					   const v3 &p2,
					   const v3 &p3);
	
	//Distances
	static v3 closestPointInLine(const v3 &p1,
										 const v3 &p2,
										 const v3 &p);
	
	//Random
	static scalar getRand(scalar range = 1.0);
	static scalar getBoolRand(scalar ratio = 1.0);
	static v3 getSolidSphereRand(scalar radius = 1.0);
	static v3 getSolidSphereRand2(scalar radius = 1.0);
	static v3 getSolidSphereRand3(scalar radius = 1.0);
	static v3 getHollowSphereRand(scalar radius = 1.0);
	static v3 getSphereRand(scalar radius = 1.0);
	static v3 getHollowHemisphereRand(scalar radius, const v3 &n);
	
	// Ranges
	static bool inRange(scalar r, scalar min, scalar max);
	static scalar clamp(scalar val, scalar min, scalar max);
	static vxColor clamp(const vxColor &c1, const vxColor &min, const vxColor &max);
	static vxColor clamp(const vxColor &c1, scalar min, scalar max);
	static unsigned int clamp(unsigned int v, unsigned int min, unsigned int max);
	
	static scalar lerp(scalar a, scalar b, scalar r);
	static v3 lerp(const v3 &v1, const v3 &v2, scalar r);
	static scalar remap(scalar v, scalar max);
	static scalar remap(scalar v, scalar min, scalar max);
	static vxColor remap(const vxColor &col, scalar min, scalar max);
	static vxColor lerp(const vxColor &c1, const vxColor &c2, scalar r);
	
	static scalar scaleFrom01(const scalar v, const scalar min, const scalar max);
	static scalar scaleTo01(const scalar min, const scalar max, const scalar value);
};


/*
 * 	v3 x(1,5,1);
	auto d = 4.45;
	auto r = MathUtils::rectAndXPlane(x, d);

	std::cout << "Rect " << x << std::endl;
	std::cout << "intersects with planeX when x = " << d << std::endl;
	std::cout << "on x = " << r << std::endl;
*/

using MU = MathUtils;

#endif // MATHUTILS_H
