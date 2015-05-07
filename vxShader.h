#ifndef _VXSHADERMC_
#define _VXSHADERMC_

#include "stdlib.h"
#include "time.h"
#include <memory>
#include <vector>

#include "vxObject.h"
#include "vxVector.h"
#include "vxCollision.h"
#include "vxLight.h"
#include "vxCirclesMap.h"

namespace vxStorage {

class vxLight;

class vxShader:public vxObject
{
protected:
public:

	using lightsRef = std::shared_ptr<std::vector<vxLight>>;
	std::vector<vxPointLight> *m_lights = nullptr;


	vxShader()
	{
		srand(time(NULL));
	}
	
	void setLightsRef(std::vector<vxPointLight> * lights)
	{
		m_lights = lights;
	}
	
	virtual vxColor getColor(const vxCollision &collide) const = 0;

	virtual double getLightLoop(const vxCollision &collision) const
	{
		//assert(m_lights);
		
		double acumLumm{0.0};

		for(auto it = std::begin(*m_lights); it!=std::end(*m_lights);++it)
		{
			acumLumm += it->luminance(collision);
		}
		
		return std::min(1.0, std::max(acumLumm, 0.0));
	}
	
	std::shared_ptr<vxLight> light() const;
	void setLights(std::vector<vxPointLight> *lights);
};

class vxLambert:public vxShader
{
	private:
	vxCirclesMap m_map;		
		
	public:
		vxLambert()
			:vxShader() 
		{
		}
		
		// vxShader interface
	virtual vxColor getColor(const vxCollision &collide) const override;
	
};

}
#endif