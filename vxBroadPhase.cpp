#include "vxBroadPhase.h"
#include <limits>

#define DRAWBBOX 1

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

	//run through geometries
	unsigned int i=0;
	unsigned int ii=1;
	
	m_c_size = m_rx * m_ry * m_rz;
	
	m_bb->clear();
	m_members.resize(m_c_size);

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
	
	for(auto geo:m_geometries)
	{
		locateAndRegister(geo);
	}
	
	m_rz = m_ry = m_rx = dngs-1;
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
	
	int a,b,c;
	auto idx = lookupVoxel(cnt, a, b, c);
	
	if(indexIsValid(idx))
	{
		if(m_members[idx].geoRef == nullptr)
		{
			m_members[idx].geoRef = geo;
			m_members[idx].index = idx;
		}
		else
		{
			std::cerr << "Adding geometry to a occupied voxel (unhandled)" 
					  << std::endl;
		}
	}
}

bool vxBroadPhase::indexIsValid(const long idx) const
{
	return !(idx<0l || idx>=m_c_size);
}

bool vxBroadPhase::throwRay(const vxRay &ray) const
{
	for(auto&& geo:m_geometries)
	{
		if(geo->throwRay(ray))
		{
			return true;
		}
	}
	
	return false;
}

int vxBroadPhase::throwRay(const vxRay &ray, vxCollision &collide) const
{
#if	DRAWBBOX
	if(bb->throwRay(ray, collide))
	{
		collide.setColor(m_color);
		collide.setValid(true);

		return 1;
	}
#else
	if(!m_bb->throwRay(ray, collide))
	{
		return 0;
	}
	
	auto&& p = ray.origin();
	
	std::vector<vxCollision> cols;
	auto sp =  collide.position() 
			+ (collide.normal().inverted() / (scalar)10000.0);

	auto prev = size();
	bpSearchResult bbxs;

	do
	{
		bbxs = getList(ray, sp);
		
		if(prev==bbxs.index)
		{
			break;
		}
		
		prev = bbxs.index;
		
		for(const auto &id: *(bbxs.listRef) )
		{
			if(m_triangles[id].throwRay(ray,collide))
			{
				if(!ray.direction().follows((collide.position()-ray.origin()).unit()))
				{
					cols.emplace_back(collide);
				}
			}
		}
	}
	while(!cols.size());

	if(cols.size())
	{
		auto mind  = (cols[0].position()-p).length();
		collide = cols[0];
		int i{0};
		for(auto&& c:cols)
		{
			i++;
			auto ds = (c.position()-p).length();
			if( ds < mind )
			{
				collide = c;
			}
		}

		collide.setColor(m_baseColor);
		collide.setValid(true);
		collide.setUV(v2(0.5,0.5));
		return 1;
	}
	
#endif
	return 0;
	
	/*
//	if(!m_root->throwRay(ray,collide))
//	{
//		return 0;
//	}
	
	auto mdis = std::numeric_limits<scalar>::max(); 
	
	vxCollision temp = collide;
	int valid {0};
	
	for(auto& geo:m_geometries)
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

	return valid;*/
}

bool vxBroadPhase::hasCollision(const vxRay &ray) const
{
	vxCollision col;
	for(auto&& geo:m_geometries)
	{
		geo->throwRay(ray,col);
		if(col.isValid())
		{
			return true;
		}
	}
	
	return false;
}
