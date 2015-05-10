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
#include "vxScene.h"

namespace vxCore {

class vxLight;
class vxScene;

class vxShader:public vxObject
{
protected:

	std::vector<vxPointLight> *m_lights = nullptr;
	vxScene	*m_scene = {nullptr};

public:
	vxShader();

	using lightsRef = std::shared_ptr<std::vector<vxLight>>;

	
	void setLightsRef(std::vector<vxPointLight> * lights);
	
	virtual vxColor getColor(const vxCollision &collide) const = 0;

	virtual double getLightLoop(const vxCollision &collision) const;
	
	void setScene(vxScene *scene);
	
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