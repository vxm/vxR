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
		
		m_xvalues[i]= bb->minX();
		m_xvalues[ii]= bb->maxX();
		
		m_yvalues[i]= bb->minY();
		m_yvalues[ii]= bb->maxY();
		
		m_zvalues[i]= bb->minZ();
		m_zvalues[ii]= bb->maxZ();
		
		m_bb->extend(*bb);
		
		++ii;
		++i;
	}
	
	m_rx = VU::sortAndUnique(m_xvalues);
	m_ry = VU::sortAndUnique(m_yvalues);
	m_rz = VU::sortAndUnique(m_zvalues);
	
	m_c_size = m_rx * m_ry * m_rz;
	m_members.resize(m_c_size);
	
	for(auto geo:m_geometries)
	{
		locateAndRegister(geo);
	}
	
}

unsigned long vxBroadPhase::index(unsigned int a, unsigned int b, unsigned int c) const
{
	return ((m_ry * m_rx) * c) + (m_rx * b) + a;
}

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
	
	if(a<0 || b<0 || c<0)
	{
		std::cerr << "index out of bounds in GeoGrid " << __LINE__ << std::endl;
	}
	
	return index(a,b,c);
}

void vxBroadPhase::locateAndRegister(vxGeometryHandle geo)
{
	auto bb = geo->boundingBox();
	auto cnt = bb->center();
	
	int a1,b1,c1;
	auto idx1 = lookupVoxel(bb->min() + ((bb->min() - cnt) / 10000.0), a1, b1, c1);
	
	int a2,b2,c2;
	auto idx2 = lookupVoxel(bb->max() + ((bb->max() - cnt) / 10000.0), a2, b2, c2);
	
#ifdef _DEBUG_
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
		
		m_members[idx1].index = idx1;
		m_members[idx1].geoRefs->emplace_back(geo);
		return;
	}
	
	for(auto x=a1;x<=a2;x++)
		for(auto y=b1;y<=b2;y++)
			for(auto z=c1;z<=c2;z++)
			{
				auto idx = index(x,y,z);
				
				if(m_members[idx].geoRefs == nullptr)
				{
					m_members[idx].geoRefs = std::make_shared<geometryHandleArray>();
				}
				
				//this index could be very redundant. //TODO:check
				m_members[idx].index = idx1;
				m_members[idx].geoRefs->emplace_back(geo);
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
		
		if(m_members[retVal].index!=m_c_size)
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

//	if(!m_root->throwRay(ray,collide))
//	{
//		return 0;
//	}
	
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
	/*
	using collision_geometryH = std::pair<vxCollision, vxGeometryHandle>;
	std::vector<collision_geometryH> cols;
	auto sp =  collide.position() 
			+ (collide.normal().inverted() / (scalar)10000.0);
	
	auto prev = m_c_size;
	bpSearchResult bbxs;
	do
	{
		bbxs = getList(ray, sp);
		
		if(prev==bbxs.index || bbxs.index == m_c_size)
		{
			break;
		}
		
		prev = bbxs.index;
		
		for(const auto &geo: *(bbxs.geoRefs))
		{
			auto exs = cols.end() == std::find_if(cols.begin(),cols.end(),
					[geo](const collision_geometryH& r)
									{return r.second.get()==geo.get();});

			if(exs && geo->throwRay(ray,collide))
			{
				cols.emplace_back(collide, geo);
			}
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
	*/
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
