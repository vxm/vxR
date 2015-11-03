#ifndef _VXSHADERMC_
#define _VXSHADERMC_

#include "stdlib.h"
#include "time.h"
#include <memory>
#include <vector>

#include "vxVector.h"
#include "vxCollision.h"
#include "vxLight.h"
#include "vxCirclesMap.h"
#include "vxScene.h"

namespace vxCore {

class vxLight;
class vxScene;

class vxShader
{
protected:

	std::vector<std::shared_ptr<vxLight>> *m_lights = nullptr;
	std::weak_ptr<vxScene>					m_scene;

public:
	vxShader();
	using lightsRef = std::shared_ptr<std::vector<vxLight>>;
	virtual vxColor getColor(const vxRay &ray, const vxCollision &collide) const = 0;
	virtual v3 getVector(const vxCollision &collide) const = 0;
	virtual vxColor getLightLoop(const vxRay &ray, const vxCollision &collision) const;
	void setScene(std::weak_ptr<vxScene> scene);
	std::shared_ptr<vxLight> light() const;
	void setLights(std::vector<std::shared_ptr<vxLight>> *lights);

	vxCirclesMap m_diffuse;	
};

class vxLambert:public vxShader
{
	private:
	
	public:
		vxLambert()
			:vxShader() 
		{
		}
		
	// vxShader interface
	virtual vxColor getColor(const vxRay &ray, const vxCollision &collide) const override;
	virtual v3 getVector(const vxCollision &collide) const override;
	
};

}
#endif
