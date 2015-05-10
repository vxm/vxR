#include "vxColor.h"
#include "MathUtils.h"

using namespace vxCore;

vxColor vxColor::red = {1.0, 0.0, 0.0};
vxColor vxColor::green = {0.0, 1.0, 0.0};
vxColor vxColor::blue = {0.0, 0.0, 1.0};
vxColor vxColor::white = {1.0, 1.0, 1.0};
vxColor vxColor::black = {0.0, 0.0, 0.0};


void vxColor::toRGBA8888(unsigned char *tbuff) const
{
	*tbuff = (unsigned char)char(MathUtils::remap(m_r,255.0));
	tbuff++;
	
	*tbuff = (unsigned char)char(MathUtils::remap(m_g,255.0));
	tbuff++;
	
	*tbuff = (unsigned char)char(MathUtils::remap(m_b,255.0));
}
