#include "vxShader.h"

#include <algorithm>
#include "MathUtils.h"

using namespace vxCore;

vxCore::vxShader::vxShader()
{
	srand(time(NULL));
}


vxColor vxLambert::getIlluminatedColor(const vxRay &ray, const vxCollision &collide) const
{
	auto lumm = getLightLoop(ray, collide);

	return getColor(ray, collide) * lumm;
}

vxColor vxLambert::getColor(const vxRay &, const vxCollision &collide) const
{
	vxColor ret;
	
	auto&& color = m_diffuse.compute(collide);
	ret = MU::remap(color, 0.0, 0.85);

	return ret;
}

v3 vxLambert::getVector(const vxCollision &collide) const
{
	const auto& c = m_diffuse.compute(collide);
	return v3(c.r(), c.g(), c.b());
}


vxColor vxCore::vxShader::getLightLoop(const vxRay &ray, const vxCollision &collision) const
{
	//assert(m_lights);
	vxColor acumColor;

	for(const auto& light:*m_lights)
	{
		acumColor.add( light->acummulationLight(ray, collision) );
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


