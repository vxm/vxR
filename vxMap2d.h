#ifndef VXMAP2D_H
#define VXMAP2D_H

#include "vxMap.h"

namespace vxCore
{

class vxMap2d : public vxMap
{
	public:
		vxMap2d();
		
		// vxMap interface
	public:
		virtual vxColor getColor(const vxCollision &collision) const  = 0;
};

}
#endif // VXMAP2D_H
