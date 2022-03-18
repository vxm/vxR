#ifndef VXPOINT_H
#define VXPOINT_H

#include "Vector.h"

namespace vxCore {


class Point final
{
public:
	scalar m_x {0.0};
	scalar m_y {0.0};
	scalar m_z {0.0};
	
	Point()
	{
	}
	
	Point(scalar x,
			scalar y,
			scalar z)
		: m_x(x)
		, m_y(y)
		, m_z(z)
	{
	}

};

}
// namespace vxCore
#endif // VXPOINT_H
