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

	double m_radius[5] = {0.05, 0.2, 0.32, 0.45, 0.5};
		
	bool m_circle1 {false};
	bool m_circle2 {false};
	bool m_circle3 {false};
	bool m_margin  {true};

	double m_margn = 0.07;
};


}
#endif // VXCIRCLESMAP_H
