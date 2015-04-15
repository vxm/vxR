#include "vxShader.h"
#include <algorithm>

namespace vxStorage {

vxColor vxLambert::getColor(vxCollision &collide, double lumm)
{
	auto distanceToCenter = 
			vxVector2d(collide.getU()-.5,collide.getV()-.5).length();

	lumm = std::max(lumm, 0.0);

	if(distanceToCenter<.3)
	{
		collide.setColor(lumm*collide.getColor().getR(), 
						 lumm*collide.getColor().getG(), 
						 lumm*collide.getColor().getB());
	}
	else
	{
		if(distanceToCenter<.35)
		{
			collide.setColor(lumm*64, lumm*24, lumm*24);
		}
		else
		{
			constexpr const double margn = 0.03;
			if ( (collide.getU()<margn || collide.getU()>(1-margn)) 
				 || (collide.getV()<margn || collide.getV()>(1-margn)))
			{
				collide.setColor(lumm*22, lumm*23, lumm*29);
			}
			else
			{
				collide.setColor(lumm*255, lumm*255, lumm*155);
			}
		}
	}

/*	float intens = collide.getPosition().length();
	vxVector3d pp=collide.getPosition();
	collide.setColor(12*pp.getX(), 12*pp.getY(), 12*pp.getZ());*/

//collide.setColor(255,255,255);


	return collide.getColor();
}

}
