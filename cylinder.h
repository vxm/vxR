#ifndef CYLINDER_H
#define CYLINDER_H

#include "IsoGeometry.h"

namespace vxCore {

class Cylinder final : public IsoGeometry
{
	scalar m_radius;

public:
	Cylinder();
	
};


}
#endif // CYLINDER_H
