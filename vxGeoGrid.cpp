#include <climits>
#include "vxGeoGrid.h"

const searchResult vxGeoGrid::invalidResult{0ul, std::make_shared<std::vector<unsigned long>>()};

vxGeoGrid::vxGeoGrid()
{
}

std::shared_ptr<vxBoundingBox> vxGeoGrid::bb() const
{
	return m_bb;
}

void vxGeoGrid::updateCache()
{
	m_members.resize(m_rx * m_ry * m_rz);
	m_xvalues.resize(m_rx+1);
	
	//Recording xlices to the grid.
	m_xSlice = fabs(m_bb->maxX() - m_bb->minX())/(scalar)(m_rx);
	auto k=0u;
	for(auto& xval:m_xvalues)
	{
		xval = m_bb->minX() + m_xSlice * k++;
	}
	
	m_yvalues.resize(m_ry+1);
	m_ySlice = fabs(m_bb->maxY() - m_bb->minY())/(scalar)(m_ry);
	k=0u;
	for(auto& yval:m_yvalues)
	{
		yval = m_bb->minY() + m_ySlice * k++;
	}
	
	m_zvalues.resize(m_rz+1);
	m_zSlice = fabs(m_bb->maxZ() - m_bb->minZ())/(scalar)(m_rz);
	k=0u;
	for(auto& zval:m_zvalues)
	{
		zval = m_bb->minZ() + m_zSlice * k++;
	}
	
	m_c_xBoxSize = m_bb->xLength() / (scalar)(m_xvalues.size() - 1);
	m_c_yBoxSize = m_bb->yLength() / (scalar)(m_yvalues.size() - 1);
	m_c_zBoxSize = m_bb->zLength() / (scalar)(m_zvalues.size() - 1);
}


void vxGeoGrid::setResolution(unsigned int x,
							  unsigned int y,
							  unsigned int z)
{
	m_rx = x;
	m_ry = y;
	m_rz = z;
	
	updateCache();
}

void vxGeoGrid::setBb(const std::shared_ptr<vxBoundingBox> &bb)
{
	m_bb = bb;
	
	auto d = m_bb->diagonal();
	setResolution(MU::clamp((unsigned int)(d.x()*64.0), 4u, 64u),
				  MU::clamp((unsigned int)(d.y()*64.0), 4u, 64u),
				  MU::clamp((unsigned int)(d.z()*64.0), 4u, 64u));
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
	return size();
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

unsigned long vxGeoGrid::linearLookupVoxel(const v3 &v, 
										   int& a, 
										   int& b, 
										   int& c) const
{
	
	a = (v.x() - m_bb->minX()) / m_c_xBoxSize;
	if(a==m_rx)
	{
		a--;
	}
	
	b = (v.y() - m_bb->minY()) / m_c_yBoxSize;
	if(b==m_ry)
	{
		b--;
	}
	
	c = (v.z() - m_bb->minZ()) / m_c_zBoxSize;
	if(c==m_rz)
	{
		c--;
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
	auto idx1 = linearLookupVoxel(tri.p1,a1,b1,c1);
	
	int a2,b2,c2;
	auto idx2 = linearLookupVoxel(tri.p2,a2,b2,c2);
	
	int a3,b3,c3;
	auto idx3 = linearLookupVoxel(tri.p3,a3,b3,c3);
	
	// like in the most cases.
	if(idx1==idx2 && idx2==idx3)
	{
		if(indexIsValid(idx1))
		{
			if(m_members[idx1].listRef ==nullptr)
			{
				m_members[idx1].listRef = std::make_shared<std::vector<unsigned long>>();
			}
			
			m_members[idx1].listRef->emplace_back(triangleID);
		}
		else
		{
			std::cerr << "Vertex added to GeoGrid wich has an invalid index"
					  << std::endl;
		}
		
		return;
	}
	
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
				const auto idx = index(x,y,z);
				
				if(indexIsValid(idx))
				{
					if(m_members[idx].listRef==nullptr)
					{
						m_members[idx].listRef = std::make_shared<std::vector<unsigned long>>();
					}
					
					m_members[idx].index = idx;
					m_members[idx].listRef->emplace_back(triangleID);
				}
				else
				{
					std::cout << "Looking up for index invalid while adding points " 
							  << idx
							  << "\t x: "
							  << x 
							  << "\t y: "
							  << y 
							  << "\t z: "
							  << z 
							  << std::endl;
				}
			}
}

bool vxGeoGrid::hasTriangles(const long idx) const
{
	return !(m_members[idx].listRef==nullptr);
}

bool vxGeoGrid::indexIsValid(const long idx) const
{
	return !(idx<0l || idx>=numVoxels());
}

const searchResult vxGeoGrid::getList(const vxRay &ray, v3 &sp) const
{
	long retVal{-1l};
	
	const auto& d = ray.direction();
	const auto& p = ray.origin();
	
	auto velX = d.xPositive() ? 1 : 0;
	auto velY = d.yPositive() ? 1 : 0;
	auto velZ = d.zPositive() ? 1 : 0;
	
	int idX;
	int idY;
	int idZ;
	
	do
	{
		retVal = linearLookupVoxel(sp, idX, idY, idZ);
		
		if(!indexIsValid(retVal))
		{
			return vxGeoGrid::invalidResult;
		}
		
		auto xVal = m_xvalues[idX + velX] - p.x();
		auto yVal = m_yvalues[idY + velY] - p.y();
		auto zVal = m_zvalues[idZ + velZ] - p.z();
		
		v3 intersectX = MU::rectAndXPlane(d, xVal);
		if(fabs(intersectX.y()) <= fabs(yVal)
				&& fabs(intersectX.z()) <= fabs(zVal))
		{
			sp = p + intersectX + v3((velX ? 1.0 : -1.0)/100.0, 0.0, 0.0);
		}
		
		v3 intersectY = MU::rectAndYPlane(d, yVal);
		if(fabs(intersectY.x()) <= fabs(xVal)
				&& fabs(intersectY.z()) <= fabs(zVal))
		{
			sp = p + intersectY + v3(0.0, (velY ? 1.0 : -1.0)/100.0, 0.0);
		}
		
		v3 intersectZ = MU::rectAndZPlane(d, zVal);
		if(fabs(intersectZ.x()) <= fabs(xVal)
				&& fabs(intersectZ.y()) <= fabs(yVal))
		{
			sp = p + intersectZ + v3(0.0, 0.0, (velZ ? 1.0 : -1.0)/100.0);
		}
		
		if(hasTriangles(retVal))
		{
			return m_members[retVal];
		}
	}
	while(indexIsValid(retVal) && m_bb->contains(sp));
	
	return vxGeoGrid::invalidResult;
}

