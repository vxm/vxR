#include "vxStandardShader.h"

vxStandardShader::vxStandardShader()
{
}

vxVector3d vxStandardShader::getVector(const vxCollision &collide) const
{
	return collide.normal();
}

vxColor vxStandardShader::getColor(const vxRay&, const vxCollision &collide) const
{
//	if(hasDiffuseMap())
//	{
		
//	}
	return collide.color();
}

vxColor vxStandardShader::getLightLoop(const vxRay&, const vxCollision &collision) const
{
	return collision.color();
}

bool vxStandardShader::hasDiffuseMap() const
{
	return m_diffueseTexture == nullptr;
}
