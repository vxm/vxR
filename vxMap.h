#ifndef VXMAP_H
#define VXMAP_H

#include "vxObject.h"
#include "vxCollision.h"
#include "vxColor.h"

namespace vxCore {


class vxMap
{
	public:
		vxMap();
	
	virtual vxColor compute(const vxCollision &collision) const = 0;
};

}
#endif // VXTEXTURE_H
