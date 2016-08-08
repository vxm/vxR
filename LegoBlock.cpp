#include "LegoBlock.h"

using namespace vxCore;

LegoBlock::LegoBlock()
{
}


scalar LegoBlockInGrid::size() const
{
	return m_size;
}

void LegoBlockInGrid::setSize(scalar value)
{
	m_size = value;
}
v3s LegoBlockInGrid::position() const
{
	return m_pos;
}

void LegoBlockInGrid::setPosition(const v3s &value)
{
	m_pos = value;
}
