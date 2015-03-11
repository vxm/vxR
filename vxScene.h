#ifndef _VXSCENEMC_
#define _VXSCENEMC_

#include "vxObject.h"
#include "vxLight.h"

namespace vxStorage {

class vxScene:public vxObject
{
protected:

	bool m_defaultLight;
	int m_lights;
	vxLight *m_light;

public:

	vxScene()
	{
		m_light = new vxPointLight;
		//light->setPosition( 3 , 15 , 5 );
		m_light->setPosition( 0 , 15 , 0 );
		m_lights=1;
		m_defaultLight=true;
	}
	
	~vxScene()
	{
		delete m_light;
	}

	double getLight(vxCollision &collide)
	{		
		return m_light->luminance(collide);
	}

};
}
#endif
