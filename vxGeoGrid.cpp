#include <climits>
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
	m_xvalues.resize(m_rx+1);
	
	//Recording xlices to the grid.
	auto xSlice = fabs(m_bb->maxX() - m_bb->minX())/(scalar)(m_rx);
	auto k=0u;
	for(auto&& xval:m_xvalues)
	{
		xval = m_bb->minX() + xSlice * k++;
	}

	m_yvalues.resize(m_ry+1);
	auto ySlice = fabs(m_bb->maxY() - m_bb->minY())/(scalar)(m_ry);
	k=0u;
	for(auto&& yval:m_yvalues)
	{
		yval = m_bb->minY() + ySlice * k++;
	}

	m_zvalues.resize(m_rz+1);
	auto zSlice = fabs(m_bb->maxZ() - m_bb->minZ())/(scalar)(m_rz);
	k=0u;
	for(auto&& zval:m_zvalues)
	{
		zval = m_bb->minZ() + zSlice * k++;
	}
}

unsigned long vxGeoGrid::index(unsigned int a, unsigned int b, unsigned int c)
{
	return ((m_ry * m_rx) * c) + (m_rx * b) + a;
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

unsigned long vxGeoGrid::lookupVoxel(const v3 &v, 
									int &a, 
									int &b, 
									int &c) const
{
	if(v.x()<m_xvalues.front() || v.y()<m_yvalues.front() || v.z()<m_zvalues.front())
		return ULONG_MAX;
	
	if(v.x()>m_xvalues.back() || v.y()>m_yvalues.back() || v.z()>m_zvalues.back())
		return ULONG_MAX;
	
	a = 0;
	for(int i=1;i<m_xvalues.size()-1;i++)
	{
		if( v.x() < m_xvalues[i] )
		{
			break;
		}

		a++;
	}

	b = 0;
	for(int i=1;i<m_yvalues.size()-1;i++)
	{
		if( v.y() < m_yvalues[i] )
		{
			break;
		}

		b++;
	}
	
	c = 0;
	for(int i=1;i<m_zvalues.size()-1;i++)
	{
		if( v.z() < m_zvalues[i] )
		{
			break;
		}

		c++;
	}
	
	if(a<0 || b<0 || c<0)
	{
		std::cerr << "index out of bounds in GeoGrid " << __LINE__ << std::endl;
	}
	
	return ((m_ry * m_rx) * c) + (m_rx * b) + a;
}


unsigned long vxGeoGrid::lookupVoxel(const v3 &v) const
{
	if(!m_bb->contains(v))
		return -1;
	
	auto a = 0;
	for(int i=1;i<m_xvalues.size()-1;i++)
	{
		if( v.x() < m_xvalues[i] )
		{
			break;
		}

		a++;
	}

	auto b = 0;
	for(int i=1;i<m_yvalues.size()-1;i++)
	{
		if( v.y() < m_yvalues[i] )
		{
			break;
		}

		b++;
	}
	
	auto c = 0;
	for(int i=1;i<m_zvalues.size()-1;i++)
	{
		if( v.z() < m_zvalues[i] )
		{
			break;
		}

		c++;
	}
	
	if(a<0 || b<0 || c<0)
	{
		std::cerr << "index out of bounds in GeoGrid " << __LINE__ << std::endl;
	}
	
	return ((m_ry * m_rx) * c) + (m_rx * b) + a;
}

void vxGeoGrid::locateAndRegister(const vxTriRef &tri, unsigned long triangleID)
{
	auto idx1 = lookupVoxel(tri.p1);
	auto idx2 = lookupVoxel(tri.p2);
	auto idx3 = lookupVoxel(tri.p3);
	
	std::set<unsigned long> indices{idx1,idx2,idx3};

	for(auto&& index:indices)
	{
		if(indexIsValid(index))
		{
			if(m_members[index]==nullptr)
			{
				m_members[index] = std::make_shared<std::vector<unsigned long>>();
			}

			m_members[index]->push_back(triangleID);
		}
		else
		{
			std::cerr << "Looking up for index invalid while adding points " 
					  << index 
					  << std::endl;
		}
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

	auto velX = d.xPositive() ? 1 : 0;
	auto velY = d.yPositive() ? 1 : 0;
	auto velZ = d.zPositive() ? 1 : 0;

	int idX;
	int idY;
	int idZ;
	
	do
	{
		retVal = lookupVoxel(pIt, idX, idY, idZ);
		if(indexIsValid(retVal) && hasTriangles(retVal))
		{
			return m_members[retVal];
		}

		auto arrayIdxX = idX + velX;
		auto arrayIdxY = idY + velY;
		auto arrayIdxZ = idZ + velZ;
		
		auto xVal = m_xvalues[arrayIdxX];
		auto yVal = m_yvalues[arrayIdxY];
		auto zVal = m_zvalues[arrayIdxZ];

		v3 intersectX = MU::rectAndXPlane(d, xVal);
		if(intersectX.y() <= yVal
			|| intersectX.z() <= zVal)
		{
			pIt = intersectX + v3((velX ? 1 : -1)/20.0, 0.0, 0.0);
			continue;
		}

		v3 intersectY = MU::rectAndYPlane(d, yVal);
		if(intersectY.x() <= xVal
			|| intersectY.z() <= zVal)
		{
			pIt = intersectY + v3(0.0, (velY ? 1 : -1)/20.0, 0.0);
			continue;
		}

		v3 intersectZ = MU::rectAndZPlane(d, zVal);
		if(intersectZ.x() <= xVal
			|| intersectZ.y() <= yVal)
		{
			pIt = intersectZ + v3(0.0, 0.0, (velZ ? 1 : -1)/20.0);
			continue;
		}
		{
			std::cerr << "Not alocated ray. vxGeoGrid.cpp:284 () hitThis should not happen: " << std::endl;
		}
		
	}
	while(indexIsValid(retVal) && m_bb->contains(pIt));

	return vxGeoGrid::emptyListRef;
}

