#ifndef VXISOGEOMETRY_H
#define VXISOGEOMETRY_H

#include "Geometry.h"

namespace vxCore {

class IsoGeometry : public Geometry
{
public:
	IsoGeometry();

	//renderable interface
	virtual bool throwRay(const Ray &ray) const;
	virtual int throwRay(const Ray &ray, Collision &collide) const;
	virtual bool hasCollision(const Ray &ray) const;
	
};

}
#endif // VXISOGEOMETRY_H
