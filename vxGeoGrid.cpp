#include "vxGeoGrid.h"

std::shared_ptr<vxBoundingBox> vxGeoGrid::bb() const
{
	return m_bb;
}

void vxGeoGrid::setBb(const std::shared_ptr<vxBoundingBox> &bb)
{
	m_bb = bb;
}

unsigned int vxGeoGrid::rx() const
{
	return m_rx;
}

void vxGeoGrid::setRx(unsigned int rx)
{
	m_rx = rx;
}

unsigned int vxGeoGrid::ry() const
{
	return m_ry;
}

void vxGeoGrid::setRy(unsigned int ry)
{
	m_ry = ry;
}

unsigned int vxGeoGrid::rz() const
{
	return m_rz;
}

void vxGeoGrid::setRz(unsigned int rz)
{
	m_rz = rz;
}

vxGeoGrid::vxGeoGrid()
{
	
}

