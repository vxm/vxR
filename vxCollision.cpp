#include "vxCollision.h"
namespace vxCore{

vxCollision::vxCollision()
{
}

vxCollision::vxCollision(const v3s &p, const v3s &n, const vxColor &c, const v2s &uv)
	: m_position(p)
	, m_normal{n}
	, m_color{c}
	, m_uv{uv}
{
}

void vxCollision::reset()
{
	setValid(false);
	m_position.set(0,0,0);
	m_color.set(0,0,0);
	m_normal.set(0,0,0);
	m_uv.set(0.0,0.0);
}

void vxCollision::setValid(bool v)
{
	m_valid = v;
}

bool vxCollision::isValid() const
{
	return m_valid;
}

void vxCollision::setPosition(scalar x, scalar y, scalar z)
{
	m_position.set(x,y,z);
}

v3s vxCollision::position() const
{
	return m_position;
}

void vxCollision::setColor(scalar r, scalar g, scalar b)
{
	m_color.set(r,g,b);
}

vxColor vxCollision::color() const
{
	return m_color;
}

void vxCollision::setNormal(const v3s &val)
{
	m_normal = val;
}

v3s vxCollision::normal() const
{
	return m_normal;
}

void vxCollision::setUV(const v2s&& uvVec)
{
	m_uv = uvVec; 
}

void vxCollision::setU(scalar u)
{
	m_uv.setX(u);
}

scalar vxCollision::u() const
{
	return m_uv.x();
}

void vxCollision::setV(scalar v)
{
	m_uv.setY(v);
}

scalar vxCollision::v() const
{
	return m_uv.y();
}

v2s vxCollision::uv() const
{
	return m_uv;
}

void vxCollision::setColor(const vxColor &val)
{
	m_color = val;
}

void vxCollision::setColor(const v3s &val)
{
	m_color = val;
}

void vxCollision::setPosition(const v3s &val)
{
	m_position = val;
}

}
