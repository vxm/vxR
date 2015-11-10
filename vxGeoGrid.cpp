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

int vxGeoGrid::lookupVoxel(const v3 &v, 
						   unsigned int &a, 
						   unsigned int &b, 
						   unsigned int &c) const
{
	if(v.x()<m_xvalues.front() || v.y()<m_yvalues.front() || v.z()<m_zvalues.front())
		return -1;
	
	if(v.x()>m_xvalues.back() || v.y()>m_yvalues.back() || v.z()>m_zvalues.back())
		return -1;
	
	a = -1;
	for(auto&&x:m_xvalues)
	{
		if(v.x()<x)
		{
			break;
		}

		a++;
	}

	b = -1;
	for(auto&&y:m_yvalues)
	{
		if(v.y()<y)
		{
			break;
		}

		b++;
	}
	
	c = -1;
	for(auto&&z:m_zvalues)
	{
		if(v.z()<z)
		{
			break;
		}

		c++;
	}
	
	return ((m_ry * m_rx) * c) + (m_rx * b) + a;
}


int vxGeoGrid::lookupVoxel(const v3 &v) const
{
	if(!m_bb->contains(v))
		return -1;
	
	auto a = -1;
	for(auto&&x:m_xvalues)
	{
		if(v.x()<x)
		{
			break;
		}

		a++;
	}

	auto b = -1;
	for(auto&&y:m_yvalues)
	{
		if(v.y()<y)
		{
			break;
		}

		b++;
	}
	
	auto c = -1;
	for(auto&&z:m_zvalues)
	{
		if(v.z()<z)
		{
			break;
		}

		c++;
	}
	
	return ((m_ry * m_rx) * c) + (m_rx * b) + a;
}

void vxGeoGrid::locateAndRegister(const vxTriRef &tri, unsigned long triangleID)
{
	std::set<unsigned long> indices;
	indices.insert(lookupVoxel(tri.p1));
	indices.insert(lookupVoxel(tri.p2));
	indices.insert(lookupVoxel(tri.p3));

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
	return !(idx<0 || idx>=numVoxels());
}

const triangleIdsRef vxGeoGrid::getList(const vxRay &ray, const v3 &sp) const
{
	auto pIt = sp;
	long retVal{-1l};

	const auto& d = ray.direction();

	auto xvel = d.xSign() ? -1 : 1;
	auto yvel = d.ySign() ? -1 : 1;
	auto zvel = d.zSign() ? -1 : 1;

	unsigned int idX;
	unsigned int idY;
	unsigned int idZ;
	
	do
	{
		retVal = lookupVoxel(pIt, idX, idY, idZ);
		if(indexIsValid(retVal) && hasTriangles(retVal))
		{
			return m_members[retVal];
		}

		auto xIterator = m_xvalues[idX + xvel];
		auto yIterator = m_yvalues[idY + yvel];
		auto zIterator = m_zvalues[idZ + zvel];

		auto&& xPlaneIntersec = MU::rectAndXPlane(d, xIterator);
		if(xPlaneIntersec.z() < zIterator)
		{
			pIt = xPlaneIntersec;
			continue;
		}

		auto&& yPlaneIntersec = MU::rectAndYPlane(d, yIterator);
		if(yPlaneIntersec.x() < xIterator)
		{
			pIt = yPlaneIntersec;
			continue;
		}

		auto&& zPlaneIntersec = MU::rectAndZPlane(d, zIterator);
		if(zPlaneIntersec.x() < xIterator)
		{
			pIt = zPlaneIntersec;
			continue;
		}
		
	}
	while(m_bb->contains(pIt));

	return vxGeoGrid::emptyListRef;
}

