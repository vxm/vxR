#ifndef VXMAP_H
#define VXMAP_H

#include <vxObject.h>
#include<vxCollision.h>

namespace vxStorage {


class vxMap : public vxObject
{
	public:
		vxMap();
	
	virtual void getColor(const vxCollision &collision) = 0;
};

}
#endif // VXTEXTURE_H
