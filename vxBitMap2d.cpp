#include "vxBitMap2d.h"

using namespace vxCore;

vxBitMap2d::vxBitMap2d()
	:  m_data(m_prop)
{
	m_prop = std::make_shared<ImageProperties>(100,100);
}

vxColor vxBitMap2d::compute(const vxCollision &collision) const
{
	const auto&& remap = vxVector2d(collision.u(),collision.v());
	const auto&& distanceToCenter = remap.length();

	vxColor outColor(vxColor::white);

	if(m_circle1)
	{
		if(distanceToCenter>m_radius[0] && distanceToCenter<m_radius[1])
		{
			outColor.set(vxColor(.9,.8,.4));
		}
	}

	if(m_circle2)
	{
		if(distanceToCenter>m_radius[1] && distanceToCenter<m_radius[2])
		{
			outColor.set(vxColor(.9,.9,.9));
		}
	}
	
	if(m_circle3)
	{
		if(distanceToCenter>m_radius[2] && distanceToCenter<m_radius[3])
		{
			outColor.set(vxColor(.6,.6,.95));
		}
	}
	
	return outColor;
}


