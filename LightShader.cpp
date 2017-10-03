#include "LightShader.h"

using namespace vxCore;

LightShader::LightShader()
{
	m_hasReflection = false;
	m_hasGI= false;
}

Color LightShader::getColor(const Ray &, const Collision &) const
{
	return Color::white;
}

Color LightShader::getIlluminatedColor(const Ray &, const Collision &) const
{
	return Color::white;
}

v3s LightShader::getVector(const Collision &) const
{
	return v3s::constZ;
}
