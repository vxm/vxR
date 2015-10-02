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
	ImageData m_data;
	std::string m_path;

public:
	vxBitMap2d(const std::string path);
	virtual vxColor compute(const vxCollision &collision) const;
	std::shared_ptr<ImageProperties> m_prop;
	bool loadImage();
	std::string path() const;
	void setPath(const std::string &path);
};


}

#endif // VXBITMAP2D_H
