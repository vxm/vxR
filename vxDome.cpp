#include "vxDome.h"

vxDome::vxDome()
{
}

vxDome::vxDome(std::shared_ptr<vxBitMap2d> env)
	: m_image(env)
{
}

bool vxDome::throwRay(const vxRay &ray) const
{
	return true;
}

int vxDome::throwRay(const vxRay &ray, vxCollision &collide) const
{
	collide.setUV(MathUtils::normalToCartesian(ray.direction()));
	auto environmentColor = m_image->compute(collide);

	//TODO: this should be valid and maybe have an alpha 0.
	collide.setValid(false);
	collide.setColor(environmentColor);
	return 1;
}

bool vxDome::hasCollision(const vxRay &ray) const
{
	return true;
}