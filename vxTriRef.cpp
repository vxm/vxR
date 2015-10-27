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
	,normal(false)
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

vxVector3d& vxTriRef::getNormal() 
{
	if(normal==true)
	{
		return n;
	}
	
	n = (p1-p3).cross(p1-p2).unit();
	normal = true;
	return n;
}

double vxTriRef::area() const
{
	const auto pb = MU::closestPointInLine(p1,p2,p3);
	const auto b = p1.distance(p2);
	const auto h = p3.distance(pb);
	return (b * h) / 2.0;
}

int vxTriRef::throwRay(const vxRay &ray, vxCollision &collide) const
{
	const double threshold = -0.001;
	const auto& p = MU::rectAndPlane(ray,p1,p2,p3);
	
	auto ta = area();
	//TODO:this needs a optimization.
	ta-=vxTriRef(p1,p2,p).area();
	if(ta<threshold)
		return 0;
	
	ta-=vxTriRef(p1,p,p3).area();
	if(ta<threshold)
		return 0;

	ta-=vxTriRef(p,p2,p3).area();
	if(ta<threshold)
		return 0;
	
	collide.setPosition(p);
	collide.setValid(true);
	collide.setColor(vxColor::white);

	return 1;
}
