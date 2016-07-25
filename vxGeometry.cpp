#include "vxGeometry.h"

#define DRAW_BB 0

using namespace vxCore;

vxColor vxGeometry::baseColor() const
{
	return m_baseColor;
}

void vxGeometry::setBaseColor(const vxColor &baseColor)
{
	m_baseColor = baseColor;
}

vxShader *vxGeometry::shader() const
{
	return m_shader;
}

void vxGeometry::setShader(vxShader *shader)
{
	m_shader = shader;
}

vxGeometry::vxGeometry()
{
	m_bb = std::make_shared<vxBoundingBox>();
}

std::shared_ptr<vxBoundingBox> vxGeometry::boundingBox()
{
	return m_bb;
}

Matrix44 vxGeometry::transform() const
{
	return m_transform;
}

Matrix44& vxGeometry::transform()
{
	return m_transform;
}

void vxGeometry::setTransform(const Matrix44 &transform)
{
	m_transform = transform;
}

bool vxGeometry::throwRay(const vxRay &ray) const
{
	auto col = vxCollision();
	return throwRay(ray, col);
}

int vxGeometry::throwRay(const vxRay &ray, vxCollision &col) const
{
	if(m_bb->throwRay(ray, col))
	{
		col.setColor(m_baseColor);
		col.setValid(true);

		return 1;
	}
	return 0;
}

bool vxGeometry::hasCollision(const vxRay &ray) const
{
	vxCollision col;
	if(m_bb->throwRay(ray, col))
	{
		return true;
	}
	
	return false;
}

