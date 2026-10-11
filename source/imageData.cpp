#include "imageData.h"

using namespace vxCore;

ImageData::ImageData(ImagePropertiesHandle &prop) { m_prop = prop; }

bool ImageData::hasData() const
{
	return m_pc != nullptr || m_hdr != nullptr;
}

void ImageData::setProperties(ImagePropertiesHandle &prop) { m_prop = prop; }

unsigned char *ImageData::get(const v2s &coords) const
{
	size_t numElements = m_prop->numElements();
	const unsigned long &&compX = coords.y() * (m_prop->rx() + 1);
	const unsigned long &&compY = coords.x() * (m_prop->ry() + 1);
	unsigned long dist = (compX + (compY * m_prop->rx())) * m_prop->numChannels();
	// std::cout << (m_prop->rx() * m_prop->ry() * m_prop->numChannels()) << " "
	// << dist << std::endl;
	if (dist > numElements)
	{
		dist = numElements;
	}

	return m_pc.get() + dist;
}

void ImageData::adoptFloatRGBA(float *pixels) { m_hdr.reset(pixels); }

std::array<double, 4> ImageData::sample(const v2s &coords) const
{
	const unsigned int channels = m_prop->numChannels();
	const unsigned int rx = m_prop->rx();
	const unsigned int ry = m_prop->ry();
	if (channels == 0 || rx == 0 || ry == 0)
	{
		return {0.0, 0.0, 0.0, 1.0};
	}

	// numElements() is a 32-bit product and overflows on large HDRIs.
	const unsigned long numElements = static_cast<unsigned long>(rx) *
									  static_cast<unsigned long>(ry) *
									  static_cast<unsigned long>(channels);
	if (numElements < channels)
	{
		return {0.0, 0.0, 0.0, 1.0};
	}

	// Image::compute passes (1-v, u): x is vertical, y is horizontal.
	// Nearest texel inside the image. The old scale by (size + 1) stepped
	// off the last row.
	double u = coords.y();
	double v = coords.x();
	if (u < 0.0)
	{
		u = 0.0;
	}
	else if (u > 1.0)
	{
		u = 1.0;
	}
	if (v < 0.0)
	{
		v = 0.0;
	}
	else if (v > 1.0)
	{
		v = 1.0;
	}

	unsigned long x = static_cast<unsigned long>(u * static_cast<double>(rx));
	unsigned long y = static_cast<unsigned long>(v * static_cast<double>(ry));
	if (x >= rx)
	{
		x = static_cast<unsigned long>(rx - 1);
	}
	if (y >= ry)
	{
		y = static_cast<unsigned long>(ry - 1);
	}

	unsigned long dist = (x + y * static_cast<unsigned long>(rx)) * channels;

	if (m_hdr)
	{
		const float *px = m_hdr.get() + dist;
		return {px[0], px[1], px[2], channels > 3 ? px[3] : 1.0};
	}

	const unsigned char *px = m_pc.get() + dist;
	return {px[0] / 255.0, px[1] / 255.0, px[2] / 255.0,
			channels > 3 ? px[3] / 255.0 : 1.0};
}

unsigned char *ImageData::initialise()
{
	static_assert(sizeof(float) == 4, "float is no 32bits");
	static_assert(sizeof(double) == 8, "double is no 64bits");
	static_assert(sizeof(unsigned char) == 1, "unsigned char is no 8bits");

	size_t numElements = m_prop->numElements();
	switch (m_prop->format())
	{
	case ImageProperties::ImgFormat::k8:
		m_pc.reset(new unsigned char[numElements]);
		break;
	case ImageProperties::ImgFormat::k16:
		m_pc.reset(new unsigned char[numElements * 2]);
		break;
	case ImageProperties::ImgFormat::k32:
		m_pc.reset(new unsigned char[numElements * 4]);
		break;
	case ImageProperties::ImgFormat::k64:
		m_pc.reset(new unsigned char[numElements * 8]);
		break;
	default:
		m_pc.reset(new unsigned char[numElements]);
		break;
	}

	// hardcode buffer type!!
	unsigned char *buff = m_pc.get();
	unsigned char *tmpp = buff;

	for (; tmpp < buff + numElements; tmpp++)
	{
		*tmpp = 0b00000000;
	}

	return buff;
}

void ImageData::copyData(const unsigned char *iBuff)
{
	size_t nElements = m_prop->numElements();

	unsigned char *buff = m_pc.get();
	unsigned char *tmpp = buff;
	auto iTmpp = iBuff;
	while (tmpp < buff + nElements)
	{
		*tmpp = *iTmpp;
		tmpp++;
		iTmpp++;

		*tmpp = *iTmpp;
		tmpp++;
		iTmpp++;

		*tmpp = *iTmpp;
		tmpp++;
		iTmpp++;

		*tmpp = *iTmpp;
		tmpp++;
		iTmpp++;
	}
}
