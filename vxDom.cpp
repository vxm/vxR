#include "vxDom.h"

vxDom::vxDom()
{
}

vxDom::vxDom(std::shared_ptr<vxBitMap2d> env)
	: m_image(env)
{
}

bool vxDom::throwRay(const vxRay &ray) const
{
	return true;
}

int vxDom::throwRay(const vxRay &ray, vxCollision &collide) const
{
	collide.setUV(MathUtils::normalToCartesian(ray.direction()));
	auto environmentColor = m_image->compute(collide);
	
	collide.setValid(false);
	collide.setColor( environmentColor );
	return 1;
}

bool vxDom::hasCollision(const vxRay &ray) const
{
	return true;
}
