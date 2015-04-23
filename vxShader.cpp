#include "vxShader.h"
#include <algorithm>

namespace vxStorage {

vxColor vxLambert::getColor(vxCollision &collide) const
{
	auto distanceToCenter = 
			vxVector2d(collide.getU()-.5,collide.getV()-.5).length();

	double lumm = std::max(getLightLoop(collide), 0.0);

	if(distanceToCenter<.075)
	{
		collide.setColor(0, 0, 0);
	}
	else if(distanceToCenter<.3)
	{
		collide.setColor(lumm*collide.getColor().getR(), 
						 lumm*collide.getColor().getG(), 
						 lumm*collide.getColor().getB());
	}
	else if(distanceToCenter<.32)
	{
		collide.setColor(lumm*2.0, lumm*4.0, lumm*6.0);
	}
	else if(distanceToCenter<.4)
	{
		collide.setColor(lumm*255.0, lumm*255.0, lumm*255.0);
	}
	else
	{
		constexpr const double margn = 0.01;
		if ( (collide.getU()<margn || collide.getU()>(1.-margn)) 
			 || (collide.getV()<margn || collide.getV()>(1.-margn)))
		{
			collide.setColor(lumm*2.0, lumm*3.0, lumm*29.0);
		}
		else
		{
			collide.setColor(lumm*255.0, lumm*255.0, lumm*155.0);
		}
	}

	return collide.getColor();
}
std::shared_ptr<vxLight> vxShader::light() const
{
	return m_light;
}

void vxShader::setLight(const std::shared_ptr<vxLight> &light)
{
	m_light = light;
}


}
