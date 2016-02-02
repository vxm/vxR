#include "vxBroadPhase.h"
#include <limits>
#include <algorithm>

#define DRAWBBOX 0

using namespace vxCore;

vxBroadPhase::vxBroadPhase()
{
	m_bb = std::make_shared<vxBoundingBox>();
}

void vxBroadPhase::addGeometry(vxGeometryHandle geo)
{
	m_geometries.emplace_back(geo);
}

vxBoundingBoxHandle vxBroadPhase::closestBox(const v3 &p) const
{
	vxBoundingBoxHandle ret;
	
	scalar d =  std::numeric_limits<scalar>::max();
	
	for(auto geo:m_geometries)
	{
		auto t = geo->boundingBox()->center().distance(p);
		
		if(t<d)
		{
			ret = geo->boundingBox();
			d = t;
		}
	}
	
	return ret;
}

void vxBroadPhase::updateCache()
{
	const auto dngs = m_geometries.size() * 2;
	
	m_xvalues.resize(dngs);
	m_yvalues.resize(dngs);
	m_zvalues.resize(dngs);
	
	m_bb->clear();
	
	unsigned int i=0;
	unsigned int ii=1;
	
	///Run through geometries and slice the 
	///broad phase
	for(auto geo:m_geometries)
	{
		auto bb = geo->boundingBox();
		
		m_xvalues[i] = bb->minX();
		m_xvalues[ii]= bb->maxX();
		
		m_yvalues[i] = bb->minY();
		m_yvalues[ii]= bb->maxY();
		
		m_zvalues[i] = bb->minZ();
		m_zvalues[ii]= bb->maxZ();
		
		m_bb->extend(*bb);
		
		ii+=2;
		i+=2;
	}
	
	//TODO:this sort could be less naive as I am 
	//inserting min and max, min and max...
	m_rx = VU::sortAndUnique(m_xvalues) - 1;
	m_ry = VU::sortAndUnique(m_yvalues) - 1;
	m_rz = VU::sortAndUnique(m_zvalues) - 1;
	
#ifdef _DEBUG
	std::cout << "Resolution: " << m_rx << " " << m_ry << " " << m_rz << std::endl;
	
	std::cout << "X: ";
	for(auto x:m_xvalues)
	{
		std::cout << ", " << x;
	}
	std::cout << std::endl;
	
	
	std::cout << "Y: ";
	for(auto y:m_yvalues)
	{
		std::cout << ", " << y;
	}
	std::cout << std::endl;
	
	
	std::cout << "Z: ";
	for(auto z:m_zvalues)
	{
		std::cout << ", " << z;
	}
	std::cout << std::endl;
#endif
	
	m_c_size = m_rx * m_ry * m_rz;
	m_members.resize(m_c_size);
	
	auto idx{0ul};
	for(auto &l:m_members)
	{
		l.index=idx++;
	}
	
	for(auto& geo:m_geometries)
	{
		locateAndRegister(geo);
	}
	
#if _DEBUG

	for(auto&l:m_members)
	{
		std::cout << "Index: " << l.index << std::endl;
		std::cout << "\tGeos: " << (l.geoRefs==nullptr ? 0 : l.geoRefs->size()) << std::endl;
	}
	
#endif
	
}

unsigned long vxBroadPhase::index(unsigned int a, unsigned int b, unsigned int c) const
{
	return ((m_ry * m_rx) * c) + (m_rx * b) + a;
}

//TODO:Binary search
unsigned long vxBroadPhase::lookupVoxel(const v3 &v, 
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

#if _DEBUG
	if(a<0 || b<0 || c<0)
	{
		std::cerr << "index out of bounds in GeoGrid " << __LINE__ << std::endl;
	}
#endif
	
	return index(a,b,c);
}

void vxBroadPhase::locateAndRegister(vxGeometryHandle geo)
{
	auto bb = geo->boundingBox();
	auto cnt = bb->center();
	int a1,b1,c1;
	auto idx1 = lookupVoxel(bb->min()+0.001, a1, b1, c1);
	int a2,b2,c2;
	auto idx2 = lookupVoxel(bb->max()-0.001, a2, b2, c2);

#ifdef _DEBUG
	std::cout << std::endl << std::endl << std::endl;
	std::cout << "bb->min() " << bb->min() << std::endl;
	
	std::cout << "a1 " << a1 << std::endl;
	std::cout << "b1 " << b1 << std::endl;
	std::cout << "c1 " << c1 << std::endl;
	
	std::cout << "bb->max() " << bb->max() << std::endl;
	std::cout << "a2 " << a1 << std::endl;
	std::cout << "b2 " << b1 << std::endl;
	std::cout << "c2 " << c1 << std::endl;
	
	if(!indexIsValid(idx1) || !indexIsValid(idx2))
	{
		std::cerr << "Adding gmetry to an invalid " 
				  << std::endl;
	}
#endif
	
	if(idx1==idx2)
	{
		if(m_members[idx1].geoRefs == nullptr)
		{
			m_members[idx1].geoRefs = std::make_shared<geometryHandleArray>();
		}
		
		m_members[idx1].geoRefs->emplace_back(geo);
		return;
	}
	
	for(auto x=a1;x<=a2;x++)
	{
		std::cout << "x: " << x << std::endl;
		for(auto y=b1;y<=b2;y++)
		{
			std::cout << "\ty: " << y << std::endl;
			
			for(auto z=c1;z<=c2;z++)
			{
				std::cout << "\t\tz: " << z << std::endl;

				auto idx = index(x,y,z);
/*				if(idx==1)
				{
					std::cerr << "not possible" << std::endl;
				}*/
				
				if(m_members[idx].geoRefs == nullptr)
				{
					m_members[idx].geoRefs = std::make_shared<geometryHandleArray>();
				}
				
				//this index could be very redundant. //TODO:check
				m_members[idx].geoRefs->push_back(geo);
			}
		}
	}
}

const bpSearchResult vxBroadPhase::getList(const vxRay &ray, v3 &sp) const
{
	long retVal{m_c_size};
	
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
		retVal = lookupVoxel(sp, idX, idY, idZ);
		
		if(!indexIsValid(retVal))
		{
			return bpSearchResult{m_c_size};
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
		
		if(m_members[retVal].geoRefs!=nullptr)
		{
			return m_members[retVal];
		}
	}
	while(indexIsValid(retVal) && m_bb->contains(sp));
	
	return bpSearchResult{m_c_size};
}

bool vxBroadPhase::indexIsValid(const long idx) const
{
	return !(idx<0l || idx>=m_c_size);
}

bool vxBroadPhase::throwRay(const vxRay &ray) const
{
	vxCollision col;
	if(throwRay(ray,col))
	{
		return true;
	}
	
	return false;
}

#define NAIVE_BB_METHOD 0
int vxBroadPhase::throwRay(const vxRay &ray, vxCollision &collide) const
{
#if	DRAWBBOX
	if(m_bb->throwRay(ray, collide))
	{
		collide.setColor(m_color);
		collide.setValid(true);
		
		return 1;
	}
#else
#if NAIVE_BB_METHOD
	auto mdis = std::numeric_limits<scalar>::max(); 
	
	vxCollision temp = collide;
	int valid {0};
	
	for(auto&& geo:m_geometries)
	{
		auto hitValid = geo->throwRay(ray, temp);
		
		if(hitValid)
		{
			auto s = temp.position().distance(ray.origin());
			
			if(s < mdis)
			{
				mdis = s;
				collide = temp;
				valid = 1;
			}
		}
	}
	
	return valid;
#else
	if(!m_bb->throwRay(ray, collide))
	{
		collide.setValid(false);
		return 0;
	}

	using collision_geometryH = std::pair<vxCollision, vxGeometryHandle>;
	std::vector<collision_geometryH> cols;
	auto sp =  collide.position() 
			+ (collide.normal().inverted() / (scalar)10000.0);
			
	auto prev = m_c_size;
	bpSearchResult bbxs;
	do
	{
		bbxs = getList(ray, sp);
		
		if(bbxs.geoRefs==nullptr || prev==bbxs.index || bbxs.index == m_c_size)
		{
			collide.setValid(false);
			return 0;
		}
		
		prev = bbxs.index;
		
		std::set<vxGeometryHandle> viewedGeos;
		int sz=0;
		for(const auto &geo: *(bbxs.geoRefs))
		{
			if(viewedGeos.size()==sz && geo->throwRay(ray,collide))
			{
				cols.emplace_back(collide, geo);
			}
			viewedGeos.insert(geo);
			sz = viewedGeos.size();
		}
	}
	while(!cols.size());
	
	if(cols.size())
	{
		auto mind  = (cols[0].first.position()-ray.origin()).length();
		collide = cols[0].first;
		int i{0};
		for(auto&& c:cols)
		{
			i++;
			auto ds = (c.first.position()-ray.origin()).length();
			if( ds < mind )
			{
				collide = c.first;
				collide.setColor(c.second->baseColor());
			}
		}
		
		collide.setValid(true);
		collide.setUV(v2(0.5,0.5));
		return 1;
	}
#endif
#endif
	return 0;
}

bool vxBroadPhase::hasCollision(const vxRay &ray) const
{
	vxCollision col;
	if(throwRay(ray,col))
	{
		return true;
	}
	
	return false;
}
