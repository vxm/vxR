#include <climits>
#include "vxGeoGrid.h"

const searchResult vxGeoGrid::invalidResult{0ul, std::make_shared<std::vector<unsigned long>>()};

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

unsigned int vxGeoGrid::size() const
{
	return m_rx * m_ry * m_rz;
}

unsigned long vxGeoGrid::index(unsigned int a, 
							   unsigned int b, 
							   unsigned int c) const
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

///
/// \brief vxGeoGrid::lookupVoxel
/// \param v
/// \param a
/// \param b
/// \param c
/// \return 
/// Very inneficient to lookup in arrays.
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
	for(unsigned int i=1;i<m_xvalues.size()-1;i++)
	{
		if( v.x() < m_xvalues[i] )
		{
			break;
		}

		a++;
	}

	b = 0;
	for(unsigned int i=1;i<m_yvalues.size()-1;i++)
	{
		if( v.y() < m_yvalues[i] )
		{
			break;
		}

		b++;
	}
	
	c = 0;
	for(unsigned int i=1;i<m_zvalues.size()-1;i++)
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
	
	return index(a,b,c);
}

void vxGeoGrid::locateAndRegister(const vxTriRef &tri, unsigned long triangleID)
{
	int a1,b1,c1;
	auto idx1 = lookupVoxel(tri.p1,a1,b1,c1);
	
	int a2,b2,c2;
	auto idx2 = lookupVoxel(tri.p2,a2,b2,c2);
	
	int a3,b3,c3;
	auto idx3 = lookupVoxel(tri.p3,a3,b3,c3);
	
	// like the most of the cases.
	if(idx1==idx2 && idx2==idx3)
	{
		if(indexIsValid(idx1))
		{
			if(m_members[idx1].listRef ==nullptr)
			{
				m_members[idx1].listRef = std::make_shared<std::vector<unsigned long>>();
			}

			m_members[idx1].listRef->push_back(triangleID);
		}
		
		return;
	}
	
	//std::set<unsigned long> indices{idx1,idx2,idx3};

	auto aMin = std::min(a3,std::min(a1,a2));
	auto bMin = std::min(b3,std::min(b1,b2));
	auto cMin = std::min(c3,std::min(c1,c2));
	
	auto aMax = std::max(a3,std::max(a1,a2));
	auto bMax = std::max(b3,std::max(b1,b2));
	auto cMax = std::max(c3,std::max(c1,c2));
	
	//for(auto&& index:indices)
	for(auto x = aMin; x<= aMax; x++)
		for(auto y = bMin; y<= bMax; y++)
			for(auto z = cMin; z<= cMax; z++)
	{
		auto idx = index(x,y,z);

		if(indexIsValid(idx))
		{
			if(m_members[idx].listRef==nullptr)
			{
				m_members[idx].listRef = std::make_shared<std::vector<unsigned long>>();
			}

			m_members[idx].index = idx;
			m_members[idx].listRef->push_back(triangleID);
		}
		else
		{
			std::cerr << "Looking up for index invalid while adding points " 
					  << idx 
					  << std::endl;
		}
	}
}

bool vxGeoGrid::hasTriangles(const long idx) const
{
	return m_members[idx].index!=size();
}

bool vxGeoGrid::indexIsValid(const long idx) const
{
	return !(idx<0l || idx>=numVoxels());
}

const searchResult vxGeoGrid::getList(const vxRay &ray, v3 &sp) const
{
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
		retVal = lookupVoxel(sp, idX, idY, idZ);
		
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
		if(fabs(intersectX.y()) <= fabs(yVal)
			&& fabs(intersectX.z()) <= fabs(zVal))
		{
			sp = intersectX + v3((velX ? 1.0 : -1.0)/100.0, 0.0, 0.0);
			continue;
		}

		v3 intersectY = MU::rectAndYPlane(d, yVal);
		if(fabs(intersectY.x()) <= fabs(xVal)
			&& fabs(intersectY.z()) <= fabs(zVal))
		{
			sp = intersectY + v3(0.0, (velY ? 1.0 : -1.0)/100.0, 0.0);
			continue;
		}

		v3 intersectZ = MU::rectAndZPlane(d, zVal);
		if(fabs(intersectZ.x()) <= fabs(xVal)
			&& fabs(intersectZ.y()) <= fabs(yVal))
		{
			sp = intersectZ + v3(0.0, 0.0, (velZ ? 1.0 : -1.0)/100.0);
			continue;
		}
		
		
		{
			sp += ray.direction();
			
			std::cerr << sp << "Not alocated ray. vxGeoGrid.cpp:284 () hitThis should not happen: " << std::endl;
		}
		
	}
	while(indexIsValid(retVal) && m_bb->contains(sp));

	return vxGeoGrid::invalidResult;
}

