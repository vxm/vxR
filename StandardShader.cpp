#include "StandardShader.h"

using namespace vxCore;

StandardShader::StandardShader()
{
}

v3s StandardShader::getVector(const Collision &collide) const
{
	return collide.normal();
}

Color StandardShader::getColor(const Ray&, const Collision &collide) const
{
//	if(hasDiffuseMap())
//	{
		
//	}
	return collide.color();
}

Color StandardShader::getLightLoop(const Ray&, const Collision &collision) const
{
	return collision.color();
}

bool StandardShader::hasDiffuseMap() const
{
	return m_diffueseTexture == nullptr;
}
