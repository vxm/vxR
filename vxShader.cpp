#include "vxShader.h"
#include <algorithm>


namespace vxStorage {

vxColor vxLambert::getColor(const vxCollision &collide) const
{
	double lumm = std::max(getLightLoop(collide), 0.0);

	vxColor col = m_map.getColor(collide);
	
	return col * lumm;
}

void vxShader::setLights(std::vector<vxPointLight> * lights)
{
	m_lights = lights;
}


}
