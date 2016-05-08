#ifndef IMAGEDATA_H
#define IMAGEDATA_H
#include "memory"

#include "vxVector.h"
#include "ImageProperties.h"

namespace vxCore {

class ImageData
{
public:
	///
	/// \brief ImageData
	/// \param prop
	///
	ImageData(std::shared_ptr<ImageProperties> &prop);
	///
	/// \brief ImageData
	///
	ImageData();
	///
	/// \brief setProperties
	/// \param prop
	///
	void setProperties(std::shared_ptr<ImageProperties> &prop);
	///
	/// \brief m_prop
	///
	std::shared_ptr<ImageProperties> m_prop;
	
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
