#include "vxGeoGrid.h"

triangleIdsRef vxGeoGrid::emptyListRef = std::make_shared<std::vector<unsigned long>>();

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

void vxGeoGrid::close()
{
	m_xvalues.resize(m_rx);
	
	//Recording xlices to the grid.
	auto xSlice = fabs(m_bb->maxX() - m_bb->minX())/(scalar)(m_rx-1);
	auto k=0u;
	for(auto&& xval:m_xvalues)
	{
		xval = m_bb->minX() + xSlice * k++;
	}

	m_yvalues.resize(m_ry);
	auto ySlice = fabs(m_bb->maxY() - m_bb->minY())/(scalar)(m_ry-1);
	k=0u;
	for(auto&& yval:m_yvalues)
	{
		yval = m_bb->minY() + ySlice * k++;
	}

	m_zvalues.resize(m_rz);
	auto zSlice = fabs(m_bb->maxZ() - m_bb->minZ())/(scalar)(m_rz-1);
	k=0u;
	for(auto&& zval:m_zvalues)
	{
		zval = m_bb->minZ() + zSlice * k++;
	}
	
}

unsigned long vxGeoGrid::numVoxels() const
{
	return m_rx * m_ry * m_rz;
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

int vxGeoGrid::lookupVoxel(const v3 &v) const
{
	return lookupVoxel(v.x(), v.y(), v.z());
}

int vxGeoGrid::lookupVoxel(const scalar a, const scalar b, const scalar c) const
{
	auto&& x = (long)floor(m_rx * a);
	auto&& y = (long)floor(m_ry * b);
	auto&& z = (long)floor(m_rz * c);
	
	if(x==m_rx)
	{
		x=m_rx-1;
	}
	
	if(y==m_ry)
	{
		y=m_ry-1;
	}
	
	if(z==m_rz)
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

bool vxGeoGrid::hasTriangles(long idx) const
{
	return !(m_members[idx]==nullptr);
}

bool vxGeoGrid::indexIsValid(long idx) const
{
	return !(idx<0 || idx>=numVoxels() || !hasTriangles(idx));
}

const triangleIdsRef vxGeoGrid::getList(const vxRay &ray, const v3 &sp) const
{
	auto pIt = sp;
	long retVal{-1l};

	const auto& d = ray.direction();

	auto xvel = d.xSign() ? -1 : 1;
	auto yvel = d.ySign() ? -1 : 1;
	auto zvel = d.zSign() ? -1 : 1;
	
	do
	{
		auto&& r1x = MU::scaleTo01(m_bb->minX(), m_bb->maxX(), pIt.x());
		auto&& r1y = MU::scaleTo01(m_bb->minY(), m_bb->maxY(), pIt.y());
		auto&& r1z = MU::scaleTo01(m_bb->minZ(), m_bb->maxZ(), pIt.z());

		retVal = lookupVoxel(r1x,r1y,r1z);
		if(hasTriangles(retVal))
		{
			return  m_members[retVal];
		}

		auto xIterator = m_xvalues[((long)floor(m_rx * r1x)) + xvel];
		auto yIterator = m_yvalues[((long)floor(m_ry * r1y)) + yvel];
		auto zIterator = m_zvalues[((long)floor(m_rz * r1z)) + zvel];

		auto&& xPlaneIntersec = MU::rectAndXPlane(d, xIterator);
		auto&& yPlaneIntersec = MU::rectAndYPlane(d, yIterator);
		auto&& zPlaneIntersec = MU::rectAndZPlane(d, zIterator);

		if(xPlaneIntersec.z() < zIterator)
		{
			pIt = xPlaneIntersec;
			continue;
		}

		if(yPlaneIntersec.x() < xIterator)
		{
			pIt = yPlaneIntersec;
			continue;
		}
		
		if(zPlaneIntersec.x() < xIterator)
		{
			pIt = zPlaneIntersec;
			continue;
		}
	}
	while(m_bb->contains(pIt));

	return vxGeoGrid::emptyListRef;
}

