#include "vxCirclesMap.h"

using namespace vxCore;

vxCirclesMap::vxCirclesMap()
{
}

vxColor vxCirclesMap::compute(const vxCollision &collision) const
{
	vxColor outColor(collision.color());
	
	auto&& u = collision.u();
	auto&& v = collision.v();
	
	outColor.set(collision.color());

	if(m_margin)
	{
		if (((u < m_margn || u>(1.-m_margn)) 
				|| (v<m_margn || v>(1.-m_margn))))
		{
			outColor.set(m_marginColor);
		}
	}
	
	return outColor;
}


