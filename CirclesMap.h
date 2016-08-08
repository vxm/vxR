#ifndef VXCIRCLESMAP_H
#define VXCIRCLESMAP_H

#include <vector>

#include "Vector.h"
#include "Map2d.h"

namespace vxCore
{

class CirclesMap : public Map2d
{
public:
	CirclesMap();
	
	// vxMap interface
public:
	virtual Color compute(const Collision &collision) const;

	std::vector<scalar> m_radii = {0.05, 0.09, 0.32, 0.45, 0.5};
	std::vector<Color> m_colors = {Color::red, 
									 Color::orange,
									 Color::blue,
									 Color::red,
									 Color::black};

	
	bool m_circle1 {true};
	bool m_circle2 {true};
	bool m_circle3 {true};
	bool m_margin  {true};

	scalar m_margn = 0.04;

	Color m_marginColor = Color::black;
};


}
#endif // VXCIRCLESMAP_H
