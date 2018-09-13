#ifndef ConvexGeometry_H
#define ConvexGeometry_H

#include "Geometry.h"

namespace vxCore {

class ConvexGeometry: public Geometry
{
public:
	ConvexGeometry();

};

using ConvexGeometryHandle = std::shared_ptr<ConvexGeometry>;

}
#endif // ConvexGeometry_H
