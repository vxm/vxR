#include "Shader.h"

#include <algorithm>
#include "MathUtils.h"
#include "Geometry.h"
using namespace vxCore;

Lambert::Lambert()
	:Shader() 
{
}

Color Shader::getDiffuseColor() const
{
	return m_diffuseColor;
}

void Shader::setDiffuseColor(const Color &diffuseColor)
{
	m_diffuseColor = diffuseColor;
}

scalar Shader::getDiffuseCoeficent() const
{
	return m_diffuseCoeficent;
}

void Shader::setDiffuseCoeficent(const scalar &diffuseCoeficent)
{
	m_diffuseCoeficent = diffuseCoeficent;
}

unsigned int Shader::getGiSamples() const
{
	return m_giSamples;
}

void Shader::setGiSamples(unsigned int giSamples)
{
	m_giSamples = giSamples;
}

scalar Shader::getGiCoeficent() const
{
	return m_giCoeficent;
}

void Shader::setGiCoeficent(const scalar &giCoeficent)
{
	m_giCoeficent = giCoeficent;
}

Color Shader::getGiColorMultiplier() const
{
	return m_giColorMultiplier;
}

void Shader::setGiColorMultiplier(const Color &giColorMultiplier)
{
	m_giColorMultiplier = giColorMultiplier;
}

unsigned int Shader::getReflectionSamples() const
{
	return m_reflectionSamples;
}

void Shader::setReflectionSamples(unsigned int reflectionSamples)
{
	m_reflectionSamples = reflectionSamples;
}

scalar Shader::getReflectionRadius() const
{
	return m_reflectionRadius;
}

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

void Shader::setReflectionColorMultiplier(const Color &reflectionColorMultiplier)
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

scalar Shader::getRefractionRadius() const
{
	return m_refractionRadius;
}

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

void Shader::setRefractionColorMultiplier(const Color &refractionColorMultiplier)
{
	m_refractionColorMultiplier = refractionColorMultiplier;
}

unsigned int Shader::getSscSamples() const
{
	return m_sscSamples;
}

void Shader::setSscSamples(unsigned int sscSamples)
{
	m_sscSamples = sscSamples;
}

scalar Shader::getSscRadius() const
{
	return m_sscRadius;
}

void Shader::setSscRadius(const scalar &sscRadius)
{
	m_sscRadius = sscRadius;
}

scalar Shader::getSscCoefficent() const
{
	return m_sscCoefficent;
}

void Shader::setSscCoefficent(const scalar &sscCoefficent)
{
	m_sscCoefficent = sscCoefficent;
}

Color Shader::getSscColorMultiplier() const
{
	return m_sscColorMultiplier;
}

void Shader::setSscColorMultiplier(const Color &sscColorMultiplier)
{
	m_sscColorMultiplier = sscColorMultiplier;
}

vxCore::Shader::Shader()
{
}


Color Lambert::getIlluminatedColor(const Ray &ray, const Collision &collide) const
{
	auto lumm = getLightLoop(ray, collide);
	
	return getColor(ray, collide) * lumm;
}


Color Lambert::getColor(const Ray &, const Collision &collide) const
{
	Color ret = Color::zero;
	
	auto&& color = collide.m_geo->baseColor();//m_diffuse.compute(collide)*m_diffuseColor;
	ret = MU::remap(color, 0.0, 0.85);

	return ret;
}

v3s Lambert::getVector(const Collision &collide) const
{
	const auto& c = m_diffuse.compute(collide);
	return v3s(c.r(), c.g(), c.b());
}


Color vxCore::Shader::getLightLoop(const Ray &ray, const Collision &collision) const
{
	//assert(m_lights);
	Color acumColor;

	for(auto&& light:*m_lights)
	{
		acumColor.add( light->acummulationLight(ray, collision) );
	}
	
	return acumColor;
}

void vxCore::Shader::setScene(std::weak_ptr<Scene> scene)
{
	m_scene = scene;
}

void vxCore::Shader::setLights(std::vector<std::shared_ptr<Light>> * lights)
{
	m_lights = lights;
}


