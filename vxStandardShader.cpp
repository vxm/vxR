#include "vxStandardShader.h"

vxStandardShader::vxStandardShader()
{
}

vxColor vxStandardShader::getColor(vxCollision &collide) const
{
	if(hasDiffuseMap())
	{
		
	}
}

double vxStandardShader::getLightLoop(const vxCollision &col) const
{
	
	return 0.0;
}

bool vxStandardShader::hasDiffuseMap() const
{
	return m_diffueseTexture == nullptr;
}
