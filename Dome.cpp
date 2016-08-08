#include "Dome.h"
#include "MathUtils.h"

using namespace vxCore;

Dome::Dome()
{
}

Dome::Dome(vxImageHandle env)
	: m_image(env)
{
}

scalar Dome::radius() const
{
	return m_radius;
}

void Dome::setRadius(const scalar &radius)
{
	m_radius = radius;
}

scalar Dome::gain() const
{
	return m_image->gain();
}

void Dome::setGain(const scalar &gain)
{
	m_image->setGain(gain);
}

scalar Dome::gamma() const
{
	return m_image->gamma();
}

void Dome::setGamma(const scalar &gamma)
{
	m_image->setGamma(gamma);
}

bool Dome::throwRay(const Ray &) const
{
	return true;
}

int Dome::throwRay(const Ray &ray, Collision &collide) const
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

bool Dome::hasCollision(const Ray &) const
{
	return true;
}
