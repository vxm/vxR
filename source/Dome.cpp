#include "Dome.h"
#include "MathUtils.h"

#include <algorithm>

using namespace vxCore;

ImageHandle Dome::lightImage() const { return m_lightImage; }

void Dome::setLightImage(const ImageHandle &lightImage)
{
	m_lightImage = lightImage;
}

Dome::Dome(ImageHandle env) : m_image(std::move(env)) {}

scalar Dome::intensity() const { return m_intensity; }

void Dome::setIntensity(const scalar &intensity) { m_intensity = intensity; }

scalar Dome::lightClamp() const { return m_lightClamp; }

void Dome::setLightClamp(const scalar &lightClamp) { m_lightClamp = lightClamp; }

scalar Dome::radius() const { return m_radius; }

void Dome::setRadius(const scalar &radius) { m_radius = radius; }

scalar Dome::gain() const { return m_image->gain(); }

void Dome::setGain(const scalar &gain) { m_image->setGain(gain); }

scalar Dome::gamma() const { return m_image->gamma(); }

void Dome::setGamma(const scalar &gamma) { m_image->setGamma(gamma); }

bool Dome::throwRay(const Ray &) const { return true; }

int Dome::throwRay(const Ray &ray, Collision &collide) const
{
	collide.setUV(MU::normalToCartesian(ray.direction()));

	auto environmentColor = m_image->compute(collide);

	collide.setPosition(ray.origin() + ray.direction() * m_radius);

	collide.setColor(environmentColor);

	return 1;
}

int Dome::computeLight(const Ray &ray, Collision &collide) const
{
	collide.setUV(MU::normalToCartesian(ray.direction()));

	auto environmentColor = m_lightImage->compute(collide) * m_intensity;

	if (m_lightClamp > 0.0)
	{
		const auto peak =
			std::max(environmentColor.r(),
					 std::max(environmentColor.g(), environmentColor.b()));
		if (peak > m_lightClamp)
		{
			// scale so the hue of the sky stays the same.
			const auto k = m_lightClamp / peak;
			environmentColor = Color(environmentColor.r() * k,
									 environmentColor.g() * k,
									 environmentColor.b() * k, environmentColor.alpha());
		}
	}

	collide.setPosition(ray.origin() + ray.direction() * m_radius);

	collide.setColor(environmentColor);

	collide.setValid(false);

	return 1;
}

bool Dome::hasCollision(const Ray &) const { return true; }
