#ifndef VXBITMAP2D_H
#define VXBITMAP2D_H
#include "vxMap2d.h"
#include "MathUtils.h"

namespace vxCore
{

class vxBitMap2d : public vxMap2d
{
	public:
		vxBitMap2d();
		
		// vxMap interface
	public:
		virtual vxColor compute(const vxCollision &collision) const;

	double m_radius[5] = {0.1, 0.6, 1.0, 3.0};
		
	bool m_circle1 {true};
	bool m_circle2 {true};
	bool m_circle3 {true};
};


}

#endif // VXBITMAP2D_H
