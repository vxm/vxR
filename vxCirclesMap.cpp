#include "vxCirclesMap.h"

using namespace vxCore;

vxCirclesMap::vxCirclesMap()
{
}

vxColor vxCirclesMap::compute(const vxCollision &collision) const
{
	vxColor col;

	auto distanceToCenter = 
			vxVector2d(collision.u()-.5,collision.v()-.5).length();

	col.set(vxColor::white);

	if(m_circle1)
	{
		if(distanceToCenter<m_radius[1] && distanceToCenter>m_radius[0])
		{
			col.set(collision.color().r(), 
					collision.color().g(), 
					collision.color().b());
		}
	}

	if(m_circle2)
	{
		if(distanceToCenter<m_radius[2] && distanceToCenter>m_radius[1])
		{
			col.set(vxColor::black);
		}
	}
	
	if(m_circle3)
	{
		if(distanceToCenter<m_radius[4] && distanceToCenter>m_radius[3])
		{
			col.set(vxColor::black);
		}
	}
	
	if(m_margin)
	{
		if (distanceToCenter>m_radius[4]
			&& ((collision.u()<m_margn || collision.u()>(1.-m_margn)) 
				|| (collision.v()<m_margn || collision.v()>(1.-m_margn))))
		{
			col.set(vxColor::black);
		}
	}
	
	return col;
}


