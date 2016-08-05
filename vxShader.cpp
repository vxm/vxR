#include "vxShader.h"

#include <algorithm>
#include "MathUtils.h"

using namespace vxCore;

vxLambert::vxLambert()
	:vxShader() 
{
}

vxColor vxShader::getDiffuseColor() const
{
	return m_diffuseColor;
}

void vxShader::setDiffuseColor(const vxColor &diffuseColor)
{
	m_diffuseColor = diffuseColor;
}

scalar vxShader::getDiffuseCoeficent() const
{
	return m_diffuseCoeficent;
}

void vxShader::setDiffuseCoeficent(const scalar &diffuseCoeficent)
{
	m_diffuseCoeficent = diffuseCoeficent;
}

unsigned int vxShader::getGiSamples() const
{
	return m_giSamples;
}

void vxShader::setGiSamples(unsigned int giSamples)
{
	m_giSamples = giSamples;
}

scalar vxShader::getGiCoeficent() const
{
	return m_giCoeficent;
}

void vxShader::setGiCoeficent(const scalar &giCoeficent)
{
	m_giCoeficent = giCoeficent;
}

vxColor vxShader::getGiColorMultiplier() const
{
	return m_giColorMultiplier;
}

void vxShader::setGiColorMultiplier(const vxColor &giColorMultiplier)
{
	m_giColorMultiplier = giColorMultiplier;
}

unsigned int vxShader::getReflectionSamples() const
{
	return m_reflectionSamples;
}

void vxShader::setReflectionSamples(unsigned int reflectionSamples)
{
	m_reflectionSamples = reflectionSamples;
}

scalar vxShader::getReflectionRadius() const
{
	return m_reflectionRadius;
}

void vxShader::setReflectionRadius(const scalar &reflectionRadius)
{
	m_reflectionRadius = reflectionRadius;
}

scalar vxShader::getReflectionCoefficent() const
{
	return m_reflectionCoefficent;
}

void vxShader::setReflectionCoefficent(const scalar &reflectionCoefficent)
{
	m_reflectionCoefficent = reflectionCoefficent;
}

vxColor vxShader::getReflectionColorMultiplier() const
{
	return m_reflectionColorMultiplier;
}

void vxShader::setReflectionColorMultiplier(const vxColor &reflectionColorMultiplier)
{
	m_reflectionColorMultiplier = reflectionColorMultiplier;
}

unsigned int vxShader::getRefractionSamples() const
{
	return m_refractionSamples;
}

void vxShader::setRefractionSamples(unsigned int refractionSamples)
{
	m_refractionSamples = refractionSamples;
}

scalar vxShader::getRefractionRadius() const
{
	return m_refractionRadius;
}

void vxShader::setRefractionRadius(const scalar &refractionRadius)
{
	m_refractionRadius = refractionRadius;
}

scalar vxShader::getRefractionCoefficent() const
{
	return m_refractionCoefficent;
}

void vxShader::setRefractionCoefficent(const scalar &refractionCoefficent)
{
	m_refractionCoefficent = refractionCoefficent;
}

vxColor vxShader::getRefractionColorMultiplier() const
{
	return m_refractionColorMultiplier;
}

void vxShader::setRefractionColorMultiplier(const vxColor &refractionColorMultiplier)
{
	m_refractionColorMultiplier = refractionColorMultiplier;
}

unsigned int vxShader::getSscSamples() const
{
	return m_sscSamples;
}

void vxShader::setSscSamples(unsigned int sscSamples)
{
	m_sscSamples = sscSamples;
}

scalar vxShader::getSscRadius() const
{
	return m_sscRadius;
}

void vxShader::setSscRadius(const scalar &sscRadius)
{
	m_sscRadius = sscRadius;
}

scalar vxShader::getSscCoefficent() const
{
	return m_sscCoefficent;
}

void vxShader::setSscCoefficent(const scalar &sscCoefficent)
{
	m_sscCoefficent = sscCoefficent;
}

vxColor vxShader::getSscColorMultiplier() const
{
	return m_sscColorMultiplier;
}

void vxShader::setSscColorMultiplier(const vxColor &sscColorMultiplier)
{
	m_sscColorMultiplier = sscColorMultiplier;
}

vxCore::vxShader::vxShader()
{
}


vxColor vxLambert::getIlluminatedColor(const vxRay &ray, const vxCollision &collide) const
{
	auto lumm = getLightLoop(ray, collide);
	
	return getColor(ray, collide) * lumm;
}


vxColor vxLambert::getColor(const vxRay &, const vxCollision &collide) const
{
	vxColor ret;
	
	auto&& color = m_diffuse.compute(collide)*m_diffuseColor;
	ret = MU::remap(color, 0.0, 0.85);

	return ret;
}

v3s vxLambert::getVector(const vxCollision &collide) const
{
	const auto& c = m_diffuse.compute(collide);
	return v3s(c.r(), c.g(), c.b());
}


vxColor vxCore::vxShader::getLightLoop(const vxRay &ray, const vxCollision &collision) const
{
	//assert(m_lights);
	vxColor acumColor;

	for(const auto& light:*m_lights)
	{
		acumColor.add( light->acummulationLight(ray, collision) );
	}
	
	return acumColor;
}

void vxCore::vxShader::setScene(std::weak_ptr<vxScene> scene)
{
	m_scene = scene;
}

void vxCore::vxShader::setLights(std::vector<std::shared_ptr<vxLight>> * lights)
{
	m_lights = lights;
}


