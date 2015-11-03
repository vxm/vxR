#ifndef VXPOINT_H
#define VXPOINT_H

#include "vxVector.h"

namespace vxCore {


class vxPoint:public v3
{
public:
	
	vxPoint()
		:v3{}
	{
	}
	
	vxPoint(scalar x,
			scalar y,
			scalar z)
		:v3{x,y,z}
	{
	}

	void operator=(const v3 &otro)
	{
		set(otro.x(),otro.y(),otro.z());
	}
};


} // namespace vxCore
#endif // VXPOINT_H
