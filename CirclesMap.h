#ifndef VXCIRCLESMAP_H
#define VXCIRCLESMAP_H

#include <vector>

#include "Map2d.h"
#include "Vector.h"

namespace vxCore
{

class CirclesMap : public Map2d
{

	// vxMap interface
public:
	virtual Color compute(const Collision &collision, const Color &innerColor,
						  const Color &outerColor) const;

	Color m_marginColor = Color::black;

	// Map interface
public:
	virtual Color compute(const Collision &collision) const override;
};

} // namespace vxCore
#endif // VXCIRCLESMAP_H
