#include "imageData.h"

using namespace vxCore;

ImageData::ImageData(ImagePropertiesHandle &prop) { m_prop = prop; }

bool ImageData::hasData() const { return m_pc.get() != nullptr; }

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
