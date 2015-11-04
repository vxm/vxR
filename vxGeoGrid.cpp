#include "vxGeoGrid.h"

listRef vxGeoGrid::emptyListRef = std::make_shared<std::vector<unsigned long>>();

vxGeoGrid::vxGeoGrid()
{
	m_members.resize(m_rx * m_ry * m_rz);
}

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

unsigned int vxGeoGrid::lookupVoxel(const scalar a, const scalar b, const scalar c) const
{
	auto&& x = (unsigned int)floor(m_rx * a);
	auto&& y = (unsigned int)floor(m_ry * b);
	auto&& z = (unsigned int)floor(m_rz * c);
	
	if(x>=m_rx)
	{
		x=m_rx-1;
	}
	if(y>=m_ry)
	{
		y=m_ry-1;
	}
	if(z>=m_rz)
	{
		z=m_rz-1;
	}
	
	return ((m_ry * m_rx) * z) + (m_rx * y) + x;
}

void vxGeoGrid::locateAndRegister(const vxTriRef &tri, unsigned long triangleID)
{
	auto r1x = MU::scaleTo01(m_bb->minX(), m_bb->maxX(), tri.p1.x());
	auto r1y = MU::scaleTo01(m_bb->minY(), m_bb->maxY(), tri.p1.y());
	auto r1z = MU::scaleTo01(m_bb->minZ(), m_bb->maxZ(), tri.p1.z());

	std::set<unsigned long> indices{lookupVoxel(r1x, r1y, r1z)};

	auto r2x = MU::scaleTo01(m_bb->minX(), m_bb->maxX(), tri.p2.x());
	auto r2y = MU::scaleTo01(m_bb->minY(), m_bb->maxY(), tri.p2.y());
	auto r2z = MU::scaleTo01(m_bb->minZ(), m_bb->maxZ(), tri.p2.z());

	indices.insert(lookupVoxel(r2x, r2y, r2z));

	auto r3x = MU::scaleTo01(m_bb->minX(), m_bb->maxX(), tri.p3.x());
	auto r3y = MU::scaleTo01(m_bb->minY(), m_bb->maxY(), tri.p3.y());
	auto r3z = MU::scaleTo01(m_bb->minZ(), m_bb->maxZ(), tri.p3.z());

	indices.insert(lookupVoxel(r3x, r3y, r3z));

	for(auto&& index:indices)
	{
		if(m_members[index]==nullptr)
		{
			m_members[index] = std::make_shared<std::vector<unsigned long>>();
		}
		
		m_members[index]->push_back(triangleID);
	}
}

listRef vxGeoGrid::getList(const vxRay &ray) const
{
	//std::vector<unsigned long> r{1,2,3,4,5,6,7,8};
//	std::cout << "Accessing elements" << std::endl;

	auto member{87ul};
	if(m_members[member]==nullptr)
		return vxGeoGrid::emptyListRef;
	
	return m_members[member];
}

