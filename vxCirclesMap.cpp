#include "vxCirclesMap.h"

using namespace vxCore;

vxCirclesMap::vxCirclesMap()
{
}

vxColor vxCirclesMap::compute(const vxCollision &collision) const
{
	vxColor outColor;

	auto distanceToCenter = 
			vxVector2d(collision.u()-.5,collision.v()-.5).length();

	outColor.set(vxColor::white);

	if(m_circle1)
	{
		if(distanceToCenter>m_radius[0] && distanceToCenter<m_radius[1])
		{
			outColor.set(vxColor::bluegreen);
		}
	}

	if(m_circle2)
	{
		if(distanceToCenter>m_radius[1] && distanceToCenter<m_radius[2])
		{
			outColor.set(vxColor::white);
		}
	}
	
	if(m_circle3)
	{
		if(distanceToCenter>m_radius[2] && distanceToCenter<m_radius[3])
		{
			outColor.set(vxColor::greenishBlue);
		}
	}
	
	if(m_margin)
	{
		if (((collision.u()<m_margn || collision.u()>(1.-m_margn)) 
				|| (collision.v()<m_margn || collision.v()>(1.-m_margn))))
		{
			outColor.set(vxColor::blue);
		}
	}
	
	return outColor;
}


