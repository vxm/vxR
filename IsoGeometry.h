#ifndef ISOGEOMETRY_H
#define ISOGEOMETRY_H

#include "Geometry.h"

namespace vxCore {

class IsoGeometry: public Geometry
{
public:
	IsoGeometry();

	//renderable interface
	virtual bool throwRay(const Ray &ray) const;
	virtual int throwRay(const Ray &ray, Collision &col) const;
	virtual bool hasCollision(const Ray &ray) const;
	
};

using IsoGeometryHandle = std::shared_ptr<IsoGeometry>;

}
#endif // ISOGEOMETRY_H
