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
			if ( (collide.getU()<.01 || collide.getU()>.99) 
				 || (collide.getV()<.01 || collide.getV()>.99))
			{
				double incanden=1/1.7;
				double sum=(lumm*incanden)+(1-incanden);
				collide.setColor(255*sum, 255*sum, 255*sum);
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
