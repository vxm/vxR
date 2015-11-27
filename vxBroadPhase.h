#ifndef VXBROADPHASE_H
#define VXBROADPHASE_H
#include "vxRay.h"
#include "vxCollision.h"
#include "vxVector.h"
#include "vxGeometry.h"

namespace vxCore {


class vxBroadPhase
{
	
	std::vector<std::shared_ptr<vxGeometry>> m_geometries;
	
	std::shared_ptr<vxBoundingBox> m_root;
	
public:
	vxBroadPhase();

	void addGeometry(std::shared_ptr<vxGeometry> geo);
	
	//renderable interface
	bool throwRay(const vxRay &ray) const;
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &ray) const;
};

}
#endif // VXBROADPHASE_H
