#ifndef IMAGEDATA_H
#define IMAGEDATA_H
#include "memory"
#include <array>
#include <cstdlib>

#include "Vector.h"
#include "ImageProperties.h"

namespace vxCore {

class ImageData
{
public:
	///
	/// \brief ImageData
	/// \param prop
	///
	ImageData(ImagePropertiesHandle &prop);
	///
	/// \brief ImageData
	///
	ImageData() = default;

	bool hasData() const;
	///
	/// \brief setProperties
	/// \param prop
	///
	void setProperties(ImagePropertiesHandle &prop);
	///
	/// \brief m_prop
	///
	ImagePropertiesHandle m_prop;
	
	struct MallocFree
	{
		void operator()(float *pixels) const { std::free(pixels); }
	};

	std::unique_ptr <unsigned char[]>	m_pc = nullptr;
	std::unique_ptr <double[]>			m_pd = nullptr;
	std::unique_ptr <float[]>			m_pf = nullptr;
	/// HDR pixels from malloc, RGBA float, linear scene values.
	std::unique_ptr <float, MallocFree>	m_hdr = nullptr;
	///
	/// \brief get
	/// \param coords
	/// \return 
	///
	unsigned char *get(const v2s &coords) const;
	///
	/// \brief sample
	/// \param coords
	/// \return RGBA. 8-bit images are scaled to 0..1. Float images stay linear HDR.
	///
	std::array<double, 4> sample(const v2s &coords) const;
	///
	/// \brief adoptFloatRGBA
	/// Takes ownership of a malloc'd RGBA float buffer.
	///
	void adoptFloatRGBA(float *pixels);
	///
	/// \brief initialise
	/// \return 
	///
	unsigned char *initialise();
	///
	/// \brief copyData
	/// \param iBuff
	///
	void copyData(const unsigned char *iBuff);
};

}
#endif // IMAGEDATA_H
