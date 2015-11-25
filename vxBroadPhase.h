#ifndef VXBROADPHASE_H
#define VXBROADPHASE_H
#include "vxRay.h"
#include "vxCollision.h"
#include "vxVector.h"

namespace vxCore {


class vxBroadPhase
{
public:
	vxBroadPhase();

	//renderable interface
	bool throwRay(const vxRay &) const;
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &) const;
};

}
#endif // VXBROADPHASE_H
