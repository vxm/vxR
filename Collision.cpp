#include "Collision.h"
#include "Ray.h"

namespace vxCore
{

Collision::Collision() {}

Collision::Collision(const v3s &p, const v3s &n, const Color &c, const v2s &uv)
    : m_position(p), m_normal{n}, m_color{c}, m_uv{uv}
{
}

void Collision::setAlphaValue(const scalar a) { m_color.setA(a); }

void Collision::reset()
{
	setValid(false);
	m_position.set(0, 0, 0);
	m_color.set(0, 0, 0);
	m_normal.set(0, 0, 0);
	m_uv.set(0.0, 0.0);
}

void Collision::setValid(bool v) { m_valid = v; }

bool Collision::isValid() const { return m_valid; }

void Collision::setPosition(scalar x, scalar y, scalar z)
{
	m_position.set(x, y, z);
}

v3s Collision::position() const { return m_position; }

void Collision::setColor(scalar r, scalar g, scalar b) { m_color.set(r, g, b); }

Color Collision::color() const { return m_color; }

void Collision::setNormal(const v3s &val) { m_normal = val; }

v3s Collision::normal() const { return m_normal; }

void Collision::setUV(const v2s &uvVec) { m_uv = uvVec; }

void Collision::setU(scalar u) { m_uv.setX(u); }

scalar Collision::u() const { return m_uv.x(); }

void Collision::setV(scalar v) { m_uv.setY(v); }

scalar Collision::v() const { return m_uv.y(); }

v2s Collision::uv() const { return m_uv; }

Ray Collision::nextRay() const
{
	return Ray(m_position + m_normal.small(), m_normal);
}

void Collision::setColor(const Color &val) { m_color = val; }

void Collision::setColor(const v3s &val) { m_color = val; }

void Collision::setPosition(const v3s &val) { m_position = val; }
}
