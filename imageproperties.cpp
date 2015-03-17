#include "imageproperties.h"




ImgFormat ImageProperties::format() const
{
	return m_format;
}

void ImageProperties::setFormat(const ImgFormat &format)
{
	m_format = format;
}

ImgFormat ImageProperties::channels() const
{
	return m_channels;
}

void ImageProperties::setChannels(const ImgFormat &channels)
{
	m_channels = channels;
}

ImgChannels ImageProperties::channels() const
{
	return m_channels;
}

void ImageProperties::setChannels(const ImgChannels &channels)
{
	m_channels = channels;
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


unsigned int vxStorage::ImageProperties::numPixels() const
{
	return m_rx * m_ry;
}

unsigned int vxStorage::ImageProperties::numChannels() const
{
	switch(m_channels)
	{
		case kRGB:
			return 3;
		break;
		case kRGBA:
			return 4;
		break;
		case kRGBZ:
			return 4;
		break;
		case kRGBAZ:
			return 5;
		break;
		default:
			return 0;
	};
}

unsigned int vxStorage::ImageProperties::numElements() const
{
	return numPixels() * numChannels();
}
