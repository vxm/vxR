#ifndef VXGEOMETRY_H
#define VXGEOMETRY_H

#include <functional>
#include <vector>
#include <atomic>
#include "Ray.h"
//#include "Shader.h"
#include "Collision.h"
#include "TriRef.h"
#include "Matrix44.h"
#include "BoundingBox.h"
#include "GeoGrid.h"
#include "Color.h"
#include <set>
#include "Vector.h"
#include "visible.h"

namespace vxCore
{

class Geometry;

using GeometryHandle = std::shared_ptr<Geometry>;
using rayFn = std::function<int(const Ray &, Collision &)>;
using rayCondFn = std::function<int(const Ray &, Collision &, rayFn &)>;

class Geometry : public Visible
{
protected:
public:
	// renderable interface
	virtual bool throwRay(const Ray &) const override { return false; }

	virtual int throwRay(const Ray &, Collision &) const override
	{
		return 0;
	}

	virtual bool hasCollision(const Ray &) const override { return false; }
};
}
#endif // VXGEOMETRY_H
