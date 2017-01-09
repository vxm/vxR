#ifndef VXGEOMETRY_H
#define VXGEOMETRY_H

#include <functional>
#include <typeinfo>
#include <vector>
#include <atomic>
#include "Vector.h"
#include "Ray.h"
//#include "Shader.h"
#include "Collision.h"
#include "TriRef.h"
#include "Matrix44.h"
#include "BoundingBox.h"
#include "GeoGrid.h"
#include "Color.h"
#include <set>
#include <visible.h>

namespace vxCore {

class Geometry;

using GeometryHandle = std::shared_ptr<Geometry>;
using rayFn = std::function<int(const Ray &,Collision &)>;
using rayCondFn = std::function<int(const Ray &,Collision &, rayFn&)>;

class Geometry : public Visible
{
protected:
	
public:
	
	//renderable interface
	virtual bool throwRay(const Ray &ray) const override
	{
		return false;
	}
	virtual int throwRay(const Ray &ray, Collision &collide) const override
	{
		return 0;
	}

	virtual bool hasCollision(const Ray &ray) const override
	{
		return false;
	}
};

}
#endif // VXGEOMETRY_H
