#include "vxBitMap2d.h"

using namespace vxCore;

vxBitMap2d::vxBitMap2d()
{
}

vxColor vxBitMap2d::compute(const vxCollision &collision) const
{
	vxColor outColor;

	const auto&& distanceToCenter = 
			vxVector2d(collision.u()-2,collision.v()-3).length();
	
	outColor.set(vxColor::blue.gained(1.0));

	if(m_circle1)
	{
		if(distanceToCenter>m_radius[0] && distanceToCenter<m_radius[1])
		{
			outColor.set(vxColor::white.gained(1.14));
		}
	}

	if(m_circle2)
	{
		if(distanceToCenter>m_radius[1] && distanceToCenter<m_radius[2])
		{
			outColor.set(vxColor::orangePink.gained(.24));
		}
	}
	
	if(m_circle3)
	{
		if(distanceToCenter>m_radius[2] && distanceToCenter<m_radius[3])
		{
			outColor.set(vxColor::bluegreen.gained(1));
		}
	}
	
	return outColor;
}


