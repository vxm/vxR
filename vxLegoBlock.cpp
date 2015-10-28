#include "vxLegoBlock.h"

vxLegoBlock::vxLegoBlock()
{
}


scalar vxLegoBlockInGrid::size() const
{
	return m_size;
}

void vxLegoBlockInGrid::setSize(scalar value)
{
	m_size = value;
}
vxVector3d vxLegoBlockInGrid::position() const
{
	return m_pos;
}

void vxLegoBlockInGrid::setPosition(const vxVector3d &value)
{
	m_pos = value;
}
