#include "vxTriRef.h"

using namespace vxCore;

vxCore::vxTriRef::vxTriRef(const vxVector3d &a, 
						   const vxVector3d &b, 
						   const vxVector3d &c)
	:p1(a)
	,p2(b)
	,p3(c)
{
	
}

vxVector3d vxTriRef::getNormal() 
{
	if(n)
	{
		return *n;
	}
	
	n = std::make_unique<vxVector3d>((p2-p1).cross(p3-p1));
	return *n;
}
