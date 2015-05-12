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
	double lumm = getLightLoop(collide);
	
	return MathUtils::clamp(m_map.getColor(collide), 0.001, 0.65) * lumm;
}


double vxCore::vxShader::getLightLoop(const vxCollision &collision) const
{
	//assert(m_lights);
	double acumLumm{0.0};
	const vxVector3d &cPnt = collision.position();
	
	for(auto light = std::begin(*m_lights); light!=std::end(*m_lights);++light)
	{
		auto l = light->get();
		const vxRayXYZ f = l->position() - cPnt;
		if (!m_scene->hasCollision(cPnt, f))
		{
			acumLumm += fabs(l->luminance(collision));
		}
	}
	
	return acumLumm;
}

void vxCore::vxShader::setScene(vxScene *scene)
{
	m_scene = scene;
}

void vxCore::vxShader::setLights(std::vector<std::shared_ptr<vxLight>> * lights)
{
	m_lights = lights;
}


