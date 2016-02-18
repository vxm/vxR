#ifndef VXPOINT_H
#define VXPOINT_H

#include "vxVector.h"

namespace vxCore {


class vxPoint:public v3s
{
public:
	
	vxPoint()
		:v3s{}
	{
	}
	
	vxPoint(scalar x,
			scalar y,
			scalar z)
		:v3s{x,y,z}
	{
	}

	void operator=(const v3s &otro)
	{
		set(otro.x(),otro.y(),otro.z());
	}
};


} // namespace vxCore
#endif // VXPOINT_H
