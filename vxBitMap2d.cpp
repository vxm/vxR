#include "vxBitMap2d.h"

using namespace vxCore;

vxBitMap2d::vxBitMap2d()
{
}

vxColor vxBitMap2d::compute(const vxCollision &collision) const
{
	vxColor outColor;

	auto distanceToCenter = 
			vxVector2d(collision.u()-.5,collision.v()-.5).length();

	outColor.set(vxColor::black);

	if(m_circle1)
	{
		if(distanceToCenter>m_radius[0] && distanceToCenter<m_radius[1])
		{
			outColor.set(vxColor::purple);
		}
	}

	if(m_circle2)
	{
		if(distanceToCenter>m_radius[1] && distanceToCenter<m_radius[2])
		{
			outColor.set(vxColor::black);
		}
	}
	
	if(m_circle3)
	{
		if(distanceToCenter>m_radius[2] && distanceToCenter<m_radius[3])
		{
			outColor.set(vxColor::blue);
		}
	}
	
	if(m_margin)
	{
		if (((collision.u()<m_margn || collision.u()>(1.-m_margn)) 
				|| (collision.v()<m_margn || collision.v()>(1.-m_margn))))
		{
			outColor.set(vxColor::black);
		}
	}
	
	return outColor;
}


