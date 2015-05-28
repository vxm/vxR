#include "vxStandardShader.h"

vxStandardShader::vxStandardShader()
{
}

vxColor vxStandardShader::getColor(const vxCollision &collide) const
{
	if(hasDiffuseMap())
	{
		
	}
}

vxColor vxStandardShader::getLightLoop(const vxCollision &col) const
{
	return vxColor::black;
}

bool vxStandardShader::hasDiffuseMap() const
{
	return m_diffueseTexture == nullptr;
}
