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

	double m_radius[5] = {0.05, 0.1, 0.12, 0.35, 0.45};
		
	bool m_circle1 {true};
	bool m_circle2 {true};
	bool m_circle3 {true};
	bool m_margin  {true};

	double m_margn = 0.02;
};


}
#endif // VXCIRCLESMAP_H
