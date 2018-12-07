#include "Ray.h"

using namespace vxCore;

scalar Ray::length() const { return m_length; }

void Ray::setLength(const scalar &length) { m_length = length; }

Ray::Ray() {}

Ray::Ray(const v3s &direction) : m_direction(direction.unit()) {}

Ray::Ray(const v3s &origin, const v3s &direction)
	: Ray(origin, direction, VisionType::kAll)
{
}

Ray::Ray(const v3s &origin, const v3s &direction, VisionType vis)
	: m_direction(direction.unit()), m_origin(origin), m_vision(vis)
{
}

Ray::Ray(scalar x, scalar y, scalar z) : m_direction(v3s{x, y, z}.unit()) {}

Ray::Ray(const Ray &ray) : m_direction{ray.m_direction}, m_origin{ray.m_origin}
{
}

Ray::Ray(const Ray &&ray)
	: m_direction{std::move(ray.m_direction)}, m_origin{std::move(ray.m_origin)}
{
}

v3s Ray::origin() const { return m_origin; }

void Ray::setOrigin(const v3s &origin) { m_origin = origin; }

v3s &Ray::direction() { return m_direction; }

v3s Ray::direction() const { return m_direction; }

void Ray::setDirection(const v3s &direction) { m_direction = direction.unit(); }

scalar Ray::incidence(const v3s &normal) const
{
	return cos(m_direction.angle(normal));
}

scalar Ray::distance(const v3s &final) const
{
	return m_origin.distance(final);
}

bool Ray::isCloser(const v3s &a, const v3s &b) const
{
	return m_origin.isCloser(a, b);
}
