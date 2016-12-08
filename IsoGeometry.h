#ifndef ISOGEOMETRY_H
#define ISOGEOMETRY_H

#include "Geometry.h"

namespace vxCore {

class IsoGeometry: public Geometry
{
public:
	IsoGeometry();

};

using IsoGeometryHandle = std::shared_ptr<IsoGeometry>;

}
#endif // ISOGEOMETRY_H
