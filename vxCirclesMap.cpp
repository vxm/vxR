#include "vxCirclesMap.h"

using namespace vxCore;

vxCirclesMap::vxCirclesMap()
{
}

vxColor vxCirclesMap::getColor(const vxCollision &collision) const
{
	vxColor col;
	auto distanceToCenter = 
			vxVector2d(collision.u()-.5,collision.v()-.5).length();

	if(distanceToCenter<.015)
	{
		col.set(0.008, 0.008, 0.008);
	}
	else if(distanceToCenter<.3)
	{
		col.set(collision.color().getR(), 
						 collision.color().getG(), 
						 collision.color().getB());
	}
	else if(distanceToCenter<.31)
	{
		col.set(0.008, 0.008, 0.008);
	}
	else if(distanceToCenter<.4)
	{
		col.set(1.0, 1.0, 0.8);
	}
	else if(distanceToCenter<.41)
	{
		col.set(0.008, 0.008, 0.008);
	}	
	else
	{
		constexpr const double margn = 0.01;
		if ( (collision.u()<margn || collision.u()>(1.-margn)) 
			 || (collision.v()<margn || collision.v()>(1.-margn)))
		{
			col.set(0.008, 0.018, 0.13);
		}
		else
		{
			col.set(0.6, 0.62, 0.35);
		}
	}

	return col;
}


