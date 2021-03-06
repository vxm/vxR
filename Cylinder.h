#ifndef CYLINDER_H
#define CYLINDER_H

#include "ConvexGeometry.h"

namespace vxCore {

class Cylinder final : public ConvexGeometry
{
	scalar m_radius = 0.50;
	scalar m_height = 0.50;
	
public:
	Cylinder() = default;
	
	scalar radius() const;
	void setRadius(const scalar &radius);
	
	scalar height() const;
	void setHeight(const scalar &height);
	
	// Geometry interface
	virtual void updateBoundingBox() override;
	
	virtual bool throwRay(const Ray &ray) const override;
	virtual int throwRay(const Ray &ray, Collision &collide) const override;
	virtual bool hasCollision(const Ray &ray) const override;
};

using CylinderHandle = std::shared_ptr<Cylinder>;

}
#endif // CYLINDER_H
