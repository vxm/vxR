#include "TriRef.h"
#include "MathUtils.h"

using namespace vxCore;

vxCore::TriRef::TriRef(const v3s &a, 
						   const v3s &b, 
						   const v3s &c,
						   const v3s &in1, 
						   const v3s &in2, 
						   const v3s &in3,
						   const v2s &iuv1, 
						   const v2s &iuv2, 
						   const v2s &iuv3)
	:p1(a)
	,p2(b)
	,p3(c)
	,n1(in1)
	,n2(in2)
	,n3(in3)
	,uv1(iuv1)
	,uv2(iuv2)
	,uv3(iuv3)
{
}

TriRef::TriRef(TriRef &&other)
	:p1{std::move(other.p1)}
	,p2{std::move(other.p2)}
	,p3{std::move(other.p3)}
	,n1{std::move(other.n1)}
	,n2{std::move(other.n2)}
	,n3{std::move(other.n3)}
	,uv1{std::move(other.uv1)}
	,uv2{std::move(other.uv2)}
	,uv3{std::move(other.uv3)}
	,ah{std::move(other.ah)}
	,m_c_h1{std::move(other.m_c_h1)}
	,m_c_h2{std::move(other.m_c_h2)}
{
}

scalar& TriRef::computeArea()
{
	m_c_h1 = fabs(MU::distanceToLine(p1,p2,p3));
	m_c_h2 = fabs(MU::distanceToLine(p2,p3,p1));
	ah = (p1.distance(p2) * m_c_h1) / 2.0;
	return ah;
}

scalar TriRef::area() const
{
	return ah;
}

scalar &TriRef::area()
{
	return ah;
}

v3s TriRef::normal() const
{
	return n1;
}

bool TriRef::throwRay(const Ray &ray) const
{
	return hasCollision(ray)==1;
}

int TriRef::throwRay(const Ray &ray, Collision &collide) const
{
	if(!n1.follows(ray.direction()))
	{
		return 0;
	}
	
	const scalar threshold = -0.0000001;
	const auto& p = MU::rectAndPlane(ray,p1,p2,p3);
	
	auto ta = area();
	
	ta-= MU::area(p1,p2,p);
	if(ta<threshold)
		return 0;
	
	ta-= MU::area(p1,p,p3);
	if(ta<threshold)
		return 0;
	
	ta-= MU::area(p,p2,p3);
	if(ta<threshold)
		return 0;
	
	collide.setPosition(p);
	
	auto t = MU::distanceToLine(p1,p2,p) / m_c_h1;
	auto s = MU::distanceToLine(p2,p3,p) / m_c_h2;
	auto u = 1.0 - s - t;
	
	collide.setNormal(n1 * s + n2 * u + n3 * t);
	
	auto interUV = uv1 * s + uv2 * u + uv3 * t;
	collide.setUV( interUV );
	
	return 1;
}

bool TriRef::hasCollision(const Ray &ray) const
{
	if(!n1.follows(ray.direction()))
	{
		return false;
	}

	const scalar threshold = -0.0000001;
	const auto& p = MU::rectAndPlane(ray,p1,p2,p3);
	
	auto ta = area();
	//TODO:this needs a optimization.
	
	ta-= MU::area(p1,p2,p);
	if(ta<threshold)
		return false;
	
	ta-= MU::area(p1,p,p3);
	if(ta<threshold)
		return false;

	ta-= MU::area(p,p2,p3);
	if(ta<threshold)
		return false;

	return true;
}
