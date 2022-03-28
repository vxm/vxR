#include "Ray.h"

using namespace vxCore;

Ray::Ray(const v3s &direction) : m_direction(direction.unit())
{
	placeQuadrant();
}

Ray::Ray(const v3s &origin, const v3s &direction)
	: Ray(origin, direction.unit(), VisionType::kAll)
{
	placeQuadrant();
}

Ray::Ray(const v3s &origin, const v3s &direction, VisionType vis)
	: m_direction(direction.unit()), m_origin(origin), m_vision(vis)
{
	placeQuadrant();
}

Ray::Ray(scalar x, scalar y, scalar z) : m_direction(v3s(x, y, z).unit())
{
	placeQuadrant();
}

Ray::Ray(const Ray &ray)
	: m_direction{ray.m_direction.unit()}, m_origin{ray.m_origin},
	  m_qd(ray.m_qd)
{
}

Ray::Ray(const Ray &&ray)
	: m_direction{std::move(ray.m_direction.unit())}, m_origin{std::move(
														  ray.m_origin)},
	  m_qd(ray.m_qd)
{
}

scalar Ray::length() const { return fabs(m_length); }

void Ray::setLength(const scalar &length) { m_length = fabs(length); }

Quadrant Ray::qd() const { return m_qd; }

void Ray::setQd(const Quadrant &qd) { m_qd = qd; }

void Ray::placeQuadrant()
{
	const auto &d = m_direction;
	if (d.xPositive())
	{
		if (d.yPositive())
		{
			if (d.zPositive())
			{
				m_qd = Quadrant::k1;
			}
			else
			{
				m_qd = Quadrant::k2;
			}
		}
		else
		{
			if (d.zPositive())
			{
				m_qd = Quadrant::k3;
			}
			else
			{
				m_qd = Quadrant::k4;
			}
		}
	}
	else
	{
		if (d.yPositive())
		{
			if (d.zPositive())
			{
				m_qd = Quadrant::k5;
			}
			else
			{
				m_qd = Quadrant::k6;
			}
		}
		else
		{
			if (d.zPositive())
			{
				m_qd = Quadrant::k7;
			}
			else
			{
				m_qd = Quadrant::k8;
			}
		}
	}
}

v3s Ray::origin() const { return m_origin; }

void Ray::setOrigin(const v3s &origin) { m_origin = origin; }

v3s &Ray::direction() { return m_direction; }

v3s Ray::direction() const { return m_direction; }

void Ray::setDirection(const v3s &direction)
{
	m_direction = direction.unit();
	placeQuadrant();
}

scalar Ray::incidence(const v3s &normal) const
{
	return cos(m_direction.angle(normal));
}

scalar Ray::distance(const v3s &final) const
{
	return m_origin.distance(final);
}

v3s Ray::nextPosition() const { return m_origin + m_direction; }

bool Ray::isCloser(const v3s &a, const v3s &b) const
{
	return m_origin.isCloser(a, b);
}
