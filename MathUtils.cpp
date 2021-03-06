#include "MathUtils.h"
#include "Collision.h"
#include "Plane.h"
#include "Ray.h"
#include "TriRef.h"
#include "Vector.h"

#define USE_STORE_RAND 0

#include <iostream>
#include <random>

thread_local std::random_device rd;  // Will be used to obtain a seed for the
									 // random number engine
thread_local std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded
									 // with rd()
thread_local std::uniform_real_distribution<> dis(0.0, 1.0);

using namespace vxCore;

//////////////////////////////////////
///////////////  RANDOM CACHES ///////
unsigned long MathUtils::m_k = 0u;
constexpr const auto cached_random = 12000u;

void MathUtils::fillRand()
{
#if USE_STORE_RAND
	rand_scalar.reserve(cached_random);
	rand_v2s.reserve(cached_random);
	rand_v3s.reserve(cached_random);
	for (auto i = 0u; i < cached_random; i++)
	{
		rand_scalar.push_back((rand() / (scalar)RAND_MAX));
		rand_v2s.emplace_back((rand() / (scalar)RAND_MAX),
							  (rand() / (scalar)RAND_MAX));
		rand_v3s.emplace_back((rand() / (scalar)RAND_MAX),
							  (rand() / (scalar)RAND_MAX),
							  (rand() / (scalar)RAND_MAX));
	}
#endif
}
///////////////  RANDOM CACHES ///////
//////////////////////////////////////

///
/// \brief MathUtils::MathUtils
///
scalar MathUtils::equalWithEpsilon(const scalar a, const scalar b)
{
	return scalar(fabs(a - b) < MathUtils::epsilon);
}

constexpr scalar MathUtils::ratio(scalar a, scalar b) { return a / (scalar)b; }

constexpr scalar MathUtils::ratio(int a, scalar b) { return a / (scalar)b; }

constexpr scalar MathUtils::ratio(scalar a, int b) { return a / (scalar)b; }

constexpr scalar MathUtils::ratio(int a, int b) { return a / (scalar)b; }

v2s MathUtils::normalToCartesian(const v3s &normal)
{
	auto normalized = normal.unit();
	auto x =
		(scalar)(PI + atan2(normalized.z(), normalized.x())) / (scalar(2.0) * PI);
	auto y = (scalar)((PI / scalar(2.0)) + asin(normalized.y())) / PI;
	return v2s{x, y};
}

v3s MathUtils::cartesianToNormal(const v2s &coords)
{
	auto x = (scalar)cos(coords.x()) * cos(coords.y());
	auto y = (scalar)cos(coords.x()) * sin(coords.y());
	auto z = (scalar)sin(coords.x());

	return v3s{x, y, z}.unit();
}

v3s MathUtils::rectAndPlane(const v3s &ray, const Plane &plane)
{
	switch (plane.m_type)
	{
	case Plane::type::kZ:
		return rectAndZPlane(ray, plane.z());
	case Plane::type::kX:
		return rectAndXPlane(ray, plane.x());
	case Plane::type::kY:
		return rectAndYPlane(ray, plane.y());
	default:
		break;
	}

	return v3s::zero;
}

v3s MathUtils::rectAndPlane(const Ray &ray, const v3s &a, const v3s &b,
							const v3s &c)
{
	const auto n = MU::normal(a, b, c);
	const auto &p1 = ray.origin();
	if (n.dot(ray.direction()) == 0.0)
	{
		return v3s::zero;
	}

	scalar u = n.dot(a - p1) / n.dot(ray.direction());
	return (ray.direction() * u) + ray.origin();
}

v3s MathUtils::rectAndPlane(const Ray &ray, TriRef &tri)
{
	const auto &n = tri.normal();
	const auto &p1 = ray.origin();
	const auto &c = ray.direction();
	if (n.dot(c) == 0.0)
	{
		return v3s::zero;
	}

	scalar u = n.dot(tri.p1 - p1) / n.dot(c);
	return (c * u) + ray.origin();

	/*
	  v3s MathUtils::rayAndZPlane(const Ray &ray, scalar z)
{
	v3s planePoint = {0.0, 0.0, z};
	v3s diff = ray.origin() - planePoint;
	scalar prod1 = diff.dot({0.0, 0.0, 1.0});
	scalar prod2 = ray.direction().dot({0.0, 0.0, 1.0});
	scalar prod3 = prod1 / prod2;
	return ray.origin() - ray.direction() * prod3;
}
*/
}

scalar MathUtils::area(const v3s &p1, const v3s &p2, const v3s &p3)
{
	const auto pb = closestPointInLine(p1, p2, p3);
	const auto b = p1.distance(p2);
	const auto h = p3.distance(pb);
	return (b * h) / scalar(2.0);
}

v3s MathUtils::normal(const v3s &p1, const v3s &p2, const v3s &p3)
{
	return (p2 - p1).cross(p3 - p1).unit();
}

v3s MathUtils::closestPointInLine(const v3s &p1, const v3s &p2, const v3s &p)
{
	auto &&v = p2 - p1;
	auto &&w = p - p1;
	auto c1 = w.dot(v);
	auto c2 = v.dot(v);

	return p1 + v * (c1 / c2);
}

scalar MathUtils::distanceToLine(const v3s &p1, const v3s &p2, const v3s &p)
{
	const auto pb = MU::closestPointInLine(p1, p2, p);
	return p.distance(pb);
}

v3s MathUtils::rectAndXPlane(const v3s &ray, scalar x)
{
	const auto t = (x - ray.x()) / (-ray.x());
	const auto y = t * -ray.y() + ray.y();
	const auto z = t * -ray.z() + ray.z();

	return {x, y, z};
}

v3s MathUtils::rectAndYPlane(const v3s &ray, scalar y)
{
	auto t = (y - ray.y()) / (-ray.y());
	auto x = t * (-ray.x()) + ray.x();
	auto z = t * (-ray.z()) + ray.z();

	return {x, y, z};
}

v3s MathUtils::rectAndZPlane(const v3s &ray, scalar z)
{
	auto t = (z - ray.z()) / (-ray.z());
	auto x = t * -ray.x() + ray.x();
	auto y = t * -ray.y() + ray.y();

	return {x, y, z};
}

scalar MathUtils::x_forRectAndYPlane(const v3s &ray, scalar y)
{
	return (y - ray.y()) / (-ray.y()) * (-ray.x()) - ray.x();
}

scalar MathUtils::z_forRectAndYPlane(const v3s &ray, scalar y)
{
	return (y - ray.y()) / (-ray.y()) * (-ray.z()) - ray.z();
}

v3s MathUtils::rayAndXPlane(const Ray &ray, scalar x)
{
	const scalar prod1 = ray.origin().x() - x;
	const scalar prod2 = ray.direction().x();
	const scalar prod3 = prod1 / prod2;
	return ray.origin() - ray.direction() * prod3;
}

v3s MathUtils::rayAndYPlane(const Ray &ray, scalar y)
{
	const scalar prod1 = ray.origin().y() - y;
	const scalar prod2 = ray.direction().y();
	const scalar prod3 = prod1 / prod2;
	return ray.origin() - ray.direction() * prod3;
}

v3s MathUtils::rayAndZPlane(const Ray &ray, scalar z)
{
	const scalar prod1 = ray.origin().z() - z;
	const scalar prod2 = ray.direction().z();
	const scalar prod3 = prod1 / prod2;
	return ray.origin() - ray.direction() * prod3;
}

scalar MathUtils::getRand(const scalar range)
{
#if USE_STORE_RAND
	return range * rand_scalar[(m_k++) % cached_random];
#else
	return dis(gen) * range;
#endif
}

v2s MathUtils::getRand2d(const scalar range)
{
	return rand_v2s[(m_k++) % cached_random] * range;
}

v3s MathUtils::getRand3d(const scalar range)
{
	return rand_v3s[(m_k++) % cached_random] * range;
}

bool MathUtils::getBoolRand(const scalar ratio) { return getRand(1.0) < ratio; }

v3s MathUtils::getHollowSphereRand(scalar radius)
{
	// TODO:real random scatter point in sphere missing
	auto u = getRand();
	auto v = getRand();

	auto a = (scalar)(scalar(2.0) * PI * u);
	auto b = (scalar)acos((scalar(2.0) * v) - 1.0);

	return cartesianToNormal(v2s{a, b}) * radius;
}

v3s MathUtils::getSolidSphereRand2(scalar radius)
{
	return getHollowSphereRand(getRand(radius) * getRand());
}

v3s MathUtils::getSolidSphereRand3(scalar radius)
{
	return getHollowSphereRand(getRand(radius) * getRand() * getRand());
}

v3s MathUtils::getSolidSphereRand(scalar radius)
{
	return getHollowSphereRand(getRand(radius));
}

v3s MathUtils::getHollowHemisphereRand(scalar radius, const v3s &n)
{
	auto r = getHollowSphereRand(radius);

	if (!n.follows(r))
	{
		r.invert();
	}

	return r;
}

v3s MathUtils::getSphereRand(scalar radius)
{
	return getHollowSphereRand(getRand(radius) - (radius / scalar(2.0)));
}

bool MathUtils::inRange(scalar r, scalar min, scalar max)
{
	return std::isgreaterequal(r, std::min(min, max)) &&
		   std::islessequal(r, std::max(min, max));
}

scalar MathUtils::clamp(scalar val, scalar min, scalar max)
{
	return std::max(std::min(max, val), min);
}

Color MathUtils::clamp(const Color &c1, const Color &min, const Color &max)
{
	return {clamp(c1.r(), min.r(), max.r()), clamp(c1.g(), min.g(), max.g()),
			clamp(c1.b(), min.b(), max.b()),
			clamp(c1.alpha(), min.alpha(), max.alpha())};
}

Color MathUtils::clamp(const Color &c1, scalar min, scalar max)
{
	return {clamp(c1.r(), min, max), clamp(c1.g(), min, max),
			clamp(c1.b(), min, max), clamp(c1.alpha(), min, max)};
}

unsigned int MathUtils::clamp(unsigned int val, unsigned int min,
							  unsigned int max)
{
	return std::max(std::min(max, val), min);
}

scalar MathUtils::lerp(scalar a, scalar b, scalar r)
{
	return (b * r) + (a * (1.0 - r));
}

scalar MathUtils::remap(scalar v, scalar max)
{
	return clamp(v * max, 0.0, max);
}

Color MathUtils::remap(const Color &col, scalar min, scalar max)
{
	return {remap(col.r(), min, max), remap(col.g(), min, max),
			remap(col.b(), min, max), remap(col.alpha(), min, max)};
}

Color MathUtils::lerp(const Color &c1, const Color &c2, scalar r)
{
	return {lerp(c1.r(), c2.r(), r), lerp(c1.g(), c2.g(), r),
			lerp(c1.b(), c2.b(), r), lerp(c1.b(), c2.b(), r)};
}

scalar MathUtils::remap(scalar v, scalar min, scalar max)
{
	return (v + min) * (max - min);
}

v3s MathUtils::lerp(const v3s &v1, const v3s &v2, scalar r)
{
	return v3s(lerp(v1.x(), v2.x(), r), lerp(v1.y(), v2.y(), r),
			   lerp(v1.z(), v2.z(), r));
}

scalar MathUtils::scaleFrom01(scalar v, scalar min, scalar max)
{
	return (v + min) * (max - min);
}

scalar MathUtils::scaleTo01(const scalar min, const scalar max,
							const scalar value)
{
	return (value - min) / (max - min);
}
