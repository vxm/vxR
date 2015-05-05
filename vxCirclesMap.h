#ifndef VXCIRCLESMAP_H
#define VXCIRCLESMAP_H
#include<vxMap2d.h>

namespace vxStorage
{

class vxCirclesMap : public vxMap2d
{
	public:
		vxCirclesMap();
		
		// vxMap interface
	public:
		virtual vxColor getColor(const vxCollision &collision) const;
};


}
#endif // VXCIRCLESMAP_H
