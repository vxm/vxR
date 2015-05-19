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
		col.set(vxColor::black);
	}
	else if(distanceToCenter<.3)
	{
		col.set(collision.color().r(), 
			collision.color().g(), 
			collision.color().b());
	}
	else if(distanceToCenter<.31)
	{
		col.set(vxColor::black);
	}
	else if(distanceToCenter<.4)
	{
		col.set(vxColor::white);
	}
	else if(distanceToCenter<.41)
	{
		col.set(vxColor::black);
	}	
	else
	{
		constexpr const double margn = 0.01;
		if ( (collision.u()<margn || collision.u()>(1.-margn)) 
			 || (collision.v()<margn || collision.v()>(1.-margn)))
		{
			col.set(vxColor::black);
		}
		else
		{
			col.set(vxColor::white);
		}
	}

	return col;
}


