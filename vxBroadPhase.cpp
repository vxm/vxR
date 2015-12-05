#include "vxBroadPhase.h"
#include <limits>

using namespace vxCore;

vxBroadPhase::vxBroadPhase()
{
	m_root = std::make_shared<vxBoundingBox>();
}

void vxBroadPhase::addGeometry(vxGeometryHandle geo)
{
	m_root->extend(*geo->boundingBox());
	m_geometries.push_back(geo);
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

