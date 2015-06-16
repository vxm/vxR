#include "vxCollision.h"
namespace vxCore{

void vxCollision::initialize()
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

void vxCollision::setPosition(double x, double y, double z)
{
	m_position.set(x,y,z);;
}

vxVector3d vxCollision::position() const
{
	return m_position;
}

void vxCollision::setColor(double r, double g, double b)
{
	m_color.set(r,g,b);
}

vxColor vxCollision::color() const
{
	return m_color;
}

void vxCollision::setNormal(const vxVector3d &val)
{
	m_normal = val;
}

vxVector3d vxCollision::normal() const
{
	return m_normal;
}

void vxCollision::setUV(const vxVector2d &uvVec)
{
	m_uv = uvVec; 
}

void vxCollision::setU(double u)
{
	m_uv.setX(u);
}

double vxCollision::u() const
{
	return m_uv.x();
}

void vxCollision::setV(double v)
{
	m_uv.setY(v);
}

double vxCollision::v() const
{
	return m_uv.y();
}

void vxCollision::setColor(const vxColor &val)
{
	m_color = val;
}

void vxCollision::setColor(const vxVector3d &val)
{
	m_color = val;
}

void vxCollision::setPosition(const vxVector3d &val)
{
	m_position = val;
}

}
