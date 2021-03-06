#ifndef SPHERE_H
#define SPHERE_H

#include "ConvexGeometry.h"

namespace vxCore
{

class Sphere final : public ConvexGeometry
{
	scalar m_radius = 0.50;

public:
	Sphere() = default;

	scalar radius() const;
	void setRadius(const scalar &radius);
	
	// Visible interface
	virtual void updateBoundingBox() override;
	virtual bool throwRay(const Ray &ray) const override;
	virtual int throwRay(const Ray &ray, Collision &collide) const override;
	virtual bool hasCollision(const Ray &ray) const override;
};

using SphereHandle = std::shared_ptr<Sphere>;
}

#endif // SPHERE_H
