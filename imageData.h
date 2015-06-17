#ifndef IMAGEDATA_H
#define IMAGEDATA_H
#include "memory"
#include "ImageProperties.h"

namespace vxCore {

class ImageData
{
public:
	ImageData(std::shared_ptr<ImageProperties> &prop);
	std::shared_ptr<ImageProperties> m_prop;

	std::unique_ptr <unsigned char[]>	m_pc = nullptr;
	std::unique_ptr <double[]>			m_pd = nullptr;
	std::unique_ptr <float[]>			m_pf = nullptr;

	unsigned char *initialise();
};

}
#endif // IMAGEDATA_H
