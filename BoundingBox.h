#ifndef VXBOUNDINGBOX_H
#define VXBOUNDINGBOX_H

#include <memory>
#include <atomic>
#include "Vector.h"
#include "Ray.h"
#include "Collision.h"
#include "MathUtils.h"

namespace vxCore {

 class BoundingBox;


using BoundingBoxHandle = std::shared_ptr<BoundingBox>;

//TODO: this class needs heavy caching on "close" method.
class BoundingBox: public std::enable_shared_from_this<BoundingBox>
{
	std::atomic_bool m_init{false};
	
	scalar m_minx;
	scalar m_miny;
	scalar m_minz;

	scalar m_maxx;
	scalar m_maxy;
	scalar m_maxz;

//	v3 m_scale;

public:

	BoundingBox();
	void clear();
	void set(v3s position, scalar size);
	void set(scalar mnx,scalar mny,scalar mnz,
			 scalar mxx,scalar mxy,scalar mxz);
	
	void markAsInitialized();
	void close();
	
	void extend(const v3s &limit);
	void extend(const BoundingBox &box);

	scalar xLength() const;
	scalar yLength() const;
	scalar zLength() const;

	v3s max() const;
	v3s min() const;
	
	v3s center() const;
	scalar minX() const;
	void setMinX(scalar minx);
	scalar minY() const;
	void setMinY(scalar miny);
	scalar minZ() const;
	void setMinZ(scalar minz);
	scalar maxX() const;
	void setMaxX(scalar maxx);
	scalar maxY() const;
	void setMaxY(scalar maxy);
	scalar maxZ() const;
	void setMaxZ(scalar maxz);

	bool contains(const v3s &v) const;
	bool contains(const v3s &v, scalar tolerance) const;
	v3s diagonal() const;
	
	//renderable interface
	bool throwRay(const Ray &ray) const;
	int throwRay(const Ray &ray, Collision &collide) const;
	bool hasCollision(const Ray &ray) const;
};
}
#endif // VXBOUNDINGBOX_H
