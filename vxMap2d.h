#ifndef VXMAP2D_H
#define VXMAP2D_H

#include<vxMap.h>

namespace vxStorage
{

class vxMap2d : public vxMap
{
	public:
		vxMap2d();
		
		// vxMap interface
	public:
		virtual void getColor(const vxCollision &collision) = 0;
};

}
#endif // VXMAP2D_H
