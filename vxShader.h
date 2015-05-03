#ifndef _VXSHADERMC_
#define _VXSHADERMC_

#include "stdlib.h"
#include "time.h"
#include <memory>
#include <vxObject.h>
#include <vxVector.h>
#include <vxCollision.h>
#include <vxLight.h>

namespace vxStorage {

class vxLight;

class vxShader:public vxObject
{
protected:

	std::shared_ptr<vxLight> m_light = {nullptr};

public:

	vxShader()
	{
		srand(time(NULL));
	}
	
	virtual vxColor getColor(vxCollision &collide) const
	{
		return collide.getColor();
	}

	double getRand()
	{
		return (rand()/(double)RAND_MAX);
	}

	double getBoolRand()
	{
		return getRand()<.5;
	}
	
	virtual double getLightLoop(const vxCollision &col) const
	{
		return light()->luminance(col);
	}
	
	std::shared_ptr<vxLight> light() const;
	void setLight(const std::shared_ptr<vxLight> &light);
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
	virtual vxColor getColor(vxCollision &collide) const override;
	
};

}
#endif