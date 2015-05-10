#ifndef VXPOINT_H
#define VXPOINT_H

#include "vxVector.h"

namespace vxCore {


class vxPoint:public vxVector3d
{
public:
	
	vxPoint()
		:vxVector3d()
	{
	}
	
	vxPoint(double x,
			double y,
			double z)
		:vxVector3d(x,y,z)
	{
	}

	void operator=(const vxVector3d &otro)
	{
		set(otro.x(),otro.y(),otro.z());
	}
};


} // namespace vxCore
#endif // VXPOINT_H
