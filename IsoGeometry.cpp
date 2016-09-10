#include "IsoGeometry.h"

#include <functional>
#include <memory>

using namespace vxCore;

IsoGeometry::IsoGeometry()
{
	m_bb->set({0,0,0},1);
}

bool IsoGeometry::throwRay(const Ray &ray) const
{
	return Geometry::throwRay(ray);
}

int IsoGeometry::throwRay(const Ray &ray, Collision &col) const
{
	/*
	std::vector<rayFn> fs;
	std::vector<rayCondFn> fcs;
	
	auto yThreshold = [](const Ray &ray,Collision &col, rayFn &fn)
	{
		/// yUp capp
		if(col.positionu()<.5)
		{
			return 1;
		}
		
		return 0;
	};
	
	fcs.push_back(yThreshold);
	
	auto valid = [](const Ray &ray,Collision &col)
	{
		return 1;
	};

	
	auto f = [](const Ray &ray,Collision &col)
	{
		/// yUp capp
		if(col.normal()==v3s::constY && col.normal().follows(ray.direction()))
		{
			auto p = col.position();
			
			if(p.distance({0.0,p.y(),0.0})<.25)
			{
				return 1;
			}
		}
		
		return 0;
	};
	
	fs.emplace_back(valid);
	fs.emplace_back(f);
	
	for(auto i=0u;i<fcs.size();++i)
	{
		if(fcs[i](ray,col,fs[i]))
		{
			return 1;
		}
	}
	*/
	return 0;
}

bool IsoGeometry::hasCollision(const Ray &ray) const
{
	return Geometry::hasCollision(ray);
}
