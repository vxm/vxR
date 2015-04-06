#ifndef VXPOINT_H
#define VXPOINT_H

#include<vxVector.h>

namespace vxStorage {


class vxPoint:public vxVector3d
{
public:
	
	vxPoint()
		:vxVector3d()
	{
	}

	void operator=(const vxVector3d &otro)
	{
		set(otro.getX(),otro.getY(),otro.getZ());
	}
};


} // namespace vxStorage
#endif // VXPOINT_H
