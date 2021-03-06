#include "Sphere.h"

using namespace vxCore;

scalar Sphere::radius() const { return m_radius; }

void Sphere::setRadius(const scalar &radius) { m_radius = radius; }

void Sphere::updateBoundingBox()
{
	const auto &t = m_transform.origin();

	m_bb->clear();
	m_bb->extend({m_radius + t.x(), m_radius + t.y(), m_radius + t.z()});
	m_bb->extend({-m_radius + t.x(), -m_radius + t.y(), -m_radius + t.z()});
}

bool Sphere::throwRay(const Ray &ray) const { return Geometry::throwRay(ray); }

int Sphere::throwRay(const Ray &ray, Collision &col) const
{
	auto d = ray.direction();
	auto f = ray.origin() - m_bb->center();

	auto a = d.dot(d);
	auto b = scalar(2.0) * f.dot(d);
	auto c = f.dot(f) - (m_radius * m_radius);

	scalar disc = (b * b) - (scalar(4.0) * (a * c));
	if (disc >= 0.0)
	{
		disc = sqrt(disc);

		scalar t1 = (-b - disc) / (scalar(2.0) * a);
		if (t1 >= 0.0)
		{
			v3s twoPos = {f + (ray.direction() * t1)};

			auto finalPos = MU::rectAndZPlane(ray.direction(), twoPos.z() - f.z());
			finalPos += f;

			col.setColor(m_color);
			col.setPosition(finalPos + m_bb->center());
			col.setNormal((col.position() - m_bb->center()).unit());
			col.setValid(true);
			return 1;
		}
	}

	col.setValid(false);
	return 0;
}

bool Sphere::hasCollision(const Ray &ray) const
{
	auto d = ray.direction();
	auto f = ray.origin() - m_bb->center();

	auto b = scalar(2.0) * f.dot(d);
	auto c = f.dot(f) - (m_radius * m_radius);

	scalar disc = (b * b) - (scalar(4.0) * (d.dot(d) * c));
	if (disc >= 0.0)
	{
		scalar t1 = (-b - sqrt(disc)) / (scalar(2.0) * d.dot(d));
		if (t1 >= 0.0)
		{
			return true;
		}
	}

	return false;
}
