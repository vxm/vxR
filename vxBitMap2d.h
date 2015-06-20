#ifndef VXBITMAP2D_H
#define VXBITMAP2D_H
#include "vxMap2d.h"
#include "ImageProperties.h"
#include "MathUtils.h"
#include "imageData.h"

namespace vxCore
{

class vxBitMap2d : public vxMap2d
{
public:
	vxBitMap2d();
	
	virtual vxColor compute(const vxCollision &collision) const;

	std::shared_ptr<ImageProperties> m_prop;
	ImageData m_data;

	double m_radius[5] = {1.1, 2.6, 3.0, 7.0};
	bool m_circle1 {true};
	bool m_circle2 {true};
	bool m_circle3 {true};
};


}

#endif // VXBITMAP2D_H
