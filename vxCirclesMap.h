#ifndef VXCIRCLESMAP_H
#define VXCIRCLESMAP_H

#include <vector>

#include "vxVector.h"
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

	std::vector<scalar> m_radii = {0.05, 0.2, 0.32, 0.45, 0.5};
	std::vector<vxColor> m_colors = {vxColor::white, 
									 vxColor::orange,
									 vxColor::white,
									 vxColor::red,
									 vxColor::black};

	
	bool m_circle1 {false};
	bool m_circle2 {true};
	bool m_circle3 {false};
	bool m_margin  {true};

	scalar m_margn = 0.05;

	vxColor m_marginColor = vxColor::black;
};


}
#endif // VXCIRCLESMAP_H
