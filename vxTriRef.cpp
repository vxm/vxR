#include "vxTriRef.h"
#include "MathUtils.h"

using namespace vxCore;

vxCore::vxTriRef::vxTriRef(const vxVector3d &a, 
						   const vxVector3d &b, 
						   const vxVector3d &c)
	:p1(a)
	,p2(b)
	,p3(c)
	,n{vxVector3d::zero}
{
}

vxTriRef::vxTriRef(vxTriRef &&other)
	:p1{std::move(other.p1)}
	,p2{std::move(other.p2)}
	,p3{std::move(other.p3)}
	,n{std::move(other.n)}
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

vxVector3d& vxTriRef::computeNormal() 
{
	n = (p2-p1)^(p3-p1).unit();
	return n;
}

vxVector3d vxTriRef::normal() const
{
	return n;
}

int vxTriRef::throwRay(const vxRay &ray, vxCollision &collide) const
{
	if(!n.follows(ray.direction()))
	{
		return 0;
	}

	const scalar threshold = -0.001;
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
	
	collide.setNormal(n);
	collide.setPosition(p);
	collide.setValid(true);
	collide.setColor(vxColor::white);

	return 1;
}

