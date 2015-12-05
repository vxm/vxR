#include "MathUtils.h"
#include "vxVector.h"

using namespace vxCore;

const scalar MathUtils::C{299'792'458.0};
const scalar MathUtils::PI{3.141'592'653'589'793'238'462'\
643'383'279'502'884'197'169'399'375'105'820'974'944'592'307'816};
const scalar MathUtils::HALF_PI{PI/(scalar)2.0};

MathUtils::MathUtils()
{
}

constexpr scalar MathUtils::ratio(scalar a, scalar b)
{
	return a/(scalar)b;
}

constexpr scalar MathUtils::ratio(int a, scalar b)
{
	return a/(scalar)b;
}

constexpr scalar MathUtils::ratio(scalar a, int b)
{
	return a/(scalar)b;
}

constexpr scalar MathUtils::ratio(int a, int b)
{
	return a/(scalar)b;
}

v2 MathUtils::normalToCartesian(const v3& normal)
{
	auto normalized = normal.unit();
	auto x = (PI + atan2(normalized.z(), normalized.x()))/(2.0*PI);
	auto y = ((PI/(scalar)2.0) + asin( normalized.y())) / PI;
	return v2{x, y};
}

v3 MathUtils::cartesianToNormal(const v2& coords)
{
	auto x = cos(coords.x()) * cos(coords.y());
	auto y = cos(coords.x()) * sin(coords.y());
	auto z = sin(coords.x());

	return v3{x,y,z}.unit();
}

v3 MathUtils::rectAndPlane(const v3& ray, 
									const vxPlane &plane)
{
	switch(plane.m_type)
	{
	case vxPlane::type::kZ :
		return rectAndZPlane(std::move(ray), plane.z());
		break;
	case vxPlane::type::kX :
		return rectAndXPlane(std::move(ray), plane.x());
		break;
	case vxPlane::type::kY :
		return rectAndYPlane(std::move(ray), plane.y());
		break;
	default:
		break;
	}
	
	return v3::zero;
}

v3 MathUtils::rectAndPlane(const vxRay& ray,
									const v3& a,
									const v3& b,
									const v3& c)
{
	const auto n = (b-a).cross(c-a);
	const auto &p1 = ray.origin();
	if(n.dot(ray.direction())==0.0)
	{
		return v3::zero;
	}
	
	scalar u = n.dot(a-p1) / n.dot(ray.direction());
	return (ray.direction() * u) + ray.origin();
}

v3 MathUtils::rectAndPlane(const vxRay &ray, vxTriRef &tri)
{
	const auto &n = tri.computeNormal();
	const auto &p1 = ray.origin();
	const auto &c = ray.direction();
	if(n.dot(c)==0.0)
	{
		return v3::zero;
	}
	
	scalar u = n.dot(tri.p1-p1) / n.dot(c);
	return (c * u) + ray.origin();
}
 

scalar MathUtils::area(const v3 &p1, 
					   const v3 &p2, 
					   const v3 &p3)
{
	const auto pb = closestPointInLine(p1,p2,p3);
	const auto b = p1.distance(p2);
	const auto h = p3.distance(pb);
	return (b * h) / 2.0;
}

v3 MathUtils::closestPointInLine(const v3 &p1,
										 const v3 &p2,
										 const v3 &p)
{
	const auto v = p2 - p1;
	const auto w = p - p1;
	const auto c1 = w.dot(v);
	const auto c2 = v.dot(v);

	return p1 + v * (c1 / c2);
}

v3 MathUtils::rectAndXPlane(const v3& ray, scalar x)
{
	// parametric ecuation of the linconst v3 &&ray, - ray.x()) / -ray.x();
	const auto t = (x - ray.x()) / (-ray.x());
	const auto y = t * -ray.y() + ray.y();
	const auto z = t * -ray.z() + ray.z();

	return v3{x,y,z};
}

v3 MathUtils::rectAndYPlane(const v3& ray, scalar y)
{
	// parametric ecuation of the line solved.
	auto t = (y - ray.y()) / (-ray.y());
	auto x = t * (-ray.x()) + ray.x();
	auto z = t * (-ray.z()) + ray.z();

	return v3{x,y,z};
}

scalar MathUtils::x_forRectAndYPlane(const v3& ray, scalar y)
{
	return (y - ray.y()) / (-ray.y()) * (-ray.x()) + ray.x();
}

scalar MathUtils::z_forRectAndYPlane(const v3& ray, scalar y)
{
	return (y - ray.y()) / (-ray.y()) * (-ray.z()) + ray.z();
}

v3 MathUtils::rectAndZPlane(const v3& ray, scalar z)
{
	// parametric ecuation of the linconst v3 &&ray, - ray.z()) / -ray.z();
	auto t = (z - ray.z()) / (-ray.z());
	auto x = t * -ray.x() + ray.x();
	auto y = t * -ray.y() + ray.y();

	return v3{x,y,z};
}

v3 MathUtils::rayAndXPlane(const vxRay &ray, scalar x)
{
	return rectAndXPlane(ray.direction(),x);
}

v3 MathUtils::rayAndYPlane(const vxRay &ray, scalar y)
{
	return rectAndYPlane(ray.direction(),y);
}

v3 MathUtils::rayAndZPlane(const vxRay &ray, scalar z)
{
	return rectAndZPlane(ray.direction(),z);
}
						
scalar MathUtils::getRand(scalar range = 1.0)
{
	return range*(rand()/(scalar)RAND_MAX);
}

scalar MathUtils::getBoolRand()
{
	return getRand(1.0)<.5;
}

v3 MathUtils::getHollowSphereRand(scalar radius)
{
	//TODO:real random scatter point in sphere missing
	auto u = getRand();
	auto v = getRand();
	
	auto a = (2.0 * PI * u );
	auto b = acos( (2.0 * v) - 1.0);

	return cartesianToNormal(v2{a,b}) * radius;
}

v3 MathUtils::getSolidSphereRand2(scalar radius)
{
	return getHollowSphereRand(getRand(radius)*getRand());
}

v3 MathUtils::getSolidSphereRand3(scalar radius)
{
	return getHollowSphereRand(getRand(radius)*getRand()*getRand());
}

v3 MathUtils::getSolidSphereRand(scalar radius)
{
	return getHollowSphereRand(getRand(radius));
}
					   
v3 MathUtils::getHollowHemisphereRand(scalar radius, 
											  const v3 &n)
{
	v3 r;
	do
	{
		r = getHollowSphereRand(radius);
	}
	while(!r.follows(n));
	
	return r;
}

v3 MathUtils::getSphereRand(scalar radius)
{
	return getHollowSphereRand(getRand(radius)-(radius/(scalar)2.0));
}
						
bool MathUtils::inRange(scalar r, scalar min, scalar max)
{
	return std::isgreaterequal(r,min) && std::islessequal(r,max);
}

scalar MathUtils::clamp(scalar val, scalar min, scalar max)
{
	return std::max(std::min(max, val), min);
}

vxColor MathUtils::clamp(const vxColor &c1, const vxColor &min, const vxColor &max)
{
	return vxColor(clamp(c1.r(), min.r(), max.r()),
				   clamp(c1.g(), min.g(), max.g()),
				   clamp(c1.b(), min.b(), max.b()));
}

vxColor MathUtils::clamp(const vxColor &c1, scalar min, scalar max)
{
	return vxColor(clamp(c1.r(), min, max),
				   clamp(c1.g(), min, max),
				   clamp(c1.b(), min, max));
}

unsigned int MathUtils::clamp(unsigned int val, unsigned int min, unsigned int max)
{
	return std::max(std::min(max, val), min);
}

scalar MathUtils::lerp(scalar a, scalar b, scalar r)
{
	return (b*r)+(a*(1.0-r));
}

scalar MathUtils::remap(scalar v, scalar max)
{
	return clamp(v*max,0.0,max);
}

vxColor MathUtils::remap(const vxColor &col, scalar min, scalar max)
{
	return std::move(vxColor((remap(col.r(),min,max)),
							   (remap(col.g(),min,max)),
							   (remap(col.b(),min,max))));
}

vxColor MathUtils::lerp(const vxColor &c1, const vxColor &c2, scalar r)
{	
	return std::move(vxColor(	(lerp(c1.r(), c2.r(), r)),
								(lerp(c1.g(), c2.g(), r)),
								(lerp(c1.b(), c2.b(), r))));
}

						
scalar MathUtils::remap(scalar v, scalar min, scalar max)
{
	return (v+min)*(max-min);
}
					
v3 MathUtils::lerp(const v3 &v1, const v3 &v2, scalar r)
{	
	return v3( lerp(v1.x(), v2.x(), r),
					   lerp(v1.y(), v2.y(), r),
					   lerp(v1.z(), v2.z(), r));
}

scalar MathUtils::scaleFrom01(scalar v, scalar min, scalar max)
{
	return (v+min)*(max-min);
}

scalar MathUtils::scaleTo01(const scalar min, const scalar max, const scalar value)
{
	return (value-min)/(max-min);
}

