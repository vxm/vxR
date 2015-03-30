#include "vxShader.h"
#include <algorithm>

namespace vxStorage {

vxColor vxLambert::getColor(vxCollision &collide, double lumm)
{
/*
	double intens = collide.getPosition().angle(vxVector3d(0,0,-1));
	if (intens<0) intens=0;
	if (intens>255) intens=255;
	collide.setColor(255*intens, 255*intens, 255*intens);
*/
	auto distanciaCentro = 
			vxVector2d(collide.getU()-.5,collide.getV()-.5).length();

	/*if(distanciaCentro<.4)
	{
		collide.setNormal(collide.getNormal()+pow(distanciaCentro,2));
	}*/

	float intens = std::max(lumm, 0.0);

	if(distanciaCentro<.3)
	{
		collide.setColor((1-distanciaCentro)*intens*collide.getColor().getR(), 
						 (1-distanciaCentro)*intens*collide.getColor().getG(), 
						 (1-distanciaCentro)*intens*collide.getColor().getB());
	}
	else
	{
		if(distanciaCentro<.35)
		{
			collide.setColor(intens*64, intens*24, intens*24);
		}
		else
		{
			if ( (collide.getU()<.01 || collide.getU()>.99) 
				 || (collide.getV()<.01 || collide.getV()>.99))
			{
				double incanden=1/1.7;
				double sum=(intens*incanden)+(1-incanden);
				collide.setColor(255*sum, 255*sum, 255*sum);
			}
			else
			{
				collide.setColor(intens*255, intens*255, intens*155);
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
