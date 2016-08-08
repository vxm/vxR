#ifndef VXPOINT_H
#define VXPOINT_H

#include "Vector.h"

namespace vxCore {


class Point final:public v3s
{
public:
	
	Point()
		:v3s{}
	{
	}
	
	Point(scalar x,
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
