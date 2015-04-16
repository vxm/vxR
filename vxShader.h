#ifndef _VXSHADERMC_
#define _VXSHADERMC_

#include "stdlib.h"
#include "time.h"
#include <memory.h>


#include <vxObject.h>
#include <vxVector.h>
#include <vxCollision.h>
//#include <vxScene.h>

namespace vxStorage {

class vxScene;

class vxShader:public vxObject
{
protected:

	//std::shared_ptr<vxScene> m_scene = {nullptr};

public:

	vxShader()
	{
		srand(time(NULL));
	}
	
	virtual vxColor getColor(vxCollision &collide) const
	{
		return vxColor(0.0, 0.0, 0.0);
	}

	double getRand()
	{
		return (rand()/(double)RAND_MAX);
	}

	double getBoolRand()
	{
		return getRand()<.5;
	}
	
	double getLumm() const
	{
		return 0.9;
	}
};

class vxLambert:public vxShader
{
private:


public:
	vxLambert()
	:vxShader() 
	{
	}

	virtual vxColor getColor( vxCollision &collide) const override;

};

}
#endif