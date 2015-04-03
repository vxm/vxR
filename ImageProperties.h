#ifndef IMAGEPROPERTIES_H
#define IMAGEPROPERTIES_H

namespace vxStorage {

class ImageProperties
{
public: // special case for public at start of a class.
	// defines the size in bits for each channel.
	enum class ImgFormat {k8, k16, k32, k64};
	// enumerates the channels to store.
	enum class ImgChannels {kRGB, kRGBA, kRGBZ, kRGBAZ};

// proper begining of the class.
private:
	unsigned int m_rx = {120};
	unsigned int m_ry = {70};
	ImgFormat m_format = {ImgFormat::k8};
	ImgChannels m_channels = {ImgChannels::kRGBA};
	
public:
	ImageProperties()
	{}
	
	ImageProperties(const unsigned int rx, const unsigned int ry)
		: m_rx(rx)
		, m_ry(ry)
	{
	}
	
	
	ImageProperties::ImgFormat format() const;
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

	double aspectRatio() const;
};


}
#endif // IMAGEPROPERTIES_H
