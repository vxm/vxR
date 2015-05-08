#include "vxShader.h"
#include "MathUtils.h"
#include <algorithm>


namespace vxStorage {

vxColor vxLambert::getColor(const vxCollision &collide) const
{
	double lumm = getLightLoop(collide);
	
	

	return MathUtils::clamp(m_map.getColor(collide), 0.001, 0.55) * lumm;
}

void vxShader::setLights(std::vector<vxPointLight> * lights)
{
	m_lights = lights;
}


}
