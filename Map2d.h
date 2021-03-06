#ifndef VXMAP2D_H
#define VXMAP2D_H

#include "Map.h"

namespace vxCore
{

class Map2d : public Map
{
	public:
		Map2d() = default;
		
		virtual ~Map2d() = default;
		
		virtual Color compute(const Collision &collision) const = 0;
};

}
#endif // VXMAP2D_H
