#include "vxShader.h"

#include <algorithm>
#include "MathUtils.h"

using namespace vxCore;

vxCore::vxShader::vxShader()
{
	srand(time(NULL));
}


vxColor vxLambert::getColor(const vxCollision &collide) const
{
	auto lumm = getLightLoop(collide);
	
	return MathUtils::remap(m_diffuse.compute(collide), 0.002, 0.65) * lumm;
}


vxColor vxCore::vxShader::getLightLoop(const vxCollision &collision) const
{
	//assert(m_lights);
	vxColor acumColor;

	for(auto light = std::begin(*m_lights);	light!=std::end(*m_lights);	++light)
	{
		acumColor.add( light->get()->acummulationLight(collision) );
	}
	
	return acumColor;
}

void vxCore::vxShader::setScene(std::weak_ptr<vxScene> scene)
{
	m_scene = scene;
}

void vxCore::vxShader::setLights(std::vector<std::shared_ptr<vxLight>> * lights)
{
	m_lights = lights;
}


