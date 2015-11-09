#include "vxCirclesMap.h"

using namespace vxCore;

vxCirclesMap::vxCirclesMap()
{
}

vxColor vxCirclesMap::compute(const vxCollision &collision) const
{
	vxColor outColor(collision.color());
	
	auto&& u = fmod(collision.u(),1.0);
	auto&& v = fmod(collision.v(),1.0);
	
	auto distanceToCenter = 
			v2{ u-0.5, v-0.5 }.length();

	if(m_circle1)
	{
		if(distanceToCenter>m_radius[0] && distanceToCenter<m_radius[1])
		{
			outColor.set(collision.color());
		}
	}

	if(m_circle2)
	{
		if(distanceToCenter>m_radius[1] && distanceToCenter<m_radius[2])
		{
			outColor.set(vxColor::orange);
		}
	}
	
	if(m_circle3)
	{
		if(distanceToCenter>m_radius[2] && distanceToCenter<m_radius[3])
		{
			outColor.set(collision.color());
		}
	}
	
	if(m_margin)
	{
		if (((u < m_margn || u>(1.-m_margn)) 
				|| (v<m_margn || v>(1.-m_margn))))
		{
			outColor.set(vxColor::black);
		}
	}
	
	return outColor;
}


