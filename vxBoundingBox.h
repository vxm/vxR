#ifndef VXBOUNDINGBOX_H
#define VXBOUNDINGBOX_H

#include <memory>
#include <atomic>
#include "vxVector.h"
#include "vxRay.h"
#include "vxCollision.h"
#include "MathUtils.h"

namespace vxCore {

 class vxBoundingBox;


using vxBoundingBoxHandle = std::shared_ptr<vxBoundingBox>;

//TODO: this class needs heavy caching on "close" method.
class vxBoundingBox: public std::enable_shared_from_this<vxBoundingBox>
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

	vxBoundingBox();
	void set(v3 position, scalar size);
	void set(scalar mnx,scalar mny,scalar mnz,
			 scalar mxx,scalar mxy,scalar mxz);
	
	void markAsInitialized();
	void close();
	
	void extend(const v3 &limit);
	void extend(const vxBoundingBox &box);

	v3 center() const;
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

	bool contains(const v3 &v) const;
	v3 diagonal() const;
	
	//renderable interface
	bool throwRay(const vxRay &ray) const;
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &ray) const;
};
}
#endif // VXBOUNDINGBOX_H
