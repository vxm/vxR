#ifndef ISOGEOMETRY_H
#define ISOGEOMETRY_H

#include "Geometry.h"

namespace vxCore {

class IsoGeometry: public Geometry
{
	scalar m_radius = 1.0;
	
public:
	IsoGeometry();
	
	//renderable interface
	virtual bool throwRay(const Ray &ray) const;
	virtual int throwRay(const Ray &ray, Collision &col) const;
	virtual bool hasCollision(const Ray &ray) const;
	
	scalar radius() const;
	void setRadius(const scalar &radius);
};

using IsoGeometryHandle = std::shared_ptr<IsoGeometry>;

}
#endif // ISOGEOMETRY_H
