#include "Shader.h"

#include "Geometry.h"
#include "MathUtils.h"
#include <algorithm>

using namespace vxCore;

bool Shader::hasReflection() const
{
	return m_hasReflection && m_reflectionCoefficent != 0.0;
}

void Shader::setHasReflection(bool hasReflection)
{
	m_hasReflection = hasReflection;
}

bool Shader::hasGI() const { return m_hasGI; }

void Shader::setHasGI(bool hasGI) { m_hasGI = hasGI; }

vxCore::Shader::Shader() {}

Color Shader::getDiffuseColor() const { return m_diffuseColor; }

void Shader::setDiffuseColor(const Color &diffuseColor)
{
	m_diffuseColor = diffuseColor;
}

scalar Shader::getDiffuseCoeficent() const { return m_diffuseCoeficent; }

void Shader::setDiffuseCoeficent(const scalar &diffuseCoeficent)
{
	m_diffuseCoeficent = diffuseCoeficent;
}

scalar Shader::getGiCoeficent() const { return m_giCoeficent; }

void Shader::setGiCoeficent(const scalar &giCoeficent)
{
	m_giCoeficent = giCoeficent;
}

Color Shader::getGiColorMultiplier() const { return m_giColorMultiplier; }

void Shader::setGiColorMultiplier(const Color &giColorMultiplier)
{
	m_giColorMultiplier = giColorMultiplier;
}

unsigned int Shader::getRayDepth() const { return m_rayDepth; }

void Shader::setRayDepth(unsigned int rayDepth) { m_rayDepth = rayDepth; }

scalar Shader::getReflectionRadius() const { return m_reflectionRadius; }

void Shader::setReflectionRadius(const scalar &reflectionRadius)
{
	m_reflectionRadius = reflectionRadius;
}

scalar Shader::getReflectionCoefficent() const
{
	return m_reflectionCoefficent;
}

void Shader::setReflectionCoefficent(const scalar &reflectionCoefficent)
{
	m_reflectionCoefficent = reflectionCoefficent;
}

Color Shader::getReflectionColorMultiplier() const
{
	return m_reflectionColorMultiplier;
}

void Shader::setReflectionColorMultiplier(
	const Color &reflectionColorMultiplier)
{
	m_reflectionColorMultiplier = reflectionColorMultiplier;
}

unsigned int Shader::getRefractionSamples() const
{
	return m_refractionSamples;
}

void Shader::setRefractionSamples(unsigned int refractionSamples)
{
	m_refractionSamples = refractionSamples;
}

scalar Shader::getRefractionRadius() const { return m_refractionRadius; }

void Shader::setRefractionRadius(const scalar &refractionRadius)
{
	m_refractionRadius = refractionRadius;
}

scalar Shader::getRefractionCoefficent() const
{
	return m_refractionCoefficent;
}

void Shader::setRefractionCoefficent(const scalar &refractionCoefficent)
{
	m_refractionCoefficent = refractionCoefficent;
}

Color Shader::getRefractionColorMultiplier() const
{
	return m_refractionColorMultiplier;
}

void Shader::setRefractionColorMultiplier(
	const Color &refractionColorMultiplier)
{
	m_refractionColorMultiplier = refractionColorMultiplier;
}

unsigned int Shader::getSscSamples() const { return m_sscSamples; }

void Shader::setSscSamples(unsigned int sscSamples)
{
	m_sscSamples = sscSamples;
}

scalar Shader::getSscRadius() const { return m_sscRadius; }

void Shader::setSscRadius(const scalar &sscRadius) { m_sscRadius = sscRadius; }

scalar Shader::getSscCoefficent() const { return m_sscCoefficent; }

void Shader::setSscCoefficent(const scalar &sscCoefficent)
{
	m_sscCoefficent = sscCoefficent;
}

Color Shader::getSscColorMultiplier() const { return m_sscColorMultiplier; }

void Shader::setSscColorMultiplier(const Color &sscColorMultiplier)
{
	m_sscColorMultiplier = sscColorMultiplier;
}

Color vxCore::Shader::getLightLoop(const Ray &ray,
								   const Collision &collision) const
{
	// assert(m_lights);
	Color acumColor;

	for (const auto &light : *m_lights)
	{
		acumColor.add(light->acummulationLight(ray, collision));
	}

	return acumColor;
}

void vxCore::Shader::setScene(Scene *scene) { m_scene = scene; }

void vxCore::Shader::setLights(std::vector<std::shared_ptr<Light>> *lights)
{
	m_lights = lights;
}

////////// Lambert

Lambert::Lambert() : Shader() {}

Color Lambert::getLumminance(const Ray &ray, const Collision &collide) const
{
	auto lumm = getLightLoop(ray, collide);

	return lumm;
}

Color Lambert::getColor(const Ray &, const Collision &collide) const
{
	Color ret = Color::zero;

	if (collide.m_geo)
	{
		auto color = collide.color();
		// collide.m_geo->color(); // m_diffuse.compute(collide)*m_diffuseColor;

		ret = MU::remap(color, 0.0, 0.98);
	}
	else
	{
		auto &&color =
			collide.color(); // m_diffuse.compute(collide)*m_diffuseColor;
		ret = MU::remap(color, 0.0, 0.85);
	}

	return ret;
}

v3s Lambert::getVector(const Collision &) const { return v3s(0.0, 0.0, 0.0); }
