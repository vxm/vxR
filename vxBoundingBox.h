#ifndef VXBOUNDINGBOX_H
#define VXBOUNDINGBOX_H

#include <atomic>
#include "vxVector.h"
#include "vxRay.h"
#include "vxCollision.h"
#include "MathUtils.h"

namespace vxCore {


class vxBoundingBox
{
	std::atomic_bool m_init{false};
	
	scalar m_minx;
	scalar m_miny;
	scalar m_minz;

	scalar m_maxx;
	scalar m_maxy;
	scalar m_maxz;

public:
	vxBoundingBox();
	
	void extend(const vxVector3d &limit);
	scalar minx() const;
	void setMinx(scalar minx);
	scalar miny() const;
	void setMiny(scalar miny);
	scalar minz() const;
	void setMinz(scalar minz);
	scalar maxx() const;
	void setMaxx(scalar maxx);
	scalar maxy() const;
	void setMaxy(scalar maxy);
	scalar maxz() const;
	void setMaxz(scalar maxz);

	bool contains(const vxVector3d &v) const;
	
	
	//renderable interface
	bool throwRay(const vxRay &ray) const;
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &ray) const;
};

}
#endif // VXBOUNDINGBOX_H
