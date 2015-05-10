#include "ImageProperties.h"

using namespace vxCore;

ImageProperties::ImgChannels ImageProperties::channels() const
{
	return m_channels;
}

void ImageProperties::setChannels(const ImgChannels &channels)
{
	m_channels = channels;
}

ImageProperties::ImgFormat ImageProperties::format() const
{
	return m_format;
}

void ImageProperties::setFormat(const ImgFormat &format)
{
	m_format = format;
}


unsigned int ImageProperties::rx() const
{
	return m_rx;
}

void ImageProperties::setRx(unsigned int rx)
{
	m_rx = rx;
}

unsigned int ImageProperties::ry() const
{
	return m_ry;
}

void ImageProperties::setRy(unsigned int ry)
{
	m_ry = ry;
}


unsigned int vxCore::ImageProperties::numPixels() const
{
	return m_rx * m_ry;
}

unsigned int vxCore::ImageProperties::numChannels() const
{
	switch(m_channels)
	{
		case ImageProperties::ImgChannels::kRGB:
			return 3;
		break;
		case ImageProperties::ImgChannels::kRGBA:
			return 4;
		break;
		case ImageProperties::ImgChannels::kRGBZ:
			return 4;
		break;
		case ImageProperties::ImgChannels::kRGBAZ:
			return 5;
		break;
		default:
			return 0;
	};
}

unsigned int vxCore::ImageProperties::numElements() const
{
	return numPixels() * numChannels();
}

double ImageProperties::aspectRatio() const
{
	return m_rx/(double)m_ry;
}

