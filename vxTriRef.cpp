#include "vxTriRef.h"
#include "MathUtils.h"

using namespace vxCore;

vxCore::vxTriRef::vxTriRef(const v3s &a, 
						   const v3s &b, 
						   const v3s &c)
	:p1(a)
	,p2(b)
	,p3(c)
{
}

vxTriRef::vxTriRef(vxTriRef &&other)
	:p1{std::move(other.p1)}
	,p2{std::move(other.p2)}
	,p3{std::move(other.p3)}
	,n1{std::move(other.n1)}
	,n2{std::move(other.n2)}
	,n3{std::move(other.n3)}
	,ah{std::move(other.ah)}
	,m_c_h1{std::move(other.m_c_h1)}
	,m_c_h2{std::move(other.m_c_h2)}
{
}

scalar& vxTriRef::computeArea()
{
	m_c_h1 = fabs(MU::distanceToLine(p1,p2,p3));
	m_c_h2 = fabs(MU::distanceToLine(p2,p3,p1));
	ah = (p1.distance(p2) * m_c_h1) / 2.0;
	return ah;
}

scalar vxTriRef::area() const
{
	return ah;
}

scalar &vxTriRef::area()
{
	return ah;
}

v3s vxTriRef::computeNormals() 
{
	auto&& c1 = p2-p1;
	auto&& c2 = p3-p1;
	n1 = c1.cross(c2).unit();
	n2 = n1;
	n3 = n1;
	return n1;
}

v3s vxTriRef::normal() const
{
	return n1;
}

bool vxTriRef::throwRay(const vxRay &ray) const
{
	return hasCollision(ray)==1;
}

int vxTriRef::throwRay(const vxRay &ray, vxCollision &collide) const
{
	if(!n1.follows(ray.direction()))
	{
		return 0;
	}

	const scalar threshold = -0.0000001;
	const auto& p = MU::rectAndPlane(ray,p1,p2,p3);
	
	auto ta = area();
	
	ta-=MU::area(p1,p2,p);
	if(ta<threshold)
		return 0;
	
	ta-=vxTriRef(p1,p,p3).computeArea();
	if(ta<threshold)
		return 0;
	
	ta-=vxTriRef(p,p2,p3).computeArea();
	if(ta<threshold)
		return 0;
	
	collide.setNormal(n1);
	collide.setPosition(p);
	
	auto t = MU::distanceToLine(p1,p2,p) / m_c_h1;
	auto s = MU::distanceToLine(p2,p3,p) / m_c_h2;
	
	if(t<.01 || s<.01 || t>.9 || s>.9)
	{
		collide.setColor(vxColor(0.01));
	}
	else
	{
		collide.setColor(vxColor(0.4));
	}
	
	return 1;
}

bool vxTriRef::hasCollision(const vxRay &ray) const
{
	if(!n1.follows(ray.direction()))
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
