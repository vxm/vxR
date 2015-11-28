#include "vxBroadPhase.h"

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
//	if(!m_root->hasCollision(ray))
//	{
//		return 0;
//	}
	
	for(auto&& geo:m_geometries)
	{
		if(geo->throwRay(ray, collide))
		{
			return 1;
		}
	}

	return 0;
}

bool vxBroadPhase::hasCollision(const vxRay &ray) const
{
	for(auto&& geo:m_geometries)
	{
		if(geo->hasCollision(ray))
		{
			return true;
		}
	}
	
	return false;
}

