#include "Geometry.h"

#define DRAW_BB 0

using namespace vxCore;

Color Geometry::baseColor() const
{
	return m_baseColor;
}

void Geometry::setBaseColor(const Color &baseColor)
{
	m_baseColor = baseColor;
}

Shader *Geometry::shader() const
{
	return m_shader;
}

void Geometry::setShader(Shader *shader)
{
	m_shader = shader;
}

int Geometry::testBoundingBox(const Ray &ray, Collision &collide) const
{
	if(!m_bb->throwRay(ray, collide))
	{
		collide.setValid(false);
		return 0;
	}
	
	collide.setValid(true);
	return 1;
}

Geometry::Geometry()
{
	m_bb = std::make_shared<BoundingBox>();
}

Geometry::~Geometry() {}

std::shared_ptr<BoundingBox> Geometry::boundingBox()
{
	return m_bb;
}

Matrix44 Geometry::transform() const
{
	return m_transform;
}

Matrix44& Geometry::transform()
{
	return m_transform;
}

void Geometry::setTransform(const Matrix44 &transform)
{
	m_transform = transform;
}

bool Geometry::throwRay(const Ray &ray) const
{
	auto col = Collision();
	return throwRay(ray, col);
}

int Geometry::throwRay(const Ray &ray, Collision &col) const
{
	if(m_bb->throwRay(ray, col))
	{
		col.setColor(m_baseColor);
		col.setValid(true);

		return 1;
	}
	return 0;
}

bool Geometry::hasCollision(const Ray &ray) const
{
	Collision col;
	if(m_bb->throwRay(ray, col))
	{
		return true;
	}
	
	return false;
}

