#ifndef IMAGEDATA_H
#define IMAGEDATA_H
#include "memory"

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
	
	std::unique_ptr <unsigned char[]>	m_pc = nullptr;
	std::unique_ptr <double[]>			m_pd = nullptr;
	std::unique_ptr <float[]>			m_pf = nullptr;
	///
	/// \brief get
	/// \param coords
	/// \return 
	///
	unsigned char *get(const v2s &coords) const;
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
