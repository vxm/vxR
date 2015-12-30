#ifndef VXBITMAP2D_H
#define VXBITMAP2D_H
#include "vxVector.h"
#include "vxMap2d.h"
#include "ImageProperties.h"
#include "MathUtils.h"
#include "imageData.h"

namespace vxCore
{

class vxBitMap2d : public vxMap2d
{
	ImageData m_data;
	std::string m_path;
	scalar m_gain{0.0};
	scalar m_gamma{1.0};

public:
	vxBitMap2d(const std::string path);
	virtual vxColor compute(const vxCollision &collision) const;
	std::shared_ptr<ImageProperties> m_prop;
	bool loadImage();
	std::string path() const;
	void setPath(const std::string &path);
	scalar gain() const;
	void setGain(const scalar &gain);
	scalar gamma() const;
	void setGamma(const scalar &gamma);
};


}

#endif // VXBITMAP2D_H
