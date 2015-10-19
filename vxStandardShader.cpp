#include "vxStandardShader.h"

vxStandardShader::vxStandardShader()
{
}

vxVector3d vxStandardShader::getVector(const vxCollision &collide) const
{
	return collide.normal();
}

vxColor vxStandardShader::getColor(const vxRay &ray, const vxCollision &collide) const
{
//	if(hasDiffuseMap())
//	{
		
//	}
	return collide.color();
}

vxColor vxStandardShader::getLightLoop(const vxRay &ray, const vxCollision &col) const
{
	return col.color();
}

bool vxStandardShader::hasDiffuseMap() const
{
	return m_diffueseTexture == nullptr;
}
