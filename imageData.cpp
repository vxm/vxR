#include "imageData.h"

using namespace vxCore;

ImageData::ImageData(std::shared_ptr<ImageProperties> &prop)
{
	m_prop = prop;
}

unsigned char* ImageData::initialise()
{
	static_assert(sizeof(float)==4, "double is no 32bits");
	static_assert(sizeof(double)==8, "double is no 64bits");
	static_assert(sizeof(unsigned char)==1, "unsigned char is no 8bits");

	size_t numElements = m_prop->numElements();
	switch(m_prop->format())
	{
		case ImageProperties::ImgFormat::k8:
			m_pc.reset(new unsigned char[numElements]);
		break;
		case ImageProperties::ImgFormat::k32:
			m_pf.reset(new float[numElements]);
		break;
		case ImageProperties::ImgFormat::k64:
			m_pd.reset(new double[numElements]);
		break;
		default:
			m_pc.reset(new unsigned char[numElements]);
		break;
	}
	
	// hardcode buffer type!!
	unsigned char *buff = m_pc.get();
	unsigned char *tmpp = buff;	
	for(;tmpp<buff+numElements;tmpp++)
	{
		*tmpp = '\0';
	}		
	
	return buff;
}
