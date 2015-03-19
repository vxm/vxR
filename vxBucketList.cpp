#include "vxBucketList.h"

using namespace vxStorage;

std::shared_ptr<const ImageProperties> vxBucketList::prop() const
{
	return m_prop;
}

void vxBucketList::setProp(const std::shared_ptr<ImageProperties> &prop)
{
	m_prop = prop;
}
