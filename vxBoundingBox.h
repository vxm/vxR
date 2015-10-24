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
	
	double m_minx;
	double m_miny;
	double m_minz;

	double m_maxx;
	double m_maxy;
	double m_maxz;

public:
	vxBoundingBox();
	
	void extend(const vxVector3d &limit);
	double minx() const;
	void setMinx(double minx);
	double miny() const;
	void setMiny(double miny);
	double minz() const;
	void setMinz(double minz);
	double maxx() const;
	void setMaxx(double maxx);
	double maxy() const;
	void setMaxy(double maxy);
	double maxz() const;
	void setMaxz(double maxz);

	bool contains(const vxVector3d &v) const;
	
	
	//renderable interface
	bool throwRay(const vxRay &ray) const;
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &ray) const;
};

}
#endif // VXBOUNDINGBOX_H
