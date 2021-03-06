#ifndef MATHUTILS_H
#define MATHUTILS_H

#include "Plane.h"
#include <cmath>
#include <vector>

namespace vxCore
{

static std::vector<scalar> rand_scalar;
static std::vector<v2s> rand_v2s;
static std::vector<v3s> rand_v3s;

class MathUtils
{
public:
	MathUtils() = default;

	// floating precission
	static scalar equalWithEpsilon(const scalar a, const scalar b);

	// Rationals

	constexpr static scalar ratio(scalar a, scalar b);
	constexpr static scalar ratio(scalar a, int b);
	constexpr static scalar ratio(int a, scalar b);
	constexpr static scalar ratio(int a, int b);

	// Speed of light

	static constexpr scalar epsilon = 0.0001;
	static constexpr scalar C = 299'792'458.0;
	static constexpr scalar PI =
		3.141'592'653'589'793'238'462'643'383'279'502'884'197'169'399'375'105'820'974'944'592'307'816;
	static constexpr scalar HALF_PI = MathUtils::PI / scalar(2.0);

	// Polar coordinates.
	static v2s normalToCartesian(const v3s &normal);
	static v3s cartesianToNormal(const v2s &coords);

	// Intersections
	static v3s rectAndPlane(const v3s &ray, const Plane &plane);
	static v3s rectAndXPlane(const v3s &ray, scalar x);
	static v3s rectAndYPlane(const v3s &ray, scalar y);
	static v3s rectAndZPlane(const v3s &ray, scalar z);
	static scalar x_forRectAndYPlane(const v3s &ray, scalar y);
	static scalar z_forRectAndYPlane(const v3s &ray, scalar y);

	static v3s rayAndXPlane(const Ray &ray, scalar x);
	static v3s rayAndYPlane(const Ray &ray, scalar y);
	static v3s rayAndZPlane(const Ray &ray, scalar z);
	static v3s rectAndPlane(const Ray &ray, const v3s &a, const v3s &b,
							const v3s &c);

	static v3s rectAndPlane(const Ray &ray, TriRef &t);

	// Geometry
	static scalar area(const v3s &p1, const v3s &p2, const v3s &p3);
	///
	/// \brief normal
	/// \param p1
	/// \param p2
	/// \param p3
	/// \return
	/// computes the normal perpendicular
	/// to the plane containing these points.
	static v3s normal(const v3s &p1, const v3s &p2, const v3s &p3);

	// Distances
	static v3s closestPointInLine(const v3s &p1, const v3s &p2, const v3s &p);
	static scalar distanceToLine(const v3s &p1, const v3s &p2, const v3s &p);

	// Random
	static void fillRand();
	static unsigned long m_k;
	static scalar getRand(const scalar range = 1.0);
	static v2s getRand2d(const scalar range);
	static v3s getRand3d(const scalar range);
	static bool getBoolRand(const scalar ratio = 1.0);
	static v3s getSolidSphereRand(scalar radius = 1.0);
	static v3s getSolidSphereRand2(scalar radius = 1.0);
	static v3s getSolidSphereRand3(scalar radius = 1.0);
	static v3s getHollowSphereRand(scalar radius = 1.0);
	static v3s getSphereRand(scalar radius = 1.0);
	static v3s getHollowHemisphereRand(scalar radius, const v3s &n);

	// Ranges
	static bool inRange(scalar r, scalar min, scalar max);
	static scalar clamp(scalar val, scalar min, scalar max);
	static Color clamp(const Color &c1, const Color &min, const Color &max);
	static Color clamp(const Color &c1, scalar min, scalar max);
	static unsigned int clamp(unsigned int v, unsigned int min, unsigned int max);

	static scalar lerp(scalar a, scalar b, scalar r);
	static v3s lerp(const v3s &v1, const v3s &v2s, scalar r);
	static scalar remap(scalar v, scalar max);
	static scalar remap(scalar v, scalar min, scalar max);
	static Color remap(const Color &col, scalar min, scalar max);
	static Color lerp(const Color &c1, const Color &c2, scalar r);

	static scalar scaleFrom01(const scalar v, const scalar min, const scalar max);
	static scalar scaleTo01(const scalar min, const scalar max,
							const scalar value);
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
} // namespace vxCore
#endif // MATHUTILS_H
