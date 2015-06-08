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

	// Speed of light
	static const double C;
	static const double PI;

	//!! this shouldn't be like this
	//! what a shame.
	static vxPoint rectAndPlane(const vxRayXYZ &ray, const vxPlane &plane);
	static vxPoint rectAndXPlane(const vxRayXYZ &ray, double x);
	static vxPoint rectAndYPlane(const vxRayXYZ &ray, double y);
	static vxPoint rectAndZPlane(const vxRayXYZ &ray, double z);

	//Random
	static double getRand(double range);
	static double getBoolRand();
	static vxVector3d getHollowSphereRand(double radius = 1.0);
	static vxVector3d getSphereRand(double radius = 1.0);
	
	static double clamp(double val, double min, double max);
	static vxColor clamp(const vxColor &c1, const vxColor &min, const vxColor &max);
	static vxColor clamp(const vxColor &c1, double min, double max);

	static double lerp(double a, double b, double r);
	static double remap(double v, double max);
	static double remap(double v, double min, double max);
	static vxColor remap(const vxColor &col, double min, double max);
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
