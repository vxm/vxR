#include "vxDome.h"
#include "MathUtils.h"

using namespace vxCore;

vxDome::vxDome()
{
}

vxDome::vxDome(vxImageHandle env)
	: m_image(env)
{
}

scalar vxDome::radius() const
{
	return m_radius;
}

void vxDome::setRadius(const scalar &radius)
{
	m_radius = radius;
}

scalar vxDome::gain() const
{
	return m_image->gain();
}

void vxDome::setGain(const scalar &gain)
{
	m_image->setGain(gain);
}

scalar vxDome::gamma() const
{
	return m_image->gamma();
}

void vxDome::setGamma(const scalar &gamma)
{
	m_image->setGamma(gamma);
}

bool vxDome::throwRay(const vxRay &) const
{
	return true;
}

int vxDome::throwRay(const vxRay &ray, vxCollision &collide) const
{
	collide.setUV(MU::normalToCartesian(ray.direction()));
	auto environmentColor = m_image->compute(collide);
	collide.setPosition(ray.origin() + ray.direction() * m_radius);
	//TODO: this should be valid and maybe have an alpha 0.
	//collide.setValid(false);
	
	collide.setColor(environmentColor);
	collide.setAlphaValue(1.0);
	return 1;
}

bool vxDome::hasCollision(const vxRay &) const
{
	return true;
}
