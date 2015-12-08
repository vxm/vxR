#include "vxTriRef.h"
#include "MathUtils.h"

using namespace vxCore;

vxCore::vxTriRef::vxTriRef(const v3 &a, 
						   const v3 &b, 
						   const v3 &c)
	:p1(a)
	,p2(b)
	,p3(c)
	,m_n{v3::zero}
{
}

vxTriRef::vxTriRef(vxTriRef &&other)
	:p1{std::move(other.p1)}
	,p2{std::move(other.p2)}
	,p3{std::move(other.p3)}
	,m_n{std::move(other.m_n)}
	,ah{std::move(other.ah)}
{
}

scalar& vxTriRef::computeArea()
{
	const auto pb = MU::closestPointInLine(p1,p2,p3);
	const auto b = p1.distance(p2);
	const auto h = p3.distance(pb);
	ah = (b * h) / 2.0;
	return ah;
}

scalar vxTriRef::area() const
{
	return ah;
}

v3& vxTriRef::computeNormal() 
{
	m_n = (p2-p1).cross(p3-p1).unit();
	return m_n;
}

v3 vxTriRef::normal() const
{
	return m_n;
}

bool vxTriRef::throwRay(const vxRay &ray) const
{
	return hasCollision(ray)==1;
}

int vxTriRef::throwRay(const vxRay &ray, vxCollision &collide) const
{
	if(!m_n.follows(ray.direction()))
	{
		return 0;
	}

	const scalar threshold = -0.0000001;
	const auto& p = MU::rectAndPlane(ray,p1,p2,p3);
	
	auto ta = area();
	//TODO:this needs a optimization.
	
	ta-=MU::area(p1,p2,p);
	if(ta<threshold)
		return 0;
	
	ta-=vxTriRef(p1,p,p3).computeArea();
	if(ta<threshold)
		return 0;

	ta-=vxTriRef(p,p2,p3).computeArea();
	if(ta<threshold)
		return 0;
	
	collide.setNormal(m_n);
	collide.setPosition(p);

	return 1;
}

bool vxTriRef::hasCollision(const vxRay &ray) const
{
	if(!m_n.follows(ray.direction()))
	{
		return false;
	}

	const scalar threshold = -0.0000001;
	const auto& p = MU::rectAndPlane(ray,p1,p2,p3);
	
	auto ta = area();
	//TODO:this needs a optimization.
	
	ta-=MU::area(p1,p2,p);
	if(ta<threshold)
		return false;
	
	ta-=vxTriRef(p1,p,p3).computeArea();
	if(ta<threshold)
		return false;

	ta-=vxTriRef(p,p2,p3).computeArea();
	if(ta<threshold)
		return false;

	return true;
}
