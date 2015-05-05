#include "vxShader.h"
#include <algorithm>


namespace vxStorage {

vxColor vxLambert::getColor(const vxCollision &collide) const
{
	double lumm = std::max(getLightLoop(collide), 0.0);

	vxColor col = m_map.getColor(collide);
	
	return col * lumm;
}
std::shared_ptr<vxLight> vxShader::light() const
{
	return m_light;
}

void vxShader::setLight(const std::shared_ptr<vxLight> &light)
{
	m_light = light;
}


}
