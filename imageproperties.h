#ifndef IMAGEPROPERTIES_H
#define IMAGEPROPERTIES_H

namespace vxStorage {

class ImageProperties
{
	enum class ImgFormat;
	enum class ImgChannels;
	
	ImgFormat m_format = {ImgFormat::k8};
	ImgChannels m_channels = {ImgChannels::kRGBA};
	unsigned int m_rx = {120};
	unsigned int m_ry = {70};
	
public:
	ImageProperties()
	{}
	
	ImageProperties(const unsigned int rx, const unsigned int ry)
		: m_rx(rx)
		, m_ry(ry)
	{
	}
	
	// defines the size in bits for each channel.
	enum class ImgFormat {k8, k16, k32, k64};
	// enumerates the channels to store.
	enum class ImgChannels {kRGB, kRGBA, kRGBZ, kRGBAZ};

	ImgFormat format() const;
	void setFormat(const ImgFormat &format);

	ImgChannels channels() const;
	void setChannels(const ImgChannels &channels);
	
	unsigned int rx() const;
	void setRx(unsigned int rx);
	
	unsigned int ry() const;
	void setRy(unsigned int ry);

	// returns the number of pixels
	unsigned int numPixels() const;
	
	// returns the number of channels
	unsigned int numChannels() const;
	
	// returns the number of values in image
	// channels multiplied by pixels
	unsigned int numElements() const;
};


}
#endif // IMAGEPROPERTIES_H
