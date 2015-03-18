#ifndef _VXSCENEMC_
#define _VXSCENEMC_

#include "vxObject.h"
#include "vxLight.h"
#include <memory>

namespace vxStorage {

class vxScene:public vxObject
{
protected:

	bool m_defaultLight = {true};
	int m_numLights = {1};
	std::shared_ptr<vxLight> m_light = {nullptr};
	
public:

	vxScene()
	{
		m_light.reset(new vxPointLight);
		m_light->setPosition( 0 , 15 , 0 );
	}
	
	~vxScene()
	{
	}

	double getLight(vxCollision &collide)
	{		
		return m_light->luminance(collide);
	}

};

}
#endif
