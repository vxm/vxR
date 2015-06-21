#ifndef VXLEGOBLOCK_H
#define VXLEGOBLOCK_H

#include "vxBox.h"
namespace vxCore{

class vxLegoBlock
{
public:
	vxLegoBlock();
	
	
	
	// vxBoxN interface
public:
	int throwRay(const vxRayXYZ &ray, vxCollision &collide) const
	{
		collide.setValid(true);
		collide.setNormal(vxVector3d::constY);
		collide.setPosition(2,2,2);
		collide.setUV(vxVector2d(.5,.5));
		return 1;
	}
};

}
#endif // VXLEGOBLOCK_H
