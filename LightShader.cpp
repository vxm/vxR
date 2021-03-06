#include "LightShader.h"

using namespace vxCore;

LightShader::LightShader()
{
	m_hasReflection = false;
	m_hasGI = false;
}

Color LightShader::getColor(const Ray &, const Collision &col) const
{
	return col.color();
}

Color LightShader::getLumminance(const Ray &, const Collision &col) const
{
	return col.color();
}

v3s LightShader::getVector(const Collision &) const { return v3s::constZ; }
