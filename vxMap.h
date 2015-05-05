#ifndef VXMAP_H
#define VXMAP_H

#include <vxObject.h>
#include<vxCollision.h>
#include<vxColor.h>

namespace vxStorage {


class vxMap : public vxObject
{
	public:
		vxMap();
	
	virtual vxColor getColor(const vxCollision &collision) const = 0;
};

}
#endif // VXTEXTURE_H
