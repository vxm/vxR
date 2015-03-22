#include <vxCamera.h>
#include <ImageProperties.h>

using namespace vxStorage;

std::shared_ptr<const ImageProperties>  vxCamera::prop() const
{
	return m_prop;
}

void vxCamera::setProp(std::shared_ptr<const ImageProperties> prop)
{
	m_prop = prop;
}


