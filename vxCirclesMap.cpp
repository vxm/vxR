#include "vxCirclesMap.h"

using namespace vxCore;

vxCirclesMap::vxCirclesMap()
{
}

vxColor vxCirclesMap::compute(const vxCollision &collision) const
{
	auto&& geoColor = collision.color();
	geoColor.set(collision.color().gained(1.2));
	vxColor outColor(geoColor);
	outColor.set(geoColor);

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


