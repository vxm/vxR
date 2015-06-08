#include "MathUtils.h"

const double MathUtils::C{299'792'458.0};
const double MathUtils::PI {3.141'592'653'589'793'238'462'\
643'383'279'502'884'197'169'399'375'105'820'974'944'592'307'816};

MathUtils::MathUtils()
{
}

vxPoint MathUtils::rectAndPlane(const vxRayXYZ &ray, const vxPlane &plane)
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

vxPoint MathUtils::rectAndXPlane(const vxRayXYZ &ray, double x)
{
	// parametric ecuation of the line solved.
	auto t = (x - ray.x()) / -ray.x();
	auto y = t * -ray.y() + ray.y();
	auto z = t * -ray.z() + ray.z();

	return vxPoint(x,y,z);
}

vxPoint MathUtils::rectAndYPlane(const vxRayXYZ &ray, double y)
{
	// parametric ecuation of the line solved.
	auto t = (y - ray.y()) / -ray.y();
	auto x = t * -ray.x() + ray.x();
	auto z = t * -ray.z() + ray.z();

	return vxPoint(x,y,z);
}

vxPoint MathUtils::rectAndZPlane(const vxRayXYZ &ray, double z)
{
	// parametric ecuation of the line solved.
	auto t = (z - ray.z()) / -ray.z();
	auto x = t * -ray.x() + ray.x();
	auto y = t * -ray.y() + ray.y();

	return vxPoint(x,y,z);
}

double MathUtils::getRand(double range)
{
	return range*(rand()/(double)RAND_MAX);
}

double MathUtils::getBoolRand()
{
	return getRand(1.0)<.5;
}

vxVector3d MathUtils::getHollowSphereRand(double radius)
{
	//auto a = 2.0 * PI * (getRand(PI));
	//auto b = 1.0 / (cos(2.0 * (getRand(PI))-1.0));

	auto a = 2.0 * PI * (getRand(PI));
	auto b = 2.0 * PI * (getRand(PI));

	//TODO:real random scatter point in sphere missing
	auto r = vxVector3d(sin(b),cos(b),sin(a));
	r.setUnit();
	r=r*radius;
	return r;
}

vxVector3d MathUtils::getSphereRand(double radius)
{
	return getHollowSphereRand(getRand(radius));
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
