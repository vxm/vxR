#include <vxCamera.h>
#include <imageproperties.h>

using namespace vxStorage;

ImageProperties vxCamera::prop() const
{
	return m_prop;
}

void vxCamera::setProp(const ImageProperties &prop)
{
	m_prop = prop;
}


