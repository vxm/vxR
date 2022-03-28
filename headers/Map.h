#ifndef VXMAP_H
#define VXMAP_H

#include "Object.h"
#include "Collision.h"
#include "Color.h"

namespace vxCore {


class Map
{
	public:
		Map() = default;
	
	virtual Color compute(const Collision &collision) const = 0;
};

}
#endif // VXTEXTURE_H
