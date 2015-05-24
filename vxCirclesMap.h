#ifndef VXCIRCLESMAP_H
#define VXCIRCLESMAP_H
#include "vxMap2d.h"

namespace vxCore
{

class vxCirclesMap : public vxMap2d
{
	public:
		vxCirclesMap();
		
		// vxMap interface
	public:
		virtual vxColor compute(const vxCollision &collision) const;

	double m_radius[5] = {0.005, 0.3, 0.31, 0.4, 0.41};
		
	bool m_circle1 {true};
	bool m_circle2 {true};
	bool m_circle3 {false};
	bool m_margin {true};

	double m_margn = 0.045;
};


}
#endif // VXCIRCLESMAP_H
