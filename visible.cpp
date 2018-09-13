#include "visible.h"

using namespace vxCore;

#define DRAW_BB 0

std::shared_ptr<Shader> Visible::shader() const
{
	return m_shader;
}

void Visible::setShader(std::shared_ptr<Shader> shader)
{
	m_shader = shader;
}

VisibleType Visible::type() const
{
    return m_type;
}

void Visible::setType(const VisibleType &type)
{
    m_type = type;
}

Visible::Visible()
{
    m_bb = std::make_shared<BoundingBox>();
}

using namespace vxCore;

Color Visible::color() const
{
    return m_color;
}

void Visible::setColor(const Color &baseColor)
{
	m_color = baseColor;
}

int Visible::testBoundingBox(const Ray &ray, Collision &collide) const
{
	if(!m_bb->throwRay(ray, collide))
	{
		collide.setValid(false);
		return 0;
	}
	
	collide.setValid(true);
	return 1;
}

Visible::~Visible() {}

std::shared_ptr<BoundingBox> Visible::boundingBox()
{
	return m_bb;
}

Matrix44 Visible::transform() const
{
	return m_transform;
}

Matrix44& Visible::transform()
{
	return m_transform;
}

void Visible::setTransform(const Matrix44 &transform)
{
	m_transform = transform;
}

bool Visible::throwRay(const Ray &ray) const
{
	auto col = Collision();
	return throwRay(ray, col);
}

int Visible::throwRay(const Ray &ray, Collision &col) const
{
	if(m_bb->throwRay(ray, col))
	{
		col.setColor(m_color);
		col.setValid(true);

		return 1;
	}
	return 0;
}

bool Visible::hasCollision(const Ray &ray) const
{
	Collision col;
	if(m_bb->throwRay(ray, col))
	{
		return true;
	}
	
	return false;
}

