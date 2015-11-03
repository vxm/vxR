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
v3 vxLegoBlockInGrid::position() const
{
	return m_pos;
}

void vxLegoBlockInGrid::setPosition(const v3 &value)
{
	m_pos = value;
}
