#include "vxTriRef.h"

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
