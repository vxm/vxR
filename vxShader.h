#ifndef _VXSHADERMC_
#define _VXSHADERMC_

#include "stdlib.h"
#include "time.h"
#include <memory>
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

	std::shared_ptr<vxLight> m_light = {nullptr};
	vxCirclesMap m_map;
public:

	vxShader()
	{
		srand(time(NULL));
	}
	
	virtual vxColor getColor(const vxCollision &collide) const = 0;

	virtual double getLightLoop(const vxCollision &col) const
	{
		return std::max(light()->luminance(col), 0.0);
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
	virtual vxColor getColor(const vxCollision &collide) const override;
	
};

}
#endif
