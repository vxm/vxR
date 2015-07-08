#include "MathUtils.h"
#include "vxVector.h"

using namespace vxCore;

const double MathUtils::C{299'792'458.0};
const double MathUtils::PI {3.141'592'653'589'793'238'462'\
643'383'279'502'884'197'169'399'375'105'820'974'944'592'307'816};

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
	auto x = (PI + atan2(normalized.z(), normalized.x()))/(2*PI);
	auto y = ((PI/2.0) + asin( normalized.y())) / PI;
	return vxVector2d(x, y);
}

vxVector3d MathUtils::rectAndPlane(const vxVector3d&& ray, const vxPlane &plane)
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
}


vxVector3d MathUtils::rectAndXPlane(const vxVector3d&& ray, double x)
{
	// parametric ecuation of the linconst vxVector3d &&ray, - ray.x()) / -ray.x();
	auto t = (x - ray.x()) / (-ray.x());
	auto y = t * -ray.y() + ray.y();
	auto z = t * -ray.z() + ray.z();

	return vxVector3d(x,y,z);
}

vxVector3d MathUtils::rectAndYPlane(const vxVector3d&& ray, double y)
{
	// parametric ecuation of the line solved.
	auto t = (y - ray.y()) / (-ray.y());
	auto x = t * (-ray.x()) + ray.x();
	auto z = t * (-ray.z()) + ray.z();

	return vxVector3d(x,y,z);
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

	return vxVector3d(x,y,z);
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
	auto a = 2.0 * PI * (getRand());
	//auto b = acos(2.0 * getRand() - 1.0);
	auto b = 2.0 * PI * (getRand());
	
	auto x = radius * cos(a) * cos(b);
	auto y = radius * cos(a) * sin(b);
	auto z = radius * sin(a);

	return vxVector3d(x,y,z);
}

vxVector3d MathUtils::getHollowHemisphereRand(double radius, const vxVector3d &normal)
{
	auto r = getHollowSphereRand(radius);
	return r;
}

vxVector3d MathUtils::getSphereRand(double radius)
{
	return getHollowSphereRand(getRand(radius));
}
						
bool MathUtils::inRange(double r, double min, double max)
{
	return r>=min && r<=max;
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
	return (b*r)-(a*(1.0-r));
}

double MathUtils::remap(double v, double max)
{
	return clamp(v*max,0.0,max);
}

vxColor MathUtils::remap(const vxColor &col, double min, double max)
{
	return vxColor((remap(col.r(),min,max)),
				   (remap(col.g(),min,max)),
				   (remap(col.b(),min,max)));
}

double MathUtils::remap(double v, double min, double max)
{
	return (v+min)*(max-min);
}
