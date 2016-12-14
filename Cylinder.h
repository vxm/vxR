#ifndef CYLINDER_H
#define CYLINDER_H

#include "IsoGeometry.h"

namespace vxCore {

class Cylinder final : public IsoGeometry
{
	scalar m_radius = 0.50;

public:
	Cylinder();
	
	scalar radius() const;
	void setRadius(const scalar &radius);
	
	// Geometry interface
public:
	virtual bool throwRay(const Ray &ray) const override;
	virtual int throwRay(const Ray &ray, Collision &collide) const override;
	virtual bool hasCollision(const Ray &ray) const override;
};

using CylinderHandle = std::shared_ptr<Cylinder>;

}
#endif // CYLINDER_H
