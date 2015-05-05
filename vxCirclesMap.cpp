#include "vxCirclesMap.h"

using namespace vxStorage;

vxCirclesMap::vxCirclesMap()
{
}

vxColor vxCirclesMap::getColor(const vxCollision &collision) const
{
	vxColor col;
	auto distanceToCenter = 
			vxVector2d(collision.getU()-.5,collision.getV()-.5).length();

	if(distanceToCenter<.075)
	{
		col.set(2.0/255.0, 2.0/255.0, 2.0/255.0);
	}
	else if(distanceToCenter<.3)
	{
		col.set(collision.getColor().getR(), 
						 collision.getColor().getG(), 
						 collision.getColor().getB());
	}
	else if(distanceToCenter<.31)
	{
		col.set(2.0/255.0, 2.0/255.0, 2.0/255.0);
	}
	else if(distanceToCenter<.4)
	{
		col.set(255.0/255.0, 255.0/255.0, 255.0/255.0);
	}
	else if(distanceToCenter<.41)
	{
		col.set(2.0/255.0, 2.0/255.0, 2.0/255.0);
	}	
	else
	{
		constexpr const double margn = 0.01;
		if ( (collision.getU()<margn || collision.getU()>(1.-margn)) 
			 || (collision.getV()<margn || collision.getV()>(1.-margn)))
		{
			col.set(2.0/255.0, 3.0/255.0, 29.0/255.0);
		}
		else
		{
			col.set(255.0/255.0, 255.0/255.0, 155.0/255.0);
		}
	}

	return col;
}

