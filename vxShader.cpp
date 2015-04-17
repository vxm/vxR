#include "vxShader.h"
#include <algorithm>

namespace vxStorage {

vxColor vxLambert::getColor(vxCollision &collide) const
{
	auto distanceToCenter = 
			vxVector2d(collide.getU()-.5,collide.getV()-.5).length();

	double lumm = std::max(getLightLoop(collide), 0.0);

	if(distanceToCenter<.2)
	{
		collide.setColor(lumm*collide.getColor().getR(), 
						 lumm*collide.getColor().getG(), 
						 lumm*collide.getColor().getB());
	}
	else
	{
		if(distanceToCenter<.22)
		{
			collide.setColor(lumm*114, lumm*64, lumm*84);
		}
		else
		{
			constexpr const double margn = 0.03;
			if ( (collide.getU()<margn || collide.getU()>(1.-margn)) 
				 || (collide.getV()<margn || collide.getV()>(1.-margn)))
			{
				collide.setColor(lumm*22, lumm*23, lumm*229);
			}
			else
			{
				collide.setColor(lumm*255, lumm*255, lumm*155);
			}
		}
	}

/*	double intens = collide.getPosition().length();
	vxVector3d pp=collide.getPosition();
	collide.setColor(12*pp.getX(), 12*pp.getY(), 12*pp.getZ());*/

//collide.setColor(255,255,255);


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
