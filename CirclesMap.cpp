#include "CirclesMap.h"

using namespace vxCore;

Color CirclesMap::compute(const Collision &collision) const
{
	Color outColor(Color::white);
return outColor;
	auto dst = collision.uv().distance({.5,.5});

	if(m_circle1)
	{
		if(dst>m_radii[0] && dst<m_radii[1])
		{
			outColor.set(collision.color());
		}
	}

	if(m_circle2)
	{
		if(dst>m_radii[1] && dst<m_radii[2])
		{
			outColor.set(Color::orange);
		}
	}
	
	if(m_circle3)
	{
		if(dst>m_radii[2] && dst<m_radii[3])
		{
			outColor.set(collision.color());
		}
	}
	
	if(m_margin)
	{
		if (((collision.u()<m_margn || collision.u()>(1.-m_margn)) 
				|| (collision.v()<m_margn || collision.v()>(1.-m_margn))))
		{
			outColor.set(Color::black);
		}
	}
	
	return outColor;
}


