#include "MathUtils.h"
#include "vxVector.h"

using namespace vxCore;

const double MathUtils::C{299'792'458.0};
const double MathUtils::PI{3.141'592'653'589'793'238'462'\
643'383'279'502'884'197'169'399'375'105'820'974'944'592'307'816};
const double MathUtils::HALF_PI{PI/2.0};

MathUtils::MathUtils()
{
}

constexpr double MathUtils::ratio(double a, double b)
{
	return a/(double)b;
}

constexpr double MathUtils::ratio(int a, double b)
{
	return a/(double)b;
}

constexpr double MathUtils::ratio(double a, int b)
{
	return a/(double)b;
}

constexpr double MathUtils::ratio(int a, int b)
{
	return a/(double)b;
}

vxVector2d MathUtils::normalToCartesian(const vxVector3d& normal)
{
	auto normalized = normal.unit();
	auto x = (PI + atan2(normalized.z(), normalized.x()))/(2.0*PI);
	auto y = ((PI/2.0) + asin( normalized.y())) / PI;
	return vxVector2d{x, y};
}

vxVector3d MathUtils::cartesianToNormal(const vxVector2d& coords)
{
	auto x = cos(coords.x()) * cos(coords.y());
	auto y = cos(coords.x()) * sin(coords.y());
	auto z = sin(coords.x());

	return vxVector3d{x,y,z}.unit();
}

vxVector3d MathUtils::rectAndPlane(const vxVector3d&& ray, 
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
	
	return vxVector3d::zero;
}

vxVector3d MathUtils::rectAndPlane(const vxRay& ray,
									const vxVector3d& a,
									const vxVector3d& b,
									const vxVector3d& c)
{
	const auto n = (b-a).cross(c-a);
	const auto &p1 = ray.origin();
	if(n.dot(ray.direction())==0.0)
	{
		return vxVector3d::zero;
	}
	
	double u = n.dot(a-p1) / n.dot(ray.direction());
	return (ray.direction() * u) + ray.origin();
}

vxVector3d MathUtils::rectAndPlane(const vxRay &ray, vxTriRef &tri)
{
	const auto &n = tri.computeNormal();
	const auto &p1 = ray.origin();
	const auto &c = ray.direction();
	if(n.dot(c)==0.0)
	{
		return vxVector3d::zero;
	}
	
	double u = n.dot(tri.p1-p1) / n.dot(c);
	return (c * u) + ray.origin();
}
 

double MathUtils::area(const vxVector3d &p1, 
					   const vxVector3d &p2, 
					   const vxVector3d &p3)
{
	const auto pb = closestPointInLine(p1,p2,p3);
	const auto b = p1.distance(p2);
	const auto h = p3.distance(pb);
	return (b * h) / 2.0;
}

vxVector3d MathUtils::closestPointInLine(const vxVector3d &p1,
										 const vxVector3d &p2,
										 const vxVector3d &p)
{
	const auto v = p2 - p1;
	const auto w = p - p1;
	const auto c1 = w.dot(v);
	const auto c2 = v.dot(v);

	return p1 + v * (c1 / c2);
}

vxVector3d MathUtils::rectAndXPlane(const vxVector3d&& ray, double x)
{
	// parametric ecuation of the linconst vxVector3d &&ray, - ray.x()) / -ray.x();
	const auto t = (x - ray.x()) / (-ray.x());
	const auto y = t * -ray.y() + ray.y();
	const auto z = t * -ray.z() + ray.z();

	return vxVector3d{x,y,z};
}

vxVector3d MathUtils::rectAndYPlane(const vxVector3d&& ray, double y)
{
	// parametric ecuation of the line solved.
	auto t = (y - ray.y()) / (-ray.y());
	auto x = t * (-ray.x()) + ray.x();
	auto z = t * (-ray.z()) + ray.z();

	return vxVector3d{x,y,z};
}

double MathUtils::x_forRectAndYPlane(const vxVector3d&& ray, double y)
{
	return (y - ray.y()) / (-ray.y()) * (-ray.x()) + ray.x();
}

double MathUtils::z_forRectAndYPlane(const vxVector3d&& ray, double y)
{
	return (y - ray.y()) / (-ray.y()) * (-ray.z()) + ray.z();
}

vxVector3d MathUtils::rectAndZPlane(const vxVector3d&& ray, double z)
{
	// parametric ecuation of the linconst vxVector3d &&ray, - ray.z()) / -ray.z();
	auto t = (z - ray.z()) / (-ray.z());
	auto x = t * -ray.x() + ray.x();
	auto y = t * -ray.y() + ray.y();

	return vxVector3d{x,y,z};
}

vxVector3d MathUtils::rayAndXPlane(const vxRay &ray, double x)
{
	return rectAndXPlane(ray.direction(),x);
}

vxVector3d MathUtils::rayAndYPlane(const vxRay &ray, double y)
{
	return rectAndYPlane(ray.direction(),y);
}

vxVector3d MathUtils::rayAndZPlane(const vxRay &ray, double z)
{
	return rectAndZPlane(ray.direction(),z);
}
						
double MathUtils::getRand(double range = 1.0)
{
	return range*(rand()/(double)RAND_MAX);
}

double MathUtils::getBoolRand()
{
	return getRand(1.0)<.5;
}

vxVector3d MathUtils::getHollowSphereRand(double radius)
{
	//TODO:real random scatter point in sphere missing
	auto u = getRand();
	auto v = getRand();
	
	auto a = (2.0 * PI * u );
	auto b = acos( (2.0 * v) - 1.0);

	return cartesianToNormal(vxVector2d{a,b}) * radius;
}

vxVector3d MathUtils::getSolidSphereRand2(double radius)
{
	return getHollowSphereRand(getRand(radius)*getRand());
}

vxVector3d MathUtils::getSolidSphereRand3(double radius)
{
	return getHollowSphereRand(getRand(radius)*getRand()*getRand());
}


vxVector3d MathUtils::getSolidSphereRand(double radius)
{
	return getHollowSphereRand(getRand(radius));
}
					   
vxVector3d MathUtils::getHollowHemisphereRand(double radius, 
											  const vxVector3d &)
{
	auto r = getHollowSphereRand(radius);
	//TODO:implement half of sphere
	return r;
}

vxVector3d MathUtils::getSphereRand(double radius)
{
	return getHollowSphereRand(getRand(radius)-(radius/2.0));
}
						
bool MathUtils::inRange(double r, double min, double max)
{
	return std::isgreaterequal(r,min) && std::islessequal(r,max);
}

double MathUtils::clamp(double val, double min, double max)
{
	return std::max(std::min(max, val), min);
}

vxColor MathUtils::clamp(const vxColor &c1, const vxColor &min, const vxColor &max)
{
	return vxColor(clamp(c1.r(), min.r(), max.r()),
				   clamp(c1.g(), min.g(), max.g()),
				   clamp(c1.b(), min.b(), max.b()));
}

vxColor MathUtils::clamp(const vxColor &c1, double min, double max)
{
	return vxColor(clamp(c1.r(), min, max),
				   clamp(c1.g(), min, max),
				   clamp(c1.b(), min, max));
}

double MathUtils::lerp(double a, double b, double r)
{
	return (b*r)+(a*(1.0-r));
}

double MathUtils::remap(double v, double max)
{
	return clamp(v*max,0.0,max);
}

vxColor MathUtils::remap(const vxColor &col, double min, double max)
{
	return std::move(vxColor((remap(col.r(),min,max)),
							   (remap(col.g(),min,max)),
							   (remap(col.b(),min,max))));
}

vxColor MathUtils::lerp(const vxColor &c1, const vxColor &c2, double r)
{	
	return std::move(vxColor(	(lerp(c1.r(), c2.r(), r)),
								(lerp(c1.g(), c2.g(), r)),
								(lerp(c1.b(), c2.b(), r))));
}

						
double MathUtils::remap(double v, double min, double max)
{
	return (v+min)*(max-min);
}
					
vxVector3d MathUtils::lerp(const vxVector3d &v1, const vxVector3d &v2, double r)
{	
	return vxVector3d( lerp(v1.x(), v2.x(), r),
					   lerp(v1.y(), v2.y(), r),
					   lerp(v1.z(), v2.z(), r));
}

double MathUtils::scaleFrom01(double v, double min, double max)
{
	return (v+min)*(max-min);
}
